#include <string>

#include <node_api.h>

#include "instance.h"
#include "objects.h"

jlnode::Instance *instance;
static bool initialized = false;
static uv_timer_t *jl_yield_timer;

void _jl_yield(uv_timer_t *_) {
    jlnode::util_functions.jl_yield();
}

void _jl_yield_close(uv_handle_t *timer) {
    delete (uv_timer_t *) timer;
}

extern "C" {

LIBRARY_API int test() {
    fprintf(stderr, "Test\n");
    return 0;
}

LIBRARY_API int start_node(
    const char *addon_path,
    const char **args,
    size_t argc,
    napi_env *env
) {
    instance = new jlnode::Instance;
    return instance->Initialize(addon_path, env, args, argc);
}

LIBRARY_API int initialize(uv_loop_t *loop, jl_functions utils) {
    jl_yield_timer = new uv_timer_t;
    uv_timer_init(loop, jl_yield_timer);
    uv_timer_start(jl_yield_timer, _jl_yield, 0, 1);
    auto ret = jlnode::initialize_utils(utils);
    if (ret == 0) {
        initialized = true;
    }
    return ret;
}

LIBRARY_API int dispose() {
    if (initialized) {
        initialized = false;
        uv_timer_stop(jl_yield_timer);
        uv_close((uv_handle_t *) jl_yield_timer, _jl_yield_close);
    }
    if (instance == nullptr) {
        return 0;
    }
    auto ret = instance->Dispose();
    delete instance;
    return ret;
}

LIBRARY_API int node_uv_run(uv_loop_t *loop, uv_run_mode mode) {
    auto ret = uv_run(loop, mode);
    if (ret == 0) {
        instance->platform->DrainTasks(instance->environment_config->setup->isolate());
        ret = uv_run(loop, mode);
    }
    return ret;
}

}
