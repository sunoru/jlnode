#include <cassert>

#include <napi.h>
#include "init.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    napi_status status;
    exports.Set(
        Napi::String::New(env, "initialize"),
        Napi::Function::New(env, jlnode_addon::Initialize)
    );
    return exports;
}


NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
