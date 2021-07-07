#include "env.h"

static Napi::Env *global_env = nullptr;
static napi_handle_scope global_handle_scope = nullptr;

void set_global_env(const void *p) {
    if (p == nullptr) {
        delete global_env;
    } else if (global_env == nullptr) {
        global_env = new Napi::Env((napi_env) p);
    } else {
        *global_env = (napi_env) p;
    }
}

napi_env get_global_env() {
    return global_env == nullptr ? nullptr : *global_env;
}

void set_global_handle_scope(const void *p) {
    global_handle_scope = (napi_handle_scope) p;
}

napi_handle_scope get_global_handle_scope() {
    return global_handle_scope;
}

napi_value run_script(JlnodeResult *_result, napi_env env, const char *scripts) {
    WRAP_ERROR(
        Napi::Env _env(env);
        return _env.RunScript(scripts);
    )
    return nullptr;
}

napi_value env_get_global(JlnodeResult *_result, napi_env env) {
    WRAP_ERROR(
        Napi::Env _env(env);
        return _env.Global();
    )
    return nullptr;
}

napi_value env_get_undefined(JlnodeResult *_result, napi_env env) {
    WRAP_ERROR(
        Napi::Env _env(env);
        return _env.Undefined();
    )
    return nullptr;
}

napi_value env_get_null(JlnodeResult *_result, napi_env env) {
    WRAP_ERROR(
        Napi::Env _env(env);
        return _env.Null();
    )
    return nullptr;
}

bool env_is_exception_pending(JlnodeResult *_result, napi_env env) {
    bool ret;
    auto status = napi_is_exception_pending(env, &ret);
    ret = status == napi_ok;
    *_result = ret ? JlnodeResult::Ok() : JlnodeResult{status, nullptr};
    return ret;
}

const napi_node_version *get_node_version(JlnodeResult *_result, napi_env _env) {
    WRAP_ERROR(return Napi::VersionManagement::GetNodeVersion(_env);)
    return nullptr;
}

uint32_t get_napi_version(JlnodeResult *_result, napi_env _env) {
    WRAP_ERROR(return Napi::VersionManagement::GetNapiVersion(_env);)
    return uint32_t();
}

void env_get_and_clear_pending_exception(JlnodeResult *_result, napi_env env) {
    WRAP_ERROR(
        Napi::Env _env(env);
        auto err = _env.GetAndClearPendingException();
        if (!err.Value().IsUndefined()) {
            throw Napi::Error(err);
        }
    )
}
