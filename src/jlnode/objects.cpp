#include "objects.h"

Napi::Value object_getter(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    if (info.Length() != 1) {
        return env.Undefined();
    }
    Napi::Value result;
    auto object = jl_box_voidpointer(info.Data());
    auto key = jl_box_voidpointer(info[0]);
    GET_FUNC_POINTER(
        jlnode::getproperty_func,
        "jlnode_getproperty",
        env.Undefined()
    );
    auto ret = jl_call2(jlnode::getproperty_func, object, key);
    JL_GC_PUSH1(&ret);
    jlnode::check_jl_exception(env, 1);
    result = Napi::Value(env, (napi_value) jl_unbox_voidpointer(ret));
    JL_GC_POP();
    return result;
}

Napi::Value object_setter(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    if (info.Length() != 2) {
        return env.Undefined();
    }
    Napi::Value result;
    auto object = jl_box_voidpointer(info.Data());
    auto key = jl_box_voidpointer(info[0]);
    auto value = jl_box_voidpointer(info[1]);
    GET_FUNC_POINTER(
        jlnode::setproperty_func,
        "jlnode_setproperty!",
        env.Undefined()
    );
    auto ret = jl_call3(jlnode::setproperty_func, object, key, value);
    JL_GC_PUSH1(&ret);
    jlnode::check_jl_exception(env, 1);
    result = Napi::Value(env, (napi_value) jl_unbox_voidpointer(ret));
    JL_GC_POP();
    return result;
}

Napi::Value object_haskey(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    if (info.Length() != 1) {
        return Napi::Boolean::New(env, false);
    }
    Napi::Value result;
    auto object = jl_box_voidpointer(info.Data());
    auto key = jl_box_voidpointer(info[0]);
    GET_FUNC_POINTER(
        jlnode::hasproperty_func,
        "jlnode_hasproperty",
        env.Undefined()
    );
    auto ret = jl_call2(jlnode::hasproperty_func, object, key);
    JL_GC_PUSH1(&ret);
    jlnode::check_jl_exception(env, 1);
    result = Napi::Value(env, (napi_value) jl_unbox_voidpointer(ret));
    JL_GC_POP();
    return result;
}

Napi::Value object_keys(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    if (info.Length() != 0) {
        return env.Undefined();
    }
    Napi::Value result;
    auto object = jl_box_voidpointer(info.Data());
    GET_FUNC_POINTER(
        jlnode::propertynames_func,
        "jlnode_propertynames",
        env.Undefined()
    );
    auto ret = jl_call1(jlnode::propertynames_func, object);
    JL_GC_PUSH1(&ret);
    jlnode::check_jl_exception(env, 1);
    result = Napi::Value(env, (napi_value) jl_unbox_voidpointer(ret));
    JL_GC_POP();
    return result;
}

napi_status create_object_mutable(napi_env _env, void *v, napi_value *ret) {
    *ret = nullptr;
    WRAP_ERROR(
        auto env = Napi::Env(_env);
        auto nv = Napi::Object::New(_env);
        nv.DefineProperties(
            {
                Napi::PropertyDescriptor::Function(env, nv, "__get__", object_getter, napi_default, v),
                Napi::PropertyDescriptor::Function(env, nv, "__set__", object_setter, napi_default, v),
                Napi::PropertyDescriptor::Function(env, nv, "__has__", object_haskey, napi_default, v),
                Napi::PropertyDescriptor::Function(env, nv, "__keys__", object_keys, napi_default, v)
            }
        );
        *ret = nv;
    );
    return napi_ok;
}


void object_finalizer(Napi::Env env, void *data, void *func) {
    GET_FUNC_POINTER(
        jlnode::object_finalizer_func,
        "object_finalizer",
    );
    auto data_ptr = jl_box_voidpointer(data);
    auto f_ptr = jl_box_voidpointer(func);
    jl_call2(jlnode::object_finalizer_func, f_ptr, data_ptr);
}

napi_status add_finalizer(napi_env _env, napi_value v, void *func, void *data) {
    WRAP_ERROR(
        jlnode::add_finalizer(_env, v, object_finalizer, data, func);
    );
    return napi_ok;
}

