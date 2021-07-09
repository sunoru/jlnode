#include "objects.h"

Napi::Value dict_getter(const Napi::CallbackInfo &info) {
    fprintf(stderr, "argv[0] = %s\n", info[0].ToString().Utf8Value().c_str());
    fprintf(stderr, "argv[1] = %s\n", info[1].ToString().Utf8Value().c_str());
    return Napi::Env(info.Env()).Undefined();
}

void dict_setter(const Napi::CallbackInfo &info) {
    fprintf(stderr, "argv[0] = %s\n", info[0].ToString().Utf8Value().c_str());
}

napi_value create_object_dict(napi_env env, jl_value_t *dict) {
    auto nv = Napi::Object::New(env);
    std::vector<Napi::PropertyDescriptor> ps;
    auto iterate = jl_get_function(jl_base_module, "iterate");
    auto next = jl_call1(iterate, dict);
    while (!jl_is_nothing(next)) {
        auto kv = jl_get_nth_field(next, 0);
        ps.push_back(Napi::PropertyDescriptor::Accessor(
            jlnode::to_napi_value(jl_get_nth_field(kv, 1)),
            dict_getter,
            dict_setter,
            napi_default_jsproperty,
            dict
        ));
        next = jl_call2(iterate, dict, jl_get_nth_field(next, 1));
    }
    nv.DefineProperties(ps);
    return nv;
}

napi_value create_object_mutable(napi_env env, jl_value_t *v) {
    return nullptr;
}
