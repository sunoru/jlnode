#include "objects.h"

Napi::Value object_getter(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    if (info.Length() != 1) {
        return env.Undefined();
    }
    auto result = jlnode::util_functions.getproperty(info.Data(), info[0]);
    return Napi::Value(env, result);
}

Napi::Value object_setter(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    if (info.Length() != 2) {
        return env.Undefined();
    }
    auto result = jlnode::util_functions.setproperty(info.Data(), info[0], info[1]);
    return Napi::Value(env, result);
}

Napi::Value object_haskey(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    if (info.Length() != 1) {
        return Napi::Boolean::New(env, false);
    }
    auto result = jlnode::util_functions.hasproperty(info.Data(), info[0]);
    return Napi::Value(env, result);
}

Napi::Value object_keys(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    if (info.Length() != 0) {
        return env.Undefined();
    }
    auto result = jlnode::util_functions.propertynames(info.Data());
    return Napi::Value(env, result);
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
    jlnode::util_functions.object_finalizer(func, data);
}

napi_status add_finalizer(napi_env _env, napi_value v, void *func, void *data) {
    WRAP_ERROR(
        jlnode::add_finalizer(_env, v, object_finalizer, data, func);
    );
    return napi_ok;
}

