#include "promises.h"

napi_status get_promise_state(napi_env _env, napi_value _promise, promise_state *state, napi_value *result) {
    *state = promise_pending;
    WRAP_ERROR(
        auto env = Napi::Env(_env);
        v8::Local<v8::Value> local;
        memcpy(static_cast<void *>(&local), &_promise, sizeof(_promise));
        if (!local->IsPromise()) {
            throw std::runtime_error("The value is not a promise.");
        }
        auto promise = local.As<v8::Promise>();
        auto s = promise->State();
        *state = static_cast<promise_state>(s);
        switch (*state) {
            case promise_pending:
                *result = env.Undefined();
                break;
            case promise_fulfilled:
            case promise_rejected:
                *result = jlnode::JsValueFromV8LocalValue(promise->Result());
                break;
        }
    );
    return napi_ok;
}
