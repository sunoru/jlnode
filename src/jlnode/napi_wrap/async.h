#ifndef JLNODE_ADDON_ASYNC_H
#define JLNODE_ADDON_ASYNC_H

#include <napi.h>
#include "../utils/wrappers.h"

extern "C" {

napi_async_work create_async_work(
    JlnodeResult *_result, napi_env _env,
    napi_value resource, const char *resource_name,
    napi_async_execute_callback execute,
    napi_async_complete_callback complete,
    void *data
) {
    WRAP_ERROR(
        napi_async_work work;
        auto status = napi_create_async_work(
            _env,
            resource == nullptr ? Napi::Object::New(_env) : resource,
            Napi::String::New(_env, resource_name),
            execute, complete, data, &work
        );
        return work;
    )
    return nullptr;
}

void delete_async_work(JlnodeResult *_result, napi_env _env, napi_async_work work) {
    WRAP_ERROR(
        auto status = napi_delete_async_work(_env, work);
        NAPI_THROW_IF_FAILED_VOID(_env, status);
    )
}

void queue_async_work(JlnodeResult *_result, napi_env _env, napi_async_work work) {
    WRAP_ERROR(
        auto status = napi_queue_async_work(_env, work);
        NAPI_THROW_IF_FAILED_VOID(_env, status);
    )
}

void cancel_async_work(JlnodeResult *_result, napi_env _env, napi_async_work work) {
    WRAP_ERROR(
        auto status = napi_cancel_async_work(_env, work);
        NAPI_THROW_IF_FAILED_VOID(_env, status);
    )
}

napi_async_context async_init(
    JlnodeResult *_result, napi_env _env,
    napi_value resource, const char *resource_name
) {
    WRAP_ERROR(
        napi_async_context context;
        auto status = napi_async_init(
            _env,
            resource == nullptr ? Napi::Object::New(_env) : resource,
            Napi::String::New(_env, resource_name),
            &context
        );
        NAPI_THROW_IF_FAILED_VOID(_env, status);
        return context;
    )
    return nullptr;
}

void async_destroy(
    JlnodeResult *_result, napi_env _env,
    napi_async_context context
) {
    WRAP_ERROR(
        auto status = napi_async_destroy(_env, context);
        NAPI_THROW_IF_FAILED_VOID(_env, status);
    )
}

napi_callback_scope open_callback_scope(
    JlnodeResult *_result, napi_env _env,
    napi_value resource,
    napi_async_context context
) {
    WRAP_ERROR(
        napi_callback_scope scope;
        auto status = napi_open_callback_scope(
            _env,
            resource == nullptr ? Napi::Object::New(_env) : resource,
            context,
            &scope
        );
        NAPI_THROW_IF_FAILED_VOID(_env, status);
        return scope;
    )
    return nullptr;
}

void close_callback_scope(
    JlnodeResult *_result, napi_env _env,
    napi_callback_scope scope
) {
    WRAP_ERROR(
        auto status = napi_close_callback_scope(_env, scope);
        NAPI_THROW_IF_FAILED_VOID(_env, status);
    )
}

}

#endif //JLNODE_ADDON_ASYNC_H
