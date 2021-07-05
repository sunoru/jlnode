#include <string>

#include "../napi_wrap/utils/strings.h"
#include "../napi_wrap/memory.h"
#include "instance.h"

jlnode::Instance *instance;

extern "C" {

int test() { return 20070128; }

int initialize(const char *addon_path) {
    instance = new jlnode::Instance;
    return instance->Initialize(addon_path);
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