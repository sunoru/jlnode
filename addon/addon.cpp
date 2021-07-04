#define NAPI_VERSION 7

#include <cassert>

#include <node_api.h>
//#include <nan.h>
#include "init.h"

//using Nan::GetFunction;
//using Nan::New;
//using Nan::Set;
//using v8::FunctionTemplate;
//using v8::String;

//NAN_MODULE_INIT(InitAll) {
//    Set(target, New<String>("initialize").ToLocalChecked(),
//        GetFunction(New<FunctionTemplate>(jlnode_addon::Initialize)).ToLocalChecked());
//}
//
//NODE_MODULE(addon, InitAll)

#define DECLARE_NAPI_METHOD(name, func)                                        \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

static napi_value Init(napi_env env, napi_value exports) {
    napi_status status;
    napi_property_descriptor desc = DECLARE_NAPI_METHOD("initialize", jlnode_addon::Initialize);
    status = napi_define_properties(env, exports, 1, &desc);
    assert(status == napi_ok);
    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)