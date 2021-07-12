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
extern jl_function_t *propertynames_func;
extern jl_function_t *getproperty_func;
extern jl_function_t *setproperty_func;

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
        auto napi_err = Napi::Error::New(_env, e.what()); \
        napi_err.ThrowAsJavaScriptException(); \
        return napi_pending_exception;  \
    }                        \
} while (0)

#define GET_FUNC_POINTER(NAME, FUNC_NAME, FAILED_VALUE) \
    do {                                  \
        (NAME) = jl_eval_string(FUNC_NAME);  \
        if ((NAME) == jl_nothing) {         \
            return FAILED_VALUE;                     \
        }                                 \
    } while(0)

#endif //JLNODE_ADDON_UTILS_H
