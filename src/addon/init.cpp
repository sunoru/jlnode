#include "init.h"

namespace jlnode_addon {

Napi::BigInt Initialize(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    auto env_napi = env.operator napi_env();
    auto env_ptr = reinterpret_cast<uint64_t>(env_napi);
    return Napi::BigInt::New(env_napi, env_ptr);
}

}