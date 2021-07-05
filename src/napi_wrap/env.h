#ifndef JLNODE_NAPI_WRAP_ENV_H
#define JLNODE_NAPI_WRAP_ENV_H

#include <napi.h>
#include "utils/wrappers.h"
#include "errors.h"

void set_global_env(const void *p);

void set_global_handle_scope(const void *p);

extern "C" {

napi_env get_global_env();

napi_handle_scope get_global_handle_scope();

napi_value run_script(JlnodeResult *_result, napi_env env, const char *scripts);

}

#endif //JLNODE_NAPI_WRAP_ENV_H
