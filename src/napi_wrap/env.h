#ifndef JLNODE_NAPI_WRAP_ENV_H
#define JLNODE_NAPI_WRAP_ENV_H

#include <napi.h>

void set_global_env(const void *p);

void set_global_handle_scope(const void *p);

napi_value run_script(napi_env env, const std::string &scripts);

extern "C" {

napi_env get_global_env();

napi_handle_scope get_global_handle_scope();

}

#endif //JLNODE_NAPI_WRAP_ENV_H
