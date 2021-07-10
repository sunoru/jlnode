#include "objects.h"

NODE_CALLBACK(dict_getter, 1, env.Undefined(), {
    auto dict = (jl_value_t *) data;
    if (argc != 1) {
        return env.Undefined();
    }
    auto key = jlnode::to_jl_value(argv[0]);
    auto ret = jl_call3(jlnode::get_func, dict, key, jl_nothing);
    return jlnode::to_napi_value(ret);
})

NODE_CALLBACK(dict_setter, 2, Napi::Boolean::New(env, false), {
    auto dict = (jl_value_t *) data;
    if (argc != 2) {
        return env.Undefined();
    }
    auto key = jlnode::to_jl_value(argv[0]);
    auto value = jlnode::to_jl_value(argv[1]);
    if (jlnode::setindex_func == nullptr) {
        jlnode::setindex_func = jl_eval_string("import NodeCall.jlnode_setindex!;NodeCall.jlnode_setindex!");
    }
    auto ret = jl_call3(jlnode::setindex_func, dict, value, key);
    return jlnode::to_napi_value(ret);
})

NODE_CALLBACK(dict_haskey, 1, Napi::Boolean::New(env, false), {
    auto dict = (jl_value_t *) data;
    if (argc != 1) {
        return env.Undefined();
    }
    auto prop = argv[0];
    auto key = jlnode::to_jl_value(prop);
    auto ret = jl_call2(jlnode::haskey_func, dict, key);
    return jlnode::to_napi_value(ret);
})

NODE_CALLBACK(dict_keys, 0, Napi::Boolean::New(env, false), {
    auto dict = (jl_value_t *) data;
    if (argc != 0) {
        return env.Undefined();
    }
    auto ret = jl_call1(jlnode::keys_func, dict);
    return jlnode::to_napi_value(ret);
})

napi_status create_object_dict(napi_env env, jl_value_t *dict, napi_value *ret) {
    WRAP_ERROR(
        auto nv = Napi::Object::New(env);
        std::vector<napi_property_descriptor> ps{
            {"__get__", nullptr, dict_getter, nullptr, nullptr, nullptr, napi_default, dict},
            {"__set__", nullptr, dict_setter, nullptr, nullptr, nullptr, napi_default, dict},
            {"__has__", nullptr, dict_haskey, nullptr, nullptr, nullptr, napi_default, dict},
            {"__keys__", nullptr, dict_keys, nullptr, nullptr, nullptr, napi_default, dict}
        };
        napi_define_properties(env, nv, ps.size(), ps.data());
        *ret = nv;
    );
    return napi_ok;
}

napi_status create_object_mutable(napi_env env, jl_value_t *v, napi_value *ret) {
    *ret = nullptr;
    return napi_ok;
}
