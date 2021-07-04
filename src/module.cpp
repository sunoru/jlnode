#include <string>

#include "instance.h"

//JLCXX_MODULE define_julia_module(jlcxx::Module &mod) {
//    mod.method("test", []() { return 0; });
//    mod.add_type<jlnode::Instance>("NodeJsInstance")
//        .constructor()
//        .method("initialize", &jlnode::Instance::Initialize)
//        .method("dispose", &jlnode::Instance::Dispose);
//}

static jlnode::Instance *instance;

extern "C" {

int test() { return 20070128; }

int initialize() {
    instance = new jlnode::Instance;
    return instance->Initialize();
}

int dispose() {
    if (instance == nullptr) {
        return 0;
    }
    auto ret = instance->Dispose();
    delete instance;
    return ret;
}

int run(const char *scripts) {
    auto result = instance->Run(scripts);
    fprintf(stderr, "%s\n", result.ToString().Utf8Value().c_str());
    return 0;
}

}