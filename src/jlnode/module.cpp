#include <string>
//#include <julia.h>

#include "instance.h"
//#include "objects.h"
//#include "utils.h"

jlnode::Instance *instance;

extern "C" {

int test() { return 20070128; }

int initialize(const char *addon_path, napi_env *env) {
    instance = new jlnode::Instance;
    return instance->Initialize(addon_path, env);
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