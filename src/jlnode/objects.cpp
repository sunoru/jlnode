#include "objects.h"

Napi::Value dict_getter(const Napi::CallbackInfo &info) {
    fprintf(stderr, "Getter\n");
    fprintf(stderr, "argv[0] = %s\n", info[0].ToString().Utf8Value().c_str());
    fprintf(stderr, "argv[1] = %s\n", info[1].ToString().Utf8Value().c_str());
    return Napi::Env(info.Env()).Undefined();
}

void dict_setter(const Napi::CallbackInfo &info) {
    fprintf(stderr, "argv[0] = %s\n", info[0].ToString().Utf8Value().c_str());
}

napi_status create_object_dict(napi_env env, jl_value_t *dict, napi_value *ret) {
    WRAP_ERROR(
        auto nv = Napi::Object::New(env);
        std::vector<Napi::PropertyDescriptor> ps;
//        auto iterate = jl_get_function(jl_base_module, "iterate");
        auto iterate = jl_eval_string("iterate");
        auto next = jl_call1(iterate, dict);
        JL_GC_PUSH1(&next);
        while (next != jl_nothing) {
            auto kv = jl_get_nth_field(next, 0);
            auto key = jl_get_nth_field(kv, 0);
            ps.push_back(Napi::PropertyDescriptor::Accessor(
                jlnode::to_napi_value(key),
                dict_getter,
                dict_setter,
                napi_default_jsproperty,
                dict
            ));
            auto state = jl_get_nth_field(next, 1);
            JL_GC_POP();
            next = jl_call2(iterate, dict, state);
            JL_GC_PUSH1(&next);
        }
        JL_GC_POP();
        nv.DefineProperties(ps);
        *ret = nv;
    );
    return napi_ok;
}

napi_status create_object_mutable(napi_env env, jl_value_t *v, napi_value *ret) {
    *ret = nullptr;
    return napi_ok;
}
