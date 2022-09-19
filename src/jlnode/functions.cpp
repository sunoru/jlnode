#include "functions.h"

Napi::Value call_function(const Napi::CallbackInfo &info) {
    auto env = info.Env();

    auto argc = info.Length();
    auto args = new napi_value[argc];
    for (auto i = 0; i < argc; ++i) {
        args[i] = info[i];
    }
    auto this_nv = info.This().operator napi_value();
    auto result = jlnode::util_functions.call_function(
        info.Data(),
        args,
        argc,
        this_nv
    );
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