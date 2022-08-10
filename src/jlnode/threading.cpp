#include "threading.h"

napi_status threadsafe_call(napi_env _env, void *func) {
    auto env = Napi::Env(_env);
    auto status = jlnode::threadsafe_jlcall.BlockingCall(func);
    return status;
}

jlnode::TSFN jlnode::threadsafe_jlcall;
Napi::Function jlcall_func;
int jlnode::initialize_threading(napi_env _env) {
    auto env = Napi::Env(_env);
    threadsafe_jlcall = TSFN::New(
        env,
        "jlcall",
        0,
        1
    );
    return 0;
}

void jlnode::CallJs(Napi::Env env, Napi::Function callback, Context *context, DataType *data) {
    auto f_ptr = data;
    GET_FUNC_POINTER(
        call_threadsafe_func,
        "jlnode_call_threadsafe",
        (void)0
    );
    jl_call1(call_threadsafe_func, jl_box_voidpointer(f_ptr));
}

void jlnode::dispose_threading() {
    threadsafe_jlcall.Release();
}