#ifndef JLNODE_UTILS_H
#define JLNODE_UTILS_H

#include <node.h>
#include <napi.h>

#ifdef _WIN32
#    define LIBRARY_API __declspec(dllexport)
#else
#    define LIBRARY_API
#endif

extern "C" {

typedef struct {
    void (*jl_yield)();
    napi_value (*propertynames)(void *);
    napi_value (*getproperty)(void *, void *);
    napi_value (*setproperty)(void *, void *, void *);
    napi_value (*hasproperty)(void *, void *);
    void (*external_finalizer)(void *);
    void (*object_finalizer)(void *, void *);
    void (*arraybuffer_finalizer)(void *);
    napi_value (*call_function)(void *, void *, size_t, void *);
    void (*call_threadsafe)(void *);
} jl_functions;

}

namespace jlnode {

extern jl_functions util_functions;

int initialize_utils(jl_functions functions);

inline napi_value JsValueFromV8LocalValue(v8::Local<v8::Value> local) {
    return reinterpret_cast<napi_value>(*local);
}

inline v8::Local<v8::Value> V8LocalValueFromJsValue(napi_value v) {
    v8::Local<v8::Value> local;
    memcpy(static_cast<void *>(&local), &v, sizeof(v));
    return local;
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

#endif //JLNODE_UTILS_H
