#include "external.h"

void external_finalize(Napi::Env env, void *data) {
    GET_FUNC_POINTER(
        jlnode::external_finalize_func,
        "import NodeCall.external_finalize;NodeCall.external_finalize",
    );
    auto ptr = jl_box_voidpointer(data);
    jl_call1(jlnode::external_finalize_func, ptr);
}

napi_status create_external(napi_env _env, void *data, napi_value *result) {
    WRAP_ERROR(
        Napi::Env env(_env);
        auto external = Napi::External<void>::New(env, data, external_finalize);
        *result = external;
        return napi_ok;
    );
}
