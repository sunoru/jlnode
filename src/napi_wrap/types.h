#ifndef JLNODE_NAPI_WRAP_TYPES_H
#define JLNODE_NAPI_WRAP_TYPES_H

#define ESC(...) __VA_ARGS__

#define WRAP_NAPI_FUNCTION_VOID(NAME, VALUE_TYPE, FUNCTION, ARGS, ...) \
    void NAME(napi_env env, napi_value value, ## __VA_ARGS__) {         \
        Napi::VALUE_TYPE v(env, value);                                 \
        v.FUNCTION(ARGS);                                               \
    }
#define WRAP_NAPI_FUNCTION(NAME, RETURN_TYPE, VALUE_TYPE, FUNCTION, ARGS, ...) \
    RETURN_TYPE NAME(napi_env env, napi_value value, ## __VA_ARGS__) {          \
        Napi::VALUE_TYPE v(env, value);                                         \
        return v.FUNCTION(ARGS);                                                \
    }

#define WRAP_NAPI_CONVERSION(NAME, FUNCTION) \
    napi_value NAME(napi_env env, napi_value value) { \
        Napi::Value v(env, value);                    \
        return v.FUNCTION();                          \
    }

#include <cstdint>
#include <napi.h>
#include "utils/strings.h"
#include "env.h"


extern "C" {

// Value
WRAP_NAPI_FUNCTION(value_type, napi_valuetype, Value, Type,)
WRAP_NAPI_CONVERSION(value_to_boolean, ToBoolean)
WRAP_NAPI_CONVERSION(value_to_number, ToNumber)
WRAP_NAPI_CONVERSION(value_to_string, ToString)
WRAP_NAPI_CONVERSION(value_to_object, ToObject)

// Boolean
WRAP_NAPI_FUNCTION(boolean_to_bool, bool, Boolean, Value,)

// Number
WRAP_NAPI_FUNCTION(number_to_int32, int32_t, Number, Int32Value,)
WRAP_NAPI_FUNCTION(number_to_uint32, uint32_t, Number, Uint32Value,)
WRAP_NAPI_FUNCTION(number_to_int64, int64_t, Number, Int64Value,)
WRAP_NAPI_FUNCTION(number_to_float, float, Number, FloatValue,)
WRAP_NAPI_FUNCTION(number_to_double, double, Number, DoubleValue,)

// BigInt
WRAP_NAPI_FUNCTION(bigint_to_int64, int64_t, BigInt, Int64Value, lossless, bool *lossless)
WRAP_NAPI_FUNCTION(bigint_to_uint64, uint64_t, BigInt, Uint64Value, lossless, bool *lossless)
WRAP_NAPI_FUNCTION(bigint_get_word_count, size_t, BigInt, WordCount,)
WRAP_NAPI_FUNCTION_VOID(
    bigint_to_words, BigInt, ToWords,
    ESC(sign_bit, word_count, words),
    int *sign_bit, size_t *word_count, uint64_t * words
)

// Date
WRAP_NAPI_FUNCTION(date_to_double, double, Date, ValueOf,)

// Name (no methods)
// String
const char *string_to_utf8(napi_env env, napi_value value, size_t *len) {
    Napi::String v(env, value);
    auto s = v.Utf8Value();
    return copy_utf8(s, len);
}

const char *string_to_utf16(napi_env env, napi_value value, size_t *len) {
    Napi::String v(env, value);
    auto s = v.Utf16Value();
    return copy_utf16(s, len);
}

}

#endif //JLNODE_NAPI_WRAP_TYPES_H
