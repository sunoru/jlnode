#include "instance.h"
#include "threading.h"

using v8::V8;

std::string get_initialization_scripts(const char *addon_path) {
    return "globalThis.require = require('module').createRequire(process.cwd() + '/')\n"
           "globalThis.__jlnode_addon = globalThis.require('"
           + std::string(addon_path) +
           "')\n"
           "return globalThis.__jlnode_addon.initialize()";
}

namespace jlnode {

EnvironmentConfig::EnvironmentConfig(
    node::MultiIsolatePlatform *platform,
    std::vector<std::string> *errors,
    const std::vector<std::string> &args,
    const std::vector<std::string> &exec_args
) {
    setup = node::CommonEnvironmentSetup::Create(platform, errors, args, exec_args);
}

IsolateConfig::IsolateConfig(
    v8::Isolate *isolate
) : locker(isolate), isolate_scope(isolate), handle_scope(isolate) {}

ContextConfig::ContextConfig(
    v8::Local<v8::Context> context
) : context_scope(context) {}

Instance::Instance() = default;

int Instance::Initialize(const char *addon_path, napi_env *env, const char **_args, size_t argc) {
    char pt[256];
    uv_get_process_title(pt, 256);
    auto args = std::vector<std::string>({pt});
    args.reserve(argc + 1);
    for (auto i = 0; i < argc; ++i) {
        args.emplace_back(_args[i]);
    }
    auto result = node::InitializeOncePerProcess(args, {
        node::ProcessInitializationFlags::kNoInitializeV8,
        node::ProcessInitializationFlags::kNoInitializeNodeV8Platform
    });
    for (const auto &error : result->errors())
        fprintf(stderr, "%s: %s\n", args[0].c_str(), error.c_str());
    if (result->early_return()) {
        return result->exit_code();
    }
    auto thread_pool_size = std::getenv("UV_THREADPOOL_SIZE");
    platform = node::MultiIsolatePlatform::Create(thread_pool_size == nullptr ? 4 : atoi(thread_pool_size));
    V8::InitializePlatform(platform.get());
    V8::Initialize();

    auto errors = std::vector<std::string>();

    environment_config = new EnvironmentConfig(platform.get(), &errors, result->args(), result->exec_args());
    auto setup = environment_config->setup.get();
    if (!setup) {
        for (const auto &error : errors)
            fprintf(stderr, "%s: %s\n", args[0].c_str(), error.c_str());
        return 1;
    }
    auto isolate = setup->isolate();
    isolate_config = new IsolateConfig(isolate);
    auto environment = setup->env();
    auto context = setup->context();
    context_config = new ContextConfig(context);

    auto init_scripts = get_initialization_scripts(addon_path);
    auto load_env_ret = node::LoadEnvironment(
        environment,
        init_scripts.c_str()
    );
    if (load_env_ret.IsEmpty()) {
        return 1;
    }
    auto t = load_env_ret.ToLocalChecked();
    if (!t->IsBigInt()) {
        char buffer[256];
        t->ToDetailString(context).ToLocalChecked()->WriteUtf8(isolate, buffer);
        fprintf(stderr, "%s: %s should be an uint64_t\n", args[0].c_str(), buffer);
        return 1;
    }
    *env = (napi_env) t->ToBigInt(context).ToLocalChecked()->Uint64Value();
    auto ret = jlnode::initialize_threading(*env);
    if (ret != 0) {
        return ret;
    }

    initialized = true;
    return 0;
}

int Instance::Dispose() const {
    if (!initialized) {
        return 0;
    }
    auto setup = environment_config->setup.get();
    auto environment = setup->env();
    jlnode::dispose_threading();
    auto exit_code = node::SpinEventLoop(environment).FromMaybe(1);
    node::Stop(environment);
    delete context_config;
    delete isolate_config;
    delete environment_config;
    V8::Dispose();
    V8::ShutdownPlatform();
    node::TearDownOncePerProcess();
    return exit_code;
}

}
