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