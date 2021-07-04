#include "instance.h"

using v8::V8;

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

Instance::Instance() {
}

Instance::~Instance() {
    if (!initialized) {
        return;
    }
    V8::Dispose();
    V8::ShutdownPlatform();
}

int Instance::Initialize() {
    char pt[256];
    uv_get_process_title(pt, 256);
    std::vector<std::string> args{std::string(pt)};
    std::vector<std::string> exec_args;
    std::vector<std::string> errors;
    int exit_code = node::InitializeNodeWithArgs(&args, &exec_args, &errors);
    for (const std::string &error : errors)
        fprintf(stderr, "%s: %s\n", args[0].c_str(), error.c_str());
    if (exit_code != 0) {
        return exit_code;
    }
    platform = node::MultiIsolatePlatform::Create(1);
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();
    int ret = uv_loop_init(&event_loop);
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

    config = std::make_unique<Config>(isolate, &event_loop, platform.get(), allocator.get());

    context = node::NewContext(isolate);
    if (context.IsEmpty()) {
        fprintf(stderr, "%s: Failed to initialize V8 Context\n", pt);
        return 1;
    }
    environment = std::make_unique<Environment>(*config, context, args, exec_args);
    auto load_env_ret = node::LoadEnvironment(
        environment->env.get(),
        "console.log(Math.random())"
    );
    if (load_env_ret.IsEmpty()) {
        return 1;
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

    exit_code = node::EmitExit(environment->env.get());
    node::Stop(environment->env.get());
    auto platform_finished = false;
    platform->AddIsolateFinishedCallback(isolate, [](void *data) {
        *static_cast<bool *>(data) = true;
    }, &platform_finished);
    platform->UnregisterIsolate(isolate);
    isolate->Dispose();
    while (!platform_finished)
        uv_run(&event_loop, UV_RUN_ONCE);
    int err = uv_loop_close(&event_loop);
    assert(err == 0);

    return exit_code;
}

int Instance::Run(const std::string scripts) {
    return 0;
}
}
