#include "instance.h"

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
    std::vector<std::string> args{std::string(pt)};
    std::vector<std::string> exec_args;
    std::vector<std::string> errors;
    args.reserve(argc + 1);
    for (auto i = 0; i < argc; ++i) {
        args.emplace_back(_args[i]);
    }
    auto exit_code = node::InitializeNodeWithArgs(&args, &exec_args, &errors);
    for (const std::string &error : errors)
        fprintf(stderr, "%s: %s\n", args[0].c_str(), error.c_str());
    if (exit_code != 0) {
        return exit_code;
    }
    platform = node::MultiIsolatePlatform::Create(1);
    V8::InitializePlatform(platform.get());
    V8::Initialize();

    environment_config = new EnvironmentConfig(platform.get(), &errors, args, exec_args);
    auto setup = environment_config->setup.get();
    if (!setup) {
        for (const std::string &error : errors)
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
        fprintf(stderr, "%s: %s should be an uint64_t\n", pt, buffer);
        return 1;
    }
    *env = (napi_env) t->ToBigInt(context).ToLocalChecked()->Uint64Value();

    initialized = true;
    return 0;
}

int Instance::Dispose() const {
    if (!initialized) {
        return 0;
    }
    auto setup = environment_config->setup.get();
    auto environment = setup->env();
    auto exit_code = node::SpinEventLoop(environment).FromMaybe(1);
    node::Stop(environment);
    delete context_config;
    delete isolate_config;
    delete environment_config;
    V8::Dispose();
    V8::ShutdownPlatform();
    return exit_code;
}

}
