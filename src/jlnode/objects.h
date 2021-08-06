#ifndef JLNODE_ADDON_OBJECTS_H
#define JLNODE_ADDON_OBJECTS_H

#include <string>
#include <vector>
#include <napi.h>

#include "jlsyms.h"
#include "utils.h"

extern "C" {

napi_status create_object_mutable(napi_env env, jl_value_t *v, napi_value *ret);

napi_status add_finalizer(napi_env env, napi_value v, void *func, void *data);

}

namespace jlnode {

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
    auto status = napi_add_finalizer(env, v, data, FinalizeData<Finalizer>::Wrapper, finalizeData, nullptr);
    if (status != napi_ok) {
        delete finalizeData;
        NAPI_THROW_IF_FAILED_VOID(env, status);
    }
}

}

#endif //JLNODE_ADDON_OBJECTS_H
