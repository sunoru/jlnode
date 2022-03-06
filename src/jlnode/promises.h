#ifndef JLNODE_PROMISES_H
#define JLNODE_PROMISES_H

#include <node.h>
#include <napi.h>
#include "utils.h"

extern "C" {

typedef enum {
    promise_pending = 0,
    promise_fulfilled,
    promise_rejected
} promise_state;

napi_status get_promise_state(napi_env env, napi_value _promise, promise_state *state, napi_value *result);

}


#endif //JLNODE_PROMISES_H
