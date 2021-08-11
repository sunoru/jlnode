#ifndef JLNODE_ADDON_THREADING_H
#define JLNODE_ADDON_THREADING_H

#include <napi.h>
#include "utils.h"

extern "C" {

napi_status create_threadsafe_function(napi_env env, void *func, void *channel);

}

#endif //JLNODE_ADDON_THREADING_H
