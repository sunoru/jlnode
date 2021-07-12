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
    void *data;
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

}

#endif //JLNODE_ADDON_OBJECTS_H
