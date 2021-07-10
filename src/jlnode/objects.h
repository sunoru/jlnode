#ifndef JLNODE_ADDON_OBJECTS_H
#define JLNODE_ADDON_OBJECTS_H

#include <vector>
#include <napi.h>

#include "jlsyms.h"
#include "utils.h"

extern "C" {

napi_status create_object_dict(napi_env env, jl_value_t *dict, napi_value *ret);
napi_status create_object_mutable(napi_env env, jl_value_t *v, napi_value *ret);

}


#endif //JLNODE_ADDON_OBJECTS_H
