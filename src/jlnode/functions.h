#ifndef JLNODE_ADDON_FUNCTIONS_H
#define JLNODE_ADDON_FUNCTIONS_H

#include <napi.h>

#include "jlsyms.h"
#include "utils.h"

namespace jlnode {


struct ThreadSafeContext;

void call_thread_safe_function(
    Napi::Env env,
    Napi::Function callback,
    ThreadSafeContext *ctx,
    jl_array_t **js_args
);

struct ThreadSafeContext {
    using DataType = jl_array_t *;
    using TSFN = Napi::TypedThreadSafeFunction<ThreadSafeContext, DataType, call_thread_safe_function>;

    explicit ThreadSafeContext(Napi::Env env) : deferred(Napi::Promise::Deferred::New(env)) {}

    Napi::Promise::Deferred deferred;
    Napi::Reference<Napi::Value> recv;
    TSFN func = nullptr;
    Napi::Reference<Napi::Value> result;
};

}

extern "C" {

napi_status create_function(napi_env env, void *func, const char *utf8name, napi_value *result);

napi_status create_thread_safe_function(
    napi_env env, napi_value js_func, napi_value recv,
    const char *utf8name, jlnode::ThreadSafeContext **result_ctx
);
napi_status get_thread_safe_promise(napi_env env, jlnode::ThreadSafeContext *ctx, napi_value *result);

napi_status call_thread_safe_function(
    napi_env env, jlnode::ThreadSafeContext *ctx,
    jl_array_t *args, napi_ref *result
);

}

#endif //JLNODE_ADDON_FUNCTIONS_H
