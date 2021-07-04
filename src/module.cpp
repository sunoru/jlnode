#include "jlcxx/jlcxx.hpp"

#include "instance.h"

JLCXX_MODULE define_julia_module(jlcxx::Module &mod) {
    mod.method("test", []() { return 0; });
    mod.add_type<jlnode::Instance>("NodeJsInstance")
        .constructor()
        .method("initialize", &jlnode::Instance::Initialize)
        .method("dispose", &jlnode::Instance::Dispose);
}
