#ifndef JLNODE_THREADING_H
#define JLNODE_THREADING_H

#include <napi.h>

#include "jlsyms.h"
#include "utils.h"

extern "C" {

napi_status threadsafe_call(
    napi_env env,
    void *func
);

}

namespace jlnode {


using Context = nullptr_t;
using DataType = void;
void CallJs(Napi::Env env, Napi::Function callback, Context *context, DataType *data);
using TSFN = Napi::TypedThreadSafeFunction<Context, DataType, CallJs>;

extern TSFN threadsafe_jlcall;

int initialize_threading(napi_env env);
void dispose_threading();

}

#endif //JLNODE_THREADING_H

