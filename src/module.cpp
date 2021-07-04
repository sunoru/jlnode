#include <string>

#include "instance.h"

jlnode::Instance *instance;

//JLCXX_MODULE define_julia_module(jlcxx::Module &mod) {
//    mod.method("test", []() { return 0; });
//    mod.add_type<jlnode::Instance>("NodeJsInstance")
//        .constructor()
//        .method("initialize", &jlnode::Instance::Initialize)
//        .method("dispose", &jlnode::Instance::Dispose);
//}

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

int run(const char *scripts, void **result) {
    try {
        *result = (void *) instance->Run(scripts).operator napi_value();
        return 0;
    } catch (Napi::Error &e) {
        char *error = new char[strlen(e.what()) + 1];
        strcpy(error, e.what());
        *result = (void *) error;
        return 1;
    } catch (std::exception &e) {
        char *error = new char[strlen(e.what()) + 1];
        strcpy(error, e.what());
        *result = (void *) error;
        return 2;
    }
}

}