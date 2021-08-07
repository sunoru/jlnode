#include <string>

#include "node_api.h"
#include "instance.h"
#include "objects.h"
#include "jlsyms.h"

jlnode::Instance *instance;
static bool initialized = false;
static uv_idle_t jl_yield_idler;

void _jl_yield(uv_idle_t *_) {
    jl_yield();
}

extern "C" {

int start_node(
    const char *addon_path,
    const char **args,
    size_t argc,
    napi_env *env
) {
    instance = new jlnode::Instance;
    return instance->Initialize(addon_path, env, args, argc);
}

int initialize(uv_loop_t *loop, jl_module_t *module) {
    uv_idle_init(loop, &jl_yield_idler);
    uv_idle_start(&jl_yield_idler, _jl_yield);
    auto ret = jlnode::initialize_utils(module);
    if (ret == 0) {
        initialized = true;
    }
    return ret;
}

int dispose() {
    if (initialized) {
        uv_idle_stop(&jl_yield_idler);
        uv_close((uv_handle_t *) &jl_yield_idler, nullptr);
        initialized = false;
    }
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
