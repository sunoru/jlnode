#ifndef JLNODE_NAPI_WRAP_TYPES_H
#define JLNODE_NAPI_WRAP_TYPES_H

#define WRAP_NAPI_CONVERSION(NAME, VALUE_TYPE, FUNCTION) \
    WRAP_NAPI_FUNCTION(NAME, napi_value, VALUE_TYPE, FUNCTION)

#define WRAP_NAPI_CONSTRUCTION(NAME, VALUE_TYPE, FROM_TYPE) \
    WRAP_NAPI_FUNCTION_STATIC(                              \
        NAME, napi_value,                                   \
        VALUE_TYPE::New(_env, _value),                      \
        FROM_TYPE _value                 \
    )

#include <cstdint>
#include <napi.h>
#include "utils/wrappers.h"
#include "utils/strings.h"
#include "env.h"


extern "C" {

// Value

// Value converters
WRAP_NAPI_CONVERSION(value_to_boolean, Value, ToBoolean())
WRAP_NAPI_CONVERSION(value_to_number, Value, ToNumber())
WRAP_NAPI_CONVERSION(value_to_string, Value, ToString())
WRAP_NAPI_CONVERSION(value_to_object, Value, ToObject())

// Value constructors: value_from_*
WRAP_NAPI_CONSTRUCTION(value_from_bool, Boolean, bool)
WRAP_NAPI_CONSTRUCTION(value_from_int32, Number, int32_t)
WRAP_NAPI_CONSTRUCTION(value_from_uint32, Number, uint32_t)
WRAP_NAPI_CONSTRUCTION(value_from_int64, Number, int64_t)
WRAP_NAPI_CONSTRUCTION(value_from_float, Number, float)
WRAP_NAPI_CONSTRUCTION(value_from_double, Number, double)
WRAP_NAPI_CONSTRUCTION(value_from_str, String, const char *)
WRAP_NAPI_CONSTRUCTION(value_from_external, External < void >, void *)
napi_value value_from_value(napi_env env, napi_value value, JlnodeResult *err) {
    WRAP_ERROR(
        return Napi::Value::From(env, value);
    )
    return nullptr;
}

// Value type checkers
WRAP_NAPI_FUNCTION(value_type, napi_valuetype, Value, Type())
WRAP_NAPI_FUNCTION(value_is_undefined, bool, Value, IsUndefined())
WRAP_NAPI_FUNCTION(value_is_null, bool, Value, IsNull())
WRAP_NAPI_FUNCTION(value_is_boolean, bool, Value, IsBoolean())
WRAP_NAPI_FUNCTION(value_is_number, bool, Value, IsNumber())
WRAP_NAPI_FUNCTION(value_is_bigint, bool, Value, IsBigInt())
WRAP_NAPI_FUNCTION(value_is_date, bool, Value, IsDate())
WRAP_NAPI_FUNCTION(value_is_string, bool, Value, IsString())
WRAP_NAPI_FUNCTION(value_is_symbol, bool, Value, IsSymbol())
WRAP_NAPI_FUNCTION(value_is_array, bool, Value, IsArray())
WRAP_NAPI_FUNCTION(value_is_array_buffer, bool, Value, IsArrayBuffer())
WRAP_NAPI_FUNCTION(value_is_typed_array, bool, Value, IsTypedArray())
WRAP_NAPI_FUNCTION(value_is_object, bool, Value, IsObject())
WRAP_NAPI_FUNCTION(value_is_function, bool, Value, IsFunction())
WRAP_NAPI_FUNCTION(value_is_promise, bool, Value, IsPromise())
WRAP_NAPI_FUNCTION(value_is_data_view, bool, Value, IsDataView())
WRAP_NAPI_FUNCTION(value_is_buffer, bool, Value, IsBuffer())
WRAP_NAPI_FUNCTION(value_is_external, bool, Value, IsExternal())

// Boolean
WRAP_NAPI_FUNCTION(boolean_to_bool, bool, Boolean, Value())

// Number
WRAP_NAPI_FUNCTION(number_to_int32, int32_t, Number, Int32Value())
WRAP_NAPI_FUNCTION(number_to_uint32, uint32_t, Number, Uint32Value())
WRAP_NAPI_FUNCTION(number_to_int64, int64_t, Number, Int64Value())
WRAP_NAPI_FUNCTION(number_to_float, float, Number, FloatValue())
WRAP_NAPI_FUNCTION(number_to_double, double, Number, DoubleValue())

// BigInt
WRAP_NAPI_FUNCTION(bigint_to_int64, int64_t, BigInt, Int64Value(lossless), bool *lossless)
WRAP_NAPI_FUNCTION(bigint_to_uint64, uint64_t, BigInt, Uint64Value(lossless), bool *lossless)
WRAP_NAPI_FUNCTION(bigint_get_word_count, size_t, BigInt, WordCount())
WRAP_NAPI_FUNCTION_VOID(
    bigint_to_words, BigInt, ToWords(sign_bit, word_count, words),
    int *sign_bit, size_t *word_count, uint64_t * words
)

// Date
WRAP_NAPI_FUNCTION(date_to_double, double, Date, ValueOf())

// Name (no methods)
// String
const char *string_to_utf8(napi_env env, napi_value value, JlnodeResult *err, size_t *len) {
    WRAP_ERROR(
        Napi::String v(env, value);
        auto s = v.Utf8Value();
        return copy_utf8(s, len);
    )
    return nullptr;
}

const char *string_to_utf16(napi_env env, napi_value value, JlnodeResult *err, size_t *len) {
    WRAP_ERROR(
        Napi::String v(env, value);
        auto s = v.Utf16Value();
        return copy_utf16(s, len);
    )
    return nullptr;
}

// Symbol
napi_value symbol_get_well_known(napi_env env, const char *name, JlnodeResult *err) {
    WRAP_ERROR(return Napi::Symbol::WellKnown(env, name);)
    return nullptr;
}

// Object
WRAP_NAPI_FUNCTION(object_get_property_value, napi_value, Object, Get(key), napi_value key)
WRAP_NAPI_FUNCTION(object_get_property_str, napi_value, Object, Get(key), const char *key)
WRAP_NAPI_FUNCTION(object_get_property_uint32, napi_value, Object, Get(key), int32_t key)
WRAP_NAPI_FUNCTION(object_set_property_value, bool, Object, Set(key, value), napi_value key, napi_value value)
WRAP_NAPI_FUNCTION(object_set_property_str, bool, Object, Set(key, value), const char *key, napi_value value)
WRAP_NAPI_FUNCTION(object_has_property_value, bool, Object, Has(key), napi_value key)
WRAP_NAPI_FUNCTION(object_has_property_str, bool, Object, Has(key), const char *key)
WRAP_NAPI_FUNCTION(object_has_property_uint32, bool, Object, Has(key), int32_t key)
WRAP_NAPI_FUNCTION(object_has_own_property_value, bool, Object, HasOwnProperty(key), napi_value key)
WRAP_NAPI_FUNCTION(object_has_own_property_str, bool, Object, HasOwnProperty(key), const char *key)
WRAP_NAPI_FUNCTION(object_delete_property_value, bool, Object, Delete(key), napi_value key)
WRAP_NAPI_FUNCTION(object_delete_property_str, bool, Object, Delete(key), const char *key)
WRAP_NAPI_FUNCTION(object_delete_property_uint32, bool, Object, Delete(key), uint32_t key)
WRAP_NAPI_FUNCTION(object_get_property_names, napi_value, Object, GetPropertyNames())
// TODO: implement `defineProperty` and `defineProperties`
// WRAP_NAPI_FUNCTION(object_define_property, bool, Object, DefineProperty, property, )
WRAP_NAPI_FUNCTION(object_instance_of, bool, Object, InstanceOf(Napi::Function(_env, constructor)),
                   napi_value constructor)

// External
WRAP_NAPI_FUNCTION_PTR(external_get_data, void, External < void >, Data())

// Array
WRAP_NAPI_FUNCTION(array_get_length, uint32_t, Array, Length())

// ArrayBuffer
// TODO: implement Finalizer and Hint
napi_value array_buffer_from_external(napi_env env, size_t byte_length, JlnodeResult *err, void *external_data) {
    WRAP_ERROR(
        if (external_data == nullptr) {
            return Napi::ArrayBuffer::New(env, byte_length);
        }
        return Napi::ArrayBuffer::New(env, external_data, byte_length);
    )
    return nullptr;
}
WRAP_NAPI_FUNCTION_PTR(array_buffer_get_data, void, ArrayBuffer, Data())
WRAP_NAPI_FUNCTION(array_buffer_get_byte_length, size_t, ArrayBuffer, ByteLength())

// TypedArray
WRAP_NAPI_CONVERSION(typed_array_to_array_buffer, TypedArray, ArrayBuffer())
WRAP_NAPI_FUNCTION(typed_array_get_type, napi_typedarray_type, TypedArray, TypedArrayType())
WRAP_NAPI_FUNCTION(typed_array_element_size, uint8_t, TypedArray, ElementSize())
WRAP_NAPI_FUNCTION(typed_array_element_length, size_t, TypedArray, ElementLength())
WRAP_NAPI_FUNCTION(typed_array_byte_offset, size_t, TypedArray, ByteOffset())
WRAP_NAPI_FUNCTION(typed_array_byte_length, size_t, TypedArray, ByteLength())

// TypedArrayOf

#define WRAP_NAPI_TYPED_ARRAY_OF(TYPE_NAME, TYPE) \
    WRAP_NAPI_FUNCTION_STATIC(         \
        TYPE_NAME ## _array_from_length,\
        napi_value,                    \
        TypedArrayOf<TYPE>::New(_env, element_length, element_type), \
        size_t element_length, napi_typedarray_type element_type \
    )                                  \
                                       \
    WRAP_NAPI_FUNCTION_STATIC(         \
        TYPE_NAME ## _array_from_array_buffer,\
        napi_value,                    \
        TypedArrayOf<TYPE>::New(_env, element_length, Napi::ArrayBuffer(_env, buffer), buffer_offset, element_type), \
        size_t element_length, napi_typedarray_type element_type,\
        napi_value buffer, size_t buffer_offset\
    )                                          \
                                               \
    WRAP_NAPI_FUNCTION(                        \
        TYPE_NAME ## _array_get_index,         \
        TYPE, TypedArrayOf<TYPE>,              \
        operator[](index),                     \
        size_t index                           \
    )                                          \
                                               \
    WRAP_NAPI_FUNCTION_VOID(                   \
        TYPE_NAME ## _array_set_index,         \
        TypedArrayOf<TYPE>,                    \
        operator[](index) = value,             \
        size_t index, TYPE value               \
    )                                          \
                                               \
    WRAP_NAPI_FUNCTION_PTR(                    \
        TYPE_NAME ## _array_get_data,          \
        TYPE, TypedArrayOf<TYPE>,              \
        Data()                                 \
    )


WRAP_NAPI_TYPED_ARRAY_OF(int8, int8_t)
WRAP_NAPI_TYPED_ARRAY_OF(uint8, uint8_t)
WRAP_NAPI_TYPED_ARRAY_OF(int16, int16_t)
WRAP_NAPI_TYPED_ARRAY_OF(uint16, uint16_t)
WRAP_NAPI_TYPED_ARRAY_OF(int32, int32_t)
WRAP_NAPI_TYPED_ARRAY_OF(uint32, uint32_t)
WRAP_NAPI_TYPED_ARRAY_OF(float32, float)
WRAP_NAPI_TYPED_ARRAY_OF(float64, double)
WRAP_NAPI_TYPED_ARRAY_OF(bigint64, int64_t)
WRAP_NAPI_TYPED_ARRAY_OF(biguint64, uint64_t)

}

#endif //JLNODE_NAPI_WRAP_TYPES_H
