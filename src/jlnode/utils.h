#ifndef JLNODE_ADDON_UTILS_H
#define JLNODE_ADDON_UTILS_H

#include <node.h>
#include <napi.h>
#include "jlsyms.h"

namespace jlnode {

extern jl_module_t *nodecall_module;
extern jl_function_t *to_napi_value_func;
extern jl_function_t *to_jl_value_func;
extern jl_function_t *propertynames_func;
extern jl_function_t *getproperty_func;
extern jl_function_t *setproperty_func;
extern jl_function_t *hasproperty_func;
extern jl_function_t *external_finalizer_func;
extern jl_function_t *object_finalizer_func;
extern jl_function_t *arraybuffer_finalizer_func;
extern jl_function_t *call_function_func;

int initialize_utils(jl_module_t *module);

jl_value_t *to_jl_value(napi_value node_value);

napi_value to_napi_value(jl_value_t *jl_value);

inline napi_value JsValueFromV8LocalValue(v8::Local<v8::Value> local) {
    return reinterpret_cast<napi_value>(*local);
}

inline v8::Local<v8::Value> V8LocalValueFromJsValue(napi_value v) {
    v8::Local<v8::Value> local;
    memcpy(static_cast<void *>(&local), &v, sizeof(v));
    return local;
}

inline void check_jl_exception(Napi::Env env) {
    auto err = jl_exception_occurred();
    if (err != nullptr) {
        throw Napi::Error::New(env, jl_typeof_str(err));
    }
}

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
    do {                                                \
        if ((NAME) == nullptr) {                        \
            (NAME) = jl_eval_string(FUNC_NAME);         \
            if (jl_exception_occurred() || (NAME) == jl_nothing) { \
                return FAILED_VALUE;                    \
            }                                           \
        }                                               \
    } while(0)

#endif //JLNODE_ADDON_UTILS_H
