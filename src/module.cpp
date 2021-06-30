#include "jlcxx/jlcxx.hpp"

#include "NodeJSInstance.h"

JLCXX_MODULE define_julia_module(jlcxx::Module &mod) {
    mod.method("test", []() { return 0; });
    mod.add_type<NodeJSInstance>("NodeJSInstance")
        .constructor<void *>()
        .method("initialize", &NodeJSInstance::Initialize);
}