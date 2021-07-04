#ifndef JLNODE_INSTANCE_H
#define JLNODE_INSTANCE_H

#include <string>

#include <node.h>
#include <uv.h>

namespace jlnode {

struct Config {
public:
    v8::Locker locker;
    v8::Isolate::Scope isolate_scope;
    std::unique_ptr<node::IsolateData, decltype(&node::FreeIsolateData)> isolate_data;
    v8::HandleScope handle_scope;

    Config(
            v8::Isolate *isolate,
            uv_loop_t *loop,
            node::MultiIsolatePlatform *platform,
            node::ArrayBufferAllocator *allocator
    );
};

struct Environment {
public:
    v8::Context::Scope context_scope;
    std::unique_ptr<node::Environment, decltype(&node::FreeEnvironment)> env;

    Environment(
            Config &config,
            v8::Local<v8::Context> context,
            std::vector<std::string> &args,
            std::vector<std::string> &exec_args
    );
};

class Instance {
public:
    uv_loop_t event_loop;
    std::unique_ptr<node::MultiIsolatePlatform> platform;
    std::shared_ptr<node::ArrayBufferAllocator> allocator;
    v8::Isolate *isolate;
    Config *config;
    v8::Local<v8::Context> context;
    Environment *environment;

    Instance();

    int Initialize();
    int Dispose();

    int Run(std::string scripts);

private:
    bool initialized;
};
}

#endif //JLNODE_INSTANCE_H
