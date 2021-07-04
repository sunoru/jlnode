#include "conversions.h"

const char *to_string(napi_value value) {
    Napi::Value v(*instance->env_napi, value);
    auto s = v.ToString().Utf8Value();
    return copy_c_str(s);
}

int get_type(napi_value value) {
    Napi::Value v(*instance->env_napi, value);
    return static_cast<int>(v.Type());
}
