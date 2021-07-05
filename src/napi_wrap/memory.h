#ifndef JLNODE_NAPI_WRAP_MEMORY_H
#define JLNODE_NAPI_WRAP_MEMORY_H

#include <napi.h>
#include "env.h"

extern "C" {

napi_handle_scope open_handle_scope(napi_env env);

void close_handle_scope(napi_env env, napi_handle_scope scope);

napi_value dereference(napi_env env, napi_ref ref);

napi_ref make_reference(napi_env env, napi_value value);

void free_reference(napi_env env, napi_ref ref);

}

#endif //JLNODE_NAPI_WRAP_MEMORY_H