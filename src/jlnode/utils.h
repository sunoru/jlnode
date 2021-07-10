#ifndef JLNODE_ADDON_UTILS_H
#define JLNODE_ADDON_UTILS_H

#include <napi.h>
#include "jlsyms.h"

namespace jlnode {

extern jl_module_t *nodecall_module;
extern jl_function_t *to_napi_value_func;
extern jl_function_t *to_jl_value_func;
extern jl_function_t *get_func;
extern jl_function_t *setindex_func;
extern jl_function_t *haskey_func;
extern jl_function_t *keys_func;

int initialize_utils(jl_module_t *module);

jl_value_t *to_jl_value(napi_value node_value);

napi_value to_napi_value(jl_value_t *jl_value);


}

#define WRAP_ERROR(...) do { \
    try {                    \
        __VA_ARGS__     \
    } catch (Napi::Error &e) { \
        e.ThrowAsJavaScriptException(); \
        return napi_pending_exception;  \
    } catch (std::exception &e) {       \
        auto napi_err = Napi::Error::New(env, e.what()); \
        napi_err.ThrowAsJavaScriptException(); \
        return napi_pending_exception;  \
    }                        \
} while (0)

#define NODE_CALLBACK(NAME, ARGC, DEFAULT_VALUE, ...) napi_value NAME( \
    napi_env _env, napi_callback_info info              \
) {                                                     \
    Napi::Env env(_env);                                \
    size_t argc = ARGC;                                 \
    napi_value argv[ARGC];                              \
    napi_value this_arg;                                \
    void *data;                                         \
    auto status = napi_get_cb_info(                     \
        env, info, &argc, argv, &this_arg, &data        \
    );                                                  \
    if (status != napi_ok) {                            \
        Napi::Error::New(env).ThrowAsJavaScriptException(); \
        return DEFAULT_VALUE;                           \
    }                                                   \
    __VA_ARGS__                                         \
}

#endif //JLNODE_ADDON_UTILS_H
