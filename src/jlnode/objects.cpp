#include "objects.h"

Napi::Value dict_getter(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    if (info.Length() != 1) {
        return env.Undefined();
    }
    auto dict = (jl_value_t *) info.Data();
    auto key = jlnode::to_jl_value(info[0]);
    auto ret = jl_call3(jlnode::get_func, dict, key, jl_nothing);
    return {env, jlnode::to_napi_value(ret)};
}

Napi::Value dict_setter(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    if (info.Length() != 2) {
        return env.Undefined();
    }
    auto dict = (jl_value_t *) info.Data();
    auto key = jlnode::to_jl_value(info[0]);
    auto value = jlnode::to_jl_value(info[1]);
    if (jlnode::setindex_func == nullptr) {
        jlnode::setindex_func = jl_eval_string("import NodeCall.jlnode_setindex!;NodeCall.jlnode_setindex!");
    }
    auto ret = jl_call3(jlnode::setindex_func, dict, value, key);
    return {env, jlnode::to_napi_value(ret)};
}

Napi::Value dict_haskey(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    if (info.Length() != 1) {
        return Napi::Boolean::New(env, false);
    }
    auto dict = (jl_value_t *) info.Data();
    auto key = jlnode::to_jl_value(info[0]);
    if (jlnode::setindex_func == nullptr) {
        jlnode::setindex_func = jl_eval_string("import NodeCall.jlnode_setindex!;NodeCall.jlnode_setindex!");
    }
    auto ret = jl_call2(jlnode::haskey_func, dict, key);
    return {env, jlnode::to_napi_value(ret)};
}

Napi::Value dict_keys(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    if (info.Length() != 0) {
        return env.Undefined();
    }
    auto dict = (jl_value_t *) info.Data();
    auto ret = jl_call1(jlnode::keys_func, dict);
    return {env, jlnode::to_napi_value(ret)};
}

napi_status create_object_dict(napi_env _env, jl_value_t *dict, napi_value *ret) {
    *ret = nullptr;
    WRAP_ERROR(
        auto env = Napi::Env(_env);
        auto nv = Napi::Object::New(_env);
        nv.DefineProperties(
            {
                Napi::PropertyDescriptor::Function(env, nv, "__get__", dict_getter, napi_default, dict),
                Napi::PropertyDescriptor::Function(env, nv, "__set__", dict_setter, napi_default, dict),
                Napi::PropertyDescriptor::Function(env, nv, "__has__", dict_haskey, napi_default, dict),
                Napi::PropertyDescriptor::Function(env, nv, "__keys__", dict_keys, napi_default, dict)
            }
        );
        *ret = nv;
    );
    return napi_ok;
}


//
//NODE_CALLBACK(mutable_setter,
//2,
//Napi::Boolean::New(env,
//false), {
//auto dict = (jl_value_t *) data;
//if (argc != 2) {
//return env.
//
//Undefined();
//
//}
//auto cl = Napi::Function(env, env.RunScript("console.log"));
//cl.Call(2, argv);
//auto key = jlnode::to_jl_value(argv[0]);
//auto value = jlnode::to_jl_value(argv[1]);
//if (jlnode::setproperty_func == nullptr) {
//GET_FUNC_POINTER(
//    jlnode::setproperty_func,
//    "import NodeCall.jlnode_setproperty!;NodeCall.jlnode_setproperty!",
//    env.Undefined()
//);
//}
//auto ret = jl_call3(jlnode::setindex_func, dict, key, value);
//return
//jlnode::to_napi_value(ret);
//})

Napi::Value mutable_getter(const std::string &name, const Napi::CallbackInfo &info) {
    auto env = info.Env();
    if (info.Length() != 0) {
        return env.Undefined();
    }
    if (jlnode::getproperty_func == nullptr) {
        GET_FUNC_POINTER(
            jlnode::getproperty_func,
            "import NodeCall.jlnode_getproperty;NodeCall.jlnode_getproperty",
            env.Undefined()
        );
    }
    auto v = (jl_value_t *) info.Data();
    auto key = jl_cstr_to_string(name.c_str());
    auto ret = jl_call2(jlnode::getproperty_func, v, key);
    return {env, jlnode::to_napi_value(ret)};
}

Napi::Value mutable_setter(const std::string &name, const Napi::CallbackInfo &info) {
    auto env = info.Env();
    if (info.Length() != 1) {
        return env.Undefined();
    }
    if (jlnode::setproperty_func == nullptr) {
        GET_FUNC_POINTER(
            jlnode::setproperty_func,
            "import NodeCall.jlnode_setproperty!;NodeCall.jlnode_setproperty!",
            env.Undefined()
        );
    }
    auto v = (jl_value_t *) info.Data();
    auto key = jl_cstr_to_string(name.c_str());
    auto value = jlnode::to_jl_value(info[0]);
    auto ret = jl_call3(jlnode::setproperty_func, v, key, value);
    return {env, jlnode::to_napi_value(ret)};
}

napi_status create_object_mutable(napi_env _env, jl_value_t *v, napi_value *ret) {
    *ret = nullptr;
    WRAP_ERROR(
        auto env = Napi::Env(_env);
        auto nv = Napi::Object::New(env);
        std::vector<napi_property_descriptor> ps;
        auto jl_property_names = jl_call1(jlnode::propertynames_func, v);
        JL_GC_PUSH1(&jl_property_names);
        Napi::Array property_names(env, jlnode::to_napi_value(jl_property_names));
        auto n = property_names.Length();
        for (int i = 0; i < n; ++i) {
            auto name_symbol = Napi::Object(env, property_names.Get(i));  // Actually a Symbol
            auto name = Napi::Name(env, name_symbol.Get("description"));
            nv.DefineProperty(jlnode::MutableAccessor(
                env, nv, name,
                mutable_getter,
                mutable_setter,
                napi_writable,
                v
            ));
        }
        JL_GC_POP();
        napi_define_properties(env, nv, ps.size(), ps.data());
        *ret = nv;
    );
    return napi_ok;
}