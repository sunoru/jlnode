#ifndef JLNODE_NAPI_WRAP_UTILS_WRAPPERS_H
#define JLNODE_NAPI_WRAP_UTILS_WRAPPERS_H

#include <napi.h>
#include "strings.h"
#include "../napi_wrap/errors.h"

#define ESC(...) __VA_ARGS__

#define WRAP_ERROR(...) if (_result->message != nullptr) { \
        free_string(_result->message);                     \
        _result->message = nullptr;                        \
    }                           \
    *_result = JlnodeResult::Ok();   \
    try {               \
        __VA_ARGS__       \
    } catch (Napi::Error &e) {       \
        *_result = JlnodeResult::From(e); \
    } catch (std::exception &e) {     \
        *_result = JlnodeResult::From(e); \
    }

#define WRAP_NAPI_FUNCTION_VOID(NAME, VALUE_TYPE, FUNCTION, ...) \
    void NAME(                                                         \
        JlnodeResult *_result, napi_env _env,                          \
        napi_value _value,                              \
        ## __VA_ARGS__) {                           \
        WRAP_ERROR(                                                    \
            Napi::VALUE_TYPE _v(_env, _value);                         \
            _v.FUNCTION;                                         \
        )                                                              \
    }

#define WRAP_NAPI_FUNCTION(NAME, RETURN_TYPE, VALUE_TYPE, FUNCTION, ...) \
    RETURN_TYPE NAME(                                                          \
        JlnodeResult *_result, napi_env _env,                                  \
        napi_value _value, ## __VA_ARGS__) {                                   \
        WRAP_ERROR(                                                            \
            Napi::VALUE_TYPE _v(_env, _value);                                 \
            return _v.FUNCTION;                                          \
        )                                                                      \
        return RETURN_TYPE();                                                  \
    }

#define WRAP_NAPI_FUNCTION_PTR(NAME, RETURN_TYPE, VALUE_TYPE, FUNCTION, ...) \
    RETURN_TYPE *NAME(                                                         \
        JlnodeResult *_result, napi_env _env,   \
        napi_value _value, ## __VA_ARGS__) {                                   \
        WRAP_ERROR(                                                            \
            Napi::VALUE_TYPE _v(_env, _value);                                 \
            return _v.FUNCTION;                                          \
        )                                                                      \
        return nullptr;                                                  \
    }

#define WRAP_NAPI_FUNCTION_STATIC(NAME, RETURN_TYPE, FUNCTION, ...) \
    RETURN_TYPE NAME(JlnodeResult *_result, napi_env _env, ## __VA_ARGS__) {   \
        WRAP_ERROR(                                                            \
            return Napi::FUNCTION;                                          \
        )                                                                      \
        return RETURN_TYPE();                                                  \
    }
#endif //JLNODE_NAPI_WRAP_UTILS_WRAPPERS_H
