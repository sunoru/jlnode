#include "externals.h"

void external_finalizer(Napi::Env env, void *data) {
    jlnode::util_functions.external_finalizer(data);
}

napi_status create_external(napi_env _env, void *data, napi_value *result) {
    WRAP_ERROR(
        Napi::Env env(_env);
        auto external = Napi::External<void>::New(env, data, external_finalizer);
        *result = external;
        return napi_ok;
    );
}
