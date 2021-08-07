#ifndef JLNODE_ADDON_EXTERNALS_H
#define JLNODE_ADDON_EXTERNALS_H

#include <napi.h>
#include "utils.h"

extern "C" {

napi_status create_external(napi_env env, void *data, napi_value *result);

}

#endif //JLNODE_ADDON_EXTERNALS_H
