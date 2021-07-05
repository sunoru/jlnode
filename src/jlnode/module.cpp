#include <string>

#include "../napi_wrap/utils/strings.h"
#include "../napi_wrap/memory.h"
#include "instance.h"

jlnode::Instance *instance;

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

int run(napi_env env, const char *scripts, void **result) {
    try {
        *result = (void *) run_script(env, scripts);
        return 0;
    } catch (Napi::Error &e) {
        size_t n;
        *result = (void *) copy_utf8(e.what(), &n);
        return 1;
    } catch (std::exception &e) {
        size_t n;
        *result = (void *) copy_utf8(e.what(), &n);
        return 2;
    }
}

}