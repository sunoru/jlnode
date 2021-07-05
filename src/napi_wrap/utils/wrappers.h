#ifndef JLNODE_NAPI_WRAP_UTILS_WRAPPERS_H
#define JLNODE_NAPI_WRAP_UTILS_WRAPPERS_H

#include <napi.h>
#include "strings.h"
#include "../errors.h"

#define ESC(...) __VA_ARGS__

#define WRAP_ERROR(...) if (err->message != nullptr) { \
        free_string(err->message);                     \
        err->message = nullptr;                        \
    }                           \
    *err = JlnodeResult::Ok();   \
    try {               \
        __VA_ARGS__       \
    } catch (Napi::Error &e) {       \
        *err = JlnodeResult::From(e); \
    } catch (std::exception &e) {     \
        *err = JlnodeResult::From(e); \
    }

#define WRAP_NAPI_FUNCTION_VOID(NAME, VALUE_TYPE, FUNCTION, ARGS, ...) \
    void NAME(                                                         \
        napi_env env, napi_value value,                                \
        JlnodeResult *err, ## __VA_ARGS__) {                           \
        WRAP_ERROR(                                                    \
            Napi::VALUE_TYPE v(env, value);                            \
            v.FUNCTION(ARGS);                                          \
        )                                                              \
    }

#define WRAP_NAPI_FUNCTION(NAME, RETURN_TYPE, VALUE_TYPE, FUNCTION, ARGS, ...) \
    RETURN_TYPE NAME(                                                          \
        napi_env env, napi_value value,                                        \
        JlnodeResult *err, ## __VA_ARGS__) {                                   \
        WRAP_ERROR(                                                            \
            Napi::VALUE_TYPE v(env, value);                                    \
            return v.FUNCTION(ARGS);                                           \
        )                                                                      \
        return RETURN_TYPE();                                                  \
    }

#endif //JLNODE_NAPI_WRAP_UTILS_WRAPPERS_H
