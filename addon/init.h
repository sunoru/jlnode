#ifndef JLNODE_INIT_H
#define JLNODE_INIT_H

#include <cassert>
#include <node_api.h>

namespace jlnode_addon {

//NAN_METHOD(Initialize);

napi_value Initialize(napi_env env, napi_callback_info info);

}

#endif //JLNODE_INIT_H
