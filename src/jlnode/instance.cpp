#include "instance.h"

using v8::V8;

std::string get_initialization_scripts(const char *addon_path) {
    return "globalThis.require = require('module').createRequire(process.cwd() + '/')\n"
           "return globalThis.require('" + std::string(addon_path) + "').initialize()";
}

namespace jlnode {

Config::Config(
    v8::Isolate *isolate,
    uv_loop_t *loop,
    node::MultiIsolatePlatform *platform,
    node::ArrayBufferAllocator *allocator
) : locker(isolate), isolate_scope(isolate), isolate_data(
    node::CreateIsolateData(isolate, loop, platform, allocator),
    node::FreeIsolateData
), handle_scope(isolate) {}


Environment::Environment(
    Config &config,
    v8::Local<v8::Context> context,
    std::vector<std::string> &args,
    std::vector<std::string> &exec_args
) : context_scope(context), env(
    node::CreateEnvironment(config.isolate_data.get(), context, args, exec_args),
    node::FreeEnvironment
) {}

Instance::Instance() = default;

int Instance::Initialize(const char *addon_path, napi_env *env) {
    char pt[256];
    uv_get_process_title(pt, 256);
    std::vector<std::string> args{std::string(pt)};
    std::vector<std::string> exec_args;
    std::vector<std::string> errors;
    auto exit_code = node::InitializeNodeWithArgs(&args, &exec_args, &errors);
    for (const std::string &error : errors)
        fprintf(stderr, "%s: %s\n", args[0].c_str(), error.c_str());
    if (exit_code != 0) {
        return exit_code;
    }
    platform = node::MultiIsolatePlatform::Create(1);
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();
    auto ret = uv_loop_init(&event_loop);
    if (ret != 0) {
        fprintf(stderr, "%s: Failed to initialize loop: %s\n",
                pt, uv_err_name(ret));
        return ret;
    }

    allocator = node::ArrayBufferAllocator::Create();
    isolate = node::NewIsolate(allocator, &event_loop, platform.get());
    if (isolate == nullptr) {
        fprintf(stderr, "%s: Failed to initialize V8 Isolate\n", pt);
        return 1;
    }

    config = new Config(isolate, &event_loop, platform.get(), allocator.get());

    context = node::NewContext(isolate);
    if (context.IsEmpty()) {
        fprintf(stderr, "%s: Failed to initialize V8 Context\n", pt);
        return 1;
    }
    environment = new Environment(*config, context, args, exec_args);
    auto init_scripts = get_initialization_scripts(addon_path);
    auto load_env_ret = node::LoadEnvironment(
        environment->env.get(),
        init_scripts.c_str()
    );
    if (load_env_ret.IsEmpty()) {
        return 1;
    }
    auto t = load_env_ret.ToLocalChecked();
    if (!t->IsBigInt()) {
        char buffer[256];
        t->ToDetailString(context).ToLocalChecked()->WriteUtf8(isolate, buffer);
        fprintf(stderr, "%s: %s should be an uint64_t\n", pt, buffer);
        return 1;
    }
    *env = (napi_env) t->ToBigInt(context).ToLocalChecked()->Uint64Value();

    initialized = true;
    return 0;
}

int Instance::Dispose() {
    if (!initialized) {
        return 0;
    }

    {
        v8::SealHandleScope seal(isolate);
        bool more;
        do {
            uv_run(&event_loop, UV_RUN_DEFAULT);
            platform->DrainTasks(isolate);
            more = uv_loop_alive(&event_loop);
            if (more) continue;
            node::EmitBeforeExit(environment->env.get());
            more = uv_loop_alive(&event_loop);
        } while (more);
    }

    auto exit_code = node::EmitExit(environment->env.get());
    node::Stop(environment->env.get());

    delete environment;
    delete config;
    auto platform_finished = false;
    platform->AddIsolateFinishedCallback(isolate, [](void *data) {
        *static_cast<bool *>(data) = true;
    }, &platform_finished);
    platform->UnregisterIsolate(isolate);
    isolate->Dispose();
    while (!platform_finished)
        uv_run(&event_loop, UV_RUN_ONCE);
    auto err = uv_loop_close(&event_loop);
    assert(err == 0);

    V8::Dispose();
    V8::ShutdownPlatform();
    return exit_code;
}

}
