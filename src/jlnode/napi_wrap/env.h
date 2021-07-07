#ifndef JLNODE_NAPI_WRAP_ENV_H
#define JLNODE_NAPI_WRAP_ENV_H

#include <napi.h>
#include "../utils/wrappers.h"
#include "errors.h"

void set_global_env(const void *p);

void set_global_handle_scope(const void *p);

napi_handle_scope get_global_handle_scope();

extern "C" {

napi_env get_global_env();

napi_value env_get_global(JlnodeResult *_result, napi_env env);
napi_value env_get_undefined(JlnodeResult *_result, napi_env env);
napi_value env_get_null(JlnodeResult *_result, napi_env env);
bool env_is_exception_pending(JlnodeResult *_result, napi_env env);
void env_get_and_clear_pending_exception(JlnodeResult *_result, napi_env env);

napi_value run_script(JlnodeResult *_result, napi_env env, const char *scripts);

const napi_node_version *get_node_version(JlnodeResult *_result, napi_env _env);
uint32_t get_napi_version(JlnodeResult *_result, napi_env _env);

}

#endif //JLNODE_NAPI_WRAP_ENV_H
