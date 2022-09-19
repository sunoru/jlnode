#include "arrays.h"

void arraybuffer_finalizer(Napi::Env env, void *data) {
    jlnode::util_functions.arraybuffer_finalizer(data);
}

napi_status create_external_arraybuffer(napi_env _env, void *data, size_t byte_length, napi_value *ret) {
    *ret = nullptr;
    WRAP_ERROR(
        auto env = Napi::Env(_env);
        auto ab = Napi::ArrayBuffer::New(env, data, byte_length, arraybuffer_finalizer);
        *ret = ab;
    );
    return napi_ok;
}
