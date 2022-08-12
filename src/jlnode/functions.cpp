#include "functions.h"

Napi::Value call_function(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    GET_FUNC_POINTER(
        jlnode::call_function_func,
        "call_function",
        env.Undefined()
    );

    auto argc = info.Length();
    auto args = new napi_value[argc];
    for (auto i = 0; i < argc; ++i) {
        args[i] = info[i];
    }
    auto this_nv = info.This().operator napi_value();
    auto result = (napi_value) nullptr;
    jl_value_t *jl_args[5] {
        jl_box_voidpointer(info.Data()),
        jl_box_voidpointer(args),
        jl_box_uint64(argc),
        jl_box_voidpointer(this_nv),
        jl_box_voidpointer(&result)
    };
    auto _result = jl_call(jlnode::call_function_func, jl_args, 5);
    delete[] args;

    return result == nullptr ? env.Undefined() : Napi::Value(env, result);
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