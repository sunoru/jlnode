#include "threading.h"

struct Context {
    void *func;
    void *channel;
};

void call_js_callback(
    napi_env env,
    napi_value js_callback,
    void *context,
    void *data
) {
    auto ctx = (Context *) context;
    if (env != nullptr) {
        auto func = jlnode::get_reference(ctx->func);
        auto channel = jlnode::get_reference(ctx->channel);
        auto result = jl_call((jl_function_t *) func, nullptr, 0);
        JL_GC_PUSH1(&result);
//        auto err = jl_exception_occurred();
//        if (err != nullptr) {
//            jl_call2(jlnode::push_func, channel, err);
//        } else {
            jl_call2(jlnode::push_func, channel, result);
//        }
        JL_GC_POP();
        fprintf(stderr, "call_js_callback\n");
    } else {
        delete ctx;
        fprintf(stderr, "call_js_callback end\n");
    }
}

napi_status create_threadsafe_function(napi_env _env, void *func, void *channel) {
    WRAP_ERROR(
        auto env = Napi::Env(_env);
        auto name = Napi::String::New(env, "tsfn");
        auto ctx = new Context{func, channel};
        napi_threadsafe_function tsfn;
        auto status = napi_create_threadsafe_function(
            env, nullptr,
            nullptr, name,
            0, 1, channel,
            nullptr, ctx,
            call_js_callback, &tsfn
        );
        NAPI_THROW_IF_FAILED_VOID(env, status);
        status = napi_call_threadsafe_function(tsfn, nullptr, napi_tsfn_blocking);
        status = napi_release_threadsafe_function(tsfn, napi_tsfn_release);
    );
    return napi_ok;
}
