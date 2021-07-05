#include <string>

#include "../napi_wrap/utils/strings.h"
#include "../napi_wrap/memory.h"
#include "../napi_wrap/types.h"
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

int run(const char *scripts, void **result) {
    try {
        *result = (void *) run_script(scripts);
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