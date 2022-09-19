#ifndef JLNODE_INSTANCE_H
#define JLNODE_INSTANCE_H

#include <cstdlib>
#include <string>

#include <node.h>
#include <uv.h>
#include <node_api.h>

namespace jlnode {

struct EnvironmentConfig {
public:
    std::unique_ptr<node::CommonEnvironmentSetup> setup;

    EnvironmentConfig(
        node::MultiIsolatePlatform *platform,
        std::vector<std::string> *errors,
        const std::vector<std::string> &args,
        const std::vector<std::string> &exec_args
    );
};

struct IsolateConfig {
public:
    v8::Locker locker;
    v8::Isolate::Scope isolate_scope;
    v8::HandleScope handle_scope;

    IsolateConfig(
        v8::Isolate *isolate
    );
};

struct ContextConfig {
public:
    v8::Context::Scope context_scope;

    ContextConfig(
        v8::Local<v8::Context> context
    );
};

class Instance {
public:
    std::unique_ptr<node::MultiIsolatePlatform> platform;
    IsolateConfig *isolate_config = nullptr;
    ContextConfig *context_config = nullptr;
    EnvironmentConfig *environment_config = nullptr;

    Instance();

    int Initialize(const char *addon_path, napi_env *env, const char **args, size_t argc);

    int Dispose() const;

private:
    bool initialized = false;
};

}

extern jlnode::Instance *instance;

#endif //JLNODE_INSTANCE_H
