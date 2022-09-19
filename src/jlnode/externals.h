#ifndef JLNODE_EXTERNALS_H
#define JLNODE_EXTERNALS_H

#include <napi.h>
#include "utils.h"

extern "C" {

LIBRARY_API napi_status create_external(napi_env env, void *data, napi_value *result);

}

#endif //JLNODE_EXTERNALS_H
