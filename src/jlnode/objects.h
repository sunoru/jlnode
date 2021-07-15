#ifndef JLNODE_ADDON_OBJECTS_H
#define JLNODE_ADDON_OBJECTS_H

#include <string>
#include <vector>
#include <napi.h>

#include "jlsyms.h"
#include "utils.h"

extern "C" {

napi_status create_object_dict(napi_env env, jl_value_t *dict, napi_value *ret);
napi_status create_object_mutable(napi_env env, jl_value_t *v, napi_value *ret);

napi_status add_finalizer(napi_env env, napi_value v, void *func, void *data);

}

namespace jlnode {

template<typename Getter, typename Setter>
struct MutableAccessorData {
    static inline
    napi_value GetterWrapper(napi_env env, napi_callback_info info) {
        return Napi::details::WrapCallback([&] {
            Napi::CallbackInfo callbackInfo(env, info);
            auto callbackData =
                static_cast<MutableAccessorData *>(callbackInfo.Data());
            callbackInfo.SetData(callbackData->data);
            return callbackData->getterCallback(callbackData->name, callbackInfo);
        });
    }

    static inline
    napi_value SetterWrapper(napi_env env, napi_callback_info info) {
        return Napi::details::WrapCallback([&] {
            Napi::CallbackInfo callbackInfo(env, info);
            auto callbackData =
                static_cast<MutableAccessorData *>(callbackInfo.Data());
            callbackInfo.SetData(callbackData->data);
            callbackData->setterCallback(callbackData->name, callbackInfo);
            return nullptr;
        });
    }

    Getter getterCallback;
    Setter setterCallback;
    std::string name;
    void *data = nullptr;
};

template<typename Getter, typename Setter>
inline Napi::PropertyDescriptor MutableAccessor(
    Napi::Env env,
    Napi::Object object,
    Napi::Name name,
    Getter getter,
    Setter setter,
    napi_property_attributes attributes,
    void *data
) {
    using CbData = MutableAccessorData<Getter, Setter>;
    auto nameString = Napi::String(env, name).Utf8Value();
    auto callbackData = new CbData({getter, setter, nameString, data});

    napi_status status = Napi::details::AttachData(env, object, callbackData);
    if (status != napi_ok) {
        delete callbackData;
        NAPI_THROW_IF_FAILED(env, status, napi_property_descriptor());
    }

    return Napi::PropertyDescriptor(
        {
            nullptr,
            name,
            nullptr,
            CbData::GetterWrapper,
            CbData::SetterWrapper,
            nullptr,
            attributes,
            callbackData
        }
    );
}

template<typename Finalizer>
struct FinalizeData {
    static inline void Wrapper(napi_env env, void *data, void *finalizeHint) NAPI_NOEXCEPT {
        Napi::details::WrapVoidCallback([&] {
            auto finalizeData = static_cast<FinalizeData *>(finalizeHint);
            finalizeData->callback(Napi::Env(env), data, finalizeData->hint);
            delete finalizeData;
        });
    }

    Finalizer callback;
    void *hint;
};

template<typename Finalizer>
void add_finalizer(napi_env env, napi_value v, Finalizer finalizeCallback, void *data, void *finalizeHint) {
    auto finalizeData =
        new FinalizeData<Finalizer>({std::move(finalizeCallback), finalizeHint});
    auto status = napi_add_finalizer(env, v, data, FinalizeData<Finalizer>::Wrapper, finalizeHint, nullptr);
    if (status != napi_ok) {
        delete finalizeData;
        NAPI_THROW_IF_FAILED_VOID(env, status);
    }
}

}

#endif //JLNODE_ADDON_OBJECTS_H
