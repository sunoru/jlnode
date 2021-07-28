#include <string>

#include "instance.h"
#include "objects.h"
#include "jlsyms.h"

jlnode::Instance *instance;

extern "C" {

int initialize(
    jl_module_t *module,
    const char *addon_path,
    const char **args,
    size_t argc,
    napi_env *env,
    uv_loop_t **loop
) {
    instance = new jlnode::Instance;
    auto ret = instance->Initialize(addon_path, env, args, argc);
    if (ret != 0) { return ret; }
    *loop = instance->event_loop;
    return jlnode::initialize_utils(module);
}

int dispose() {
    if (instance == nullptr) {
        return 0;
    }
    auto ret = instance->Dispose();
    delete instance;
    return ret;
}

int node_uv_run(uv_loop_t *loop, uv_run_mode mode) {
    auto ret = uv_run(loop, mode);
    if (ret == 0) {
        instance->platform->DrainTasks(instance->isolate);
        ret = uv_run(loop, mode);
    }
    return ret;
}

}
