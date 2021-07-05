#include "memory.h"

napi_handle_scope open_handle_scope(napi_env env) {
    napi_handle_scope handle_scope;
    auto status = napi_open_handle_scope(env, &handle_scope);
    NAPI_THROW_IF_FAILED_VOID(env, status);
    set_global_handle_scope(handle_scope);
    return handle_scope;
}

void close_handle_scope(napi_env env, napi_handle_scope scope) {
    auto status = napi_close_handle_scope(env, scope);
    NAPI_THROW_IF_FAILED_VOID(env, status);
    set_global_handle_scope(nullptr);
}

napi_value dereference(JlnodeResult *_result, napi_env env, napi_ref ref) {
    WRAP_ERROR(
        napi_value value;
        auto status = napi_get_reference_value(env, ref, &value);
        NAPI_THROW_IF_FAILED(env, status);
        return value;
    )
    return nullptr;
}

napi_ref make_reference(JlnodeResult *_result, napi_env env, napi_value value) {
    WRAP_ERROR(
        napi_ref ref;
        auto status = napi_create_reference(env, value, 1, &ref);
        NAPI_THROW_IF_FAILED(env, status);
        return ref;
    )
    return nullptr;
}

void free_reference(JlnodeResult *_result, napi_env env, napi_ref ref) {
    WRAP_ERROR(
        auto status = napi_delete_reference(env, ref);
        NAPI_THROW_IF_FAILED(env, status);
    )
}
