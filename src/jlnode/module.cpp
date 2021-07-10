#include <string>

#include "instance.h"
#include "objects.h"
#include "jlsyms.h"

jlnode::Instance *instance;

extern "C" {

int test() { return 20070128; }

int initialize(jl_module_t *module, const char *addon_path, napi_env *env) {
    instance = new jlnode::Instance;
    auto ret = instance->Initialize(addon_path, env);
    if (ret != 0) { return ret; }
    return jlnode::initialize_utils(module);
}

int dispose() {
    if (instance == nullptr) {
        return 0;
    }
    auto ret = instance->Dispose();
    delete instance;
    return ret;
}

}