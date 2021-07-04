#include "init.h"

namespace jlnode_addon {

//NAN_METHOD(Initialize) {
//    int result = Nan::To<int>(info[0]).FromJust();
//
//    info.GetReturnValue().Set(result);
//}

napi_value Initialize(napi_env env, napi_callback_info info) {
    napi_status status;
    napi_value ret;
    status = napi_create_int64(env, (int64_t) env, &ret);
    assert(status == napi_ok);
    return ret;
}

}