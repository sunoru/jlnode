#include "arrays.h"

void arraybuffer_finalizer(Napi::Env env, void *data) {
    GET_FUNC_POINTER(
        jlnode::arraybuffer_finalizer_func,
        "import NodeCall.arraybuffer_finalizer;NodeCall.arraybuffer_finalizer",
    );
    auto ptr = jl_box_voidpointer(data);
    jl_call1(jlnode::arraybuffer_finalizer_func, ptr);
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
