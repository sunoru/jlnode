#include "functions.h"

Napi::Value call_function(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    GET_FUNC_POINTER(
        jlnode::call_function_func,
        "import NodeCall.call_function;NodeCall.call_function",
        env.Undefined()
    );

    auto argc = info.Length();
    auto args = new napi_value[argc];
    for (auto i = 0; i < argc; ++i) {
        args[i] = info[i];
    }
    jl_value_t *jl_args[4]{
        jl_box_voidpointer(info.Data()),
        jl_box_voidpointer(args),
        jl_box_uint64(argc),
        jl_box_voidpointer(info.This().operator napi_value())
    };
    auto result = jl_call(jlnode::call_function_func, jl_args, 4);
    delete[] args;

    return result == nullptr ? env.Undefined() : Napi::Value(env, (napi_value) jl_unbox_voidpointer(result));
}

napi_status create_function(napi_env _env, void *func, const char *utf8name, napi_value *result) {
    *result = nullptr;
    WRAP_ERROR(
        auto env = Napi::Env(_env);
        auto nv = Napi::Function::New(env, call_function, utf8name, func);
        *result = nv;
    );
    return napi_ok;
}

using Context = jlnode::ThreadSafeContext;
using DataType = Context::DataType;

napi_status create_thread_safe_function(
    napi_env _env,
    napi_value js_func,
    napi_value recv,
    const char *utf8name,
    Context **result_ctx
) {
    *result_ctx = nullptr;
    WRAP_ERROR(
        auto env = Napi::Env(_env);
        auto ctx = new Context(env);
        ctx->recv = Napi::Persistent(Napi::Value(env, recv));
        ctx->func = Context::TSFN::New(
            env, Napi::Function(env, js_func),
            utf8name, 0, 1,
            ctx,
            [](Napi::Env, void *, Context *_ctx) {
                delete _ctx;
            }
        );
        *result_ctx = ctx;
    );
    return napi_ok;
}

napi_status get_thread_safe_promise(napi_env _env, Context *ctx, napi_value *result) {
    *result = nullptr;
    WRAP_ERROR(
        auto env = Napi::Env(_env);
        *result = ctx->deferred.Promise();
    );
    return napi_ok;
}

napi_status call_thread_safe_function(
    napi_env _env, Context *ctx, DataType args, napi_ref *result
) {
    *result = nullptr;
    WRAP_ERROR(
        auto env = Napi::Env(_env);
        jlnode::reference((jl_value_t *) args);
        fprintf(stderr, "1\n");
        auto js_args = new DataType(args);
        fprintf(stderr, "2\n");
        auto status = ctx->func.BlockingCall(js_args);
        fprintf(stderr, "3\n");
        if (status != napi_ok) {
            return status;
        }
        status = ctx->func.Release();
        fprintf(stderr, "4\n");
        if (status != napi_ok) {
            return status;
        }
        NAPI_THROW_IF_FAILED_VOID(env, status);
        *result = ctx->result;
    );
    return napi_ok;
}


void jlnode::call_thread_safe_function(
    Napi::Env env, Napi::Function callback,
    ThreadSafeContext *ctx, DataType *js_args
) {
    if (env != nullptr) {
        if (callback != nullptr) {
            auto args = *js_args;
            auto result = callback.Call(ctx->recv.Value(), args->length, (napi_value *) args->data);
            ctx->result = Napi::Persistent(result);
        }
    } else {
        auto args = *js_args;
        delete js_args;
        jlnode::dereference(args->data);
    }
}