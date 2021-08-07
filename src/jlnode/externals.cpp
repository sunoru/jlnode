#include "externals.h"

void external_finalizer(Napi::Env env, void *data) {
    GET_FUNC_POINTER(
        jlnode::external_finalizer_func,
        "import NodeCall.external_finalizer;NodeCall.external_finalizer",
    );
    auto ptr = jl_box_voidpointer(data);
    jl_call1(jlnode::external_finalizer_func, ptr);
}

napi_status create_external(napi_env _env, void *data, napi_value *result) {
    WRAP_ERROR(
        Napi::Env env(_env);
        auto external = Napi::External<void>::New(env, data, external_finalizer);
        *result = external;
        return napi_ok;
    );
}
