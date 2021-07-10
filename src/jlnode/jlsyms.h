#ifndef JLNODE_ADDON_JLSYMS_H
#define JLNODE_ADDON_JLSYMS_H

#include <csetjmp>
#include <ucontext.h>
#include <uv.h>
#include <napi.h>

typedef void jl_module_t;
typedef struct _jl_value_t jl_value_t;
typedef jl_value_t jl_function_t;

namespace jlnode {

}

// Copied from julia.h
// Keeping only necessary symbols here.

extern "C" {

extern jl_module_t *jl_base_module;

extern jl_value_t *jl_call1(jl_function_t *f, jl_value_t *a);
extern jl_value_t *jl_call2(jl_function_t *f, jl_value_t *a, jl_value_t *b);
extern jl_value_t *jl_call3(jl_function_t *f, jl_value_t *a, jl_value_t *b, jl_value_t *c);

extern jl_value_t *jl_eval_string(const char *str);

extern jl_value_t *jl_get_nth_field(jl_value_t *v, size_t i);

extern void jl_set_nth_field(jl_value_t *v, size_t i, jl_value_t *rhs);

jl_value_t *jl_box_voidpointer(void *x);
void *jl_unbox_voidpointer(jl_value_t *v);
uint64_t jl_unbox_uint64(jl_value_t *v);

extern jl_value_t *jl_nothing;

// gc -------------------------------------------------------------------------

typedef struct _jl_gcframe_t {
    size_t nroots;
    struct _jl_gcframe_t *prev;
    // actual roots go here
} jl_gcframe_t;

#define AL_N_INLINE 29
typedef struct {
    size_t len;
    size_t max;
    void **items;
    void *_space[AL_N_INLINE];
} arraylist_t;

typedef struct _jl_taggedvalue_t jl_taggedvalue_t;
typedef struct {
    jl_taggedvalue_t *freelist;   // root of list of free objects
    jl_taggedvalue_t *newpages;   // root of list of chunks of free objects
    uint16_t osize;      // size of objects in this pool
} jl_gc_pool_t;

typedef struct {
    // variable for tracking weak references
    arraylist_t weak_refs;
    // live tasks started on this thread
    // that are holding onto a stack from the pool
    arraylist_t live_tasks;

    // variables for tracking malloc'd arrays
    struct _mallocarray_t *mallocarrays;
    struct _mallocarray_t *mafreelist;

    // variables for tracking big objects
    struct _bigval_t *big_objects;

    // variables for tracking "remembered set"
    arraylist_t rem_bindings;
    arraylist_t _remset[2]; // contains jl_value_t*
    // lower bound of the number of pointers inside remembered values
    int remset_nptr;
    arraylist_t *remset;
    arraylist_t *last_remset;

    // variables for allocating objects from pools
#ifdef _P64
#  define JL_GC_N_POOLS 41
#elif MAX_ALIGN == 8
#  define JL_GC_N_POOLS 42
#else
#  define JL_GC_N_POOLS 43
#endif
    jl_gc_pool_t norm_pools[JL_GC_N_POOLS];

#define JL_N_STACK_POOLS 16
    arraylist_t free_stacks[JL_N_STACK_POOLS];
} jl_thread_heap_t;
typedef struct {
    int64_t allocd;
    int64_t freed;
    uint64_t malloc;
    uint64_t realloc;
    uint64_t poolalloc;
    uint64_t bigalloc;
    uint64_t freecall;
} jl_thread_gc_num_t;
typedef ucontext_t jl_ucontext_t;
#ifndef _OS_WINDOWS_
#  define jl_jmp_buf sigjmp_buf
#  if defined(_CPU_ARM_) || defined(_CPU_PPC_) || defined(_CPU_WASM_)
#    define MAX_ALIGN 8
#  elif defined(_CPU_AARCH64_)
// int128 is 16 bytes aligned on aarch64
#    define MAX_ALIGN 16
#  elif defined(_P64)
// Generically we assume MAX_ALIGN is sizeof(void*)
#    define MAX_ALIGN 8
#  else
#    define MAX_ALIGN 4
#  endif
#else
#  include "win32_ucontext.h"
#  define jl_jmp_buf jmp_buf
#  define MAX_ALIGN 8
#endif
typedef struct {
    volatile unsigned long owner;
    uint32_t count;
} jl_mutex_t;
typedef union _jl_gc_mark_data jl_gc_mark_data_t;
typedef struct {
    // thread local increment of `perm_scanned_bytes`
    size_t perm_scanned_bytes;
    // thread local increment of `scanned_bytes`
    size_t scanned_bytes;
    // Number of queued big objects (<= 1024)
    size_t nbig_obj;
    // Array of queued big objects to be moved between the young list
    // and the old list.
    // A set low bit means that the object should be moved from the old list
    // to the young list (`mark_reset_age`).
    // Objects can only be put into this list when the mark bit is flipped to
    // `1` (atomically). Combining with the sync after marking,
    // this makes sure that a single objects can only appear once in
    // the lists (the mark bit cannot be flipped to `0` without sweeping)
    void *big_obj[1024];
    jl_mutex_t stack_lock;
    void **pc_stack;
    void **pc_stack_end;
    jl_gc_mark_data_t *data_stack;
} jl_gc_mark_cache_t;
typedef struct {
    void **pc; // Current stack address for the pc (up growing)
    jl_gc_mark_data_t *data; // Current stack address for the data (up growing)
    void **pc_start; // Cached value of `gc_cache->pc_stack`
    void **pc_end; // Cached value of `gc_cache->pc_stack_end`
} jl_gc_mark_sp_t;
struct _jl_tls_states_t {
    struct _jl_gcframe_t *pgcstack;
    size_t world_age;
    int16_t tid;
    uint64_t rngseed;
    volatile size_t *safepoint;
    volatile int8_t sleep_check_state;
    // Whether it is safe to execute GC at the same time.
#define JL_GC_STATE_WAITING 1
    // gc_state = 1 means the thread is doing GC or is waiting for the GC to
    //              finish.
#define JL_GC_STATE_SAFE 2
    // gc_state = 2 means the thread is running unmanaged code that can be
    //              execute at the same time with the GC.
    volatile int8_t gc_state;
    volatile int8_t in_finalizer;
    int8_t disable_gc;
    jl_thread_heap_t heap;
    jl_thread_gc_num_t gc_num;
    uv_mutex_t sleep_lock;
    uv_cond_t wake_signal;
    volatile sig_atomic_t defer_signal;
    struct _jl_task_t *current_task;
    struct _jl_task_t *next_task;
#ifdef MIGRATE_TASKS
    struct _jl_task_t *previous_task;
#endif
    struct _jl_task_t *root_task;
    void *stackbase;
    size_t stacksize;
    jl_ucontext_t base_ctx; // base context of stack
    jl_jmp_buf *safe_restore;
    // Temp storage for exception thrown in signal handler. Not rooted.
    struct _jl_value_t *sig_exception;
    // Temporary backtrace buffer. Scanned for gc roots when bt_size > 0.
    struct _jl_bt_element_t *bt_data; // JL_MAX_BT_SIZE + 1 elements long
    size_t bt_size;    // Size for backtrace in transit in bt_data
    // Atomically set by the sender, reset by the handler.
    volatile sig_atomic_t signal_request;
    // Allow the sigint to be raised asynchronously
    // this is limited to the few places we do synchronous IO
    // we can make this more general (similar to defer_signal) if necessary
    volatile sig_atomic_t io_wait;
#ifdef _OS_WINDOWS_
    int needs_resetstkoflw;
#else
    void *signal_stack;
#endif
    unsigned long system_id;
    // execution of certain certain impure
    // statements is prohibited from certain
    // callbacks (such as generated functions)
    // as it may make compilation undecidable
    int in_pure_callback;
    // Counter to disable finalizer **on the current thread**
    int finalizers_inhibited;
    arraylist_t finalizers;
    jl_gc_mark_cache_t gc_cache;
    arraylist_t sweep_objs;
    jl_gc_mark_sp_t gc_mark_sp;
    // Saved exception for previous external API call or NULL if cleared.
    // Access via jl_exception_occurred().
    struct _jl_value_t *previous_exception;
};
typedef struct _jl_tls_states_t jl_tls_states_t;
typedef jl_tls_states_t *jl_ptls_t;
extern const jl_ptls_t jl_get_ptls_states(void);
#define jl_pgcstack (jl_get_ptls_states()->pgcstack)
#define JL_GC_ENCODE_PUSH(n)       ((((size_t)(n))<<2)|1)

#define JL_GC_PUSH1(arg1)                                                                               \
  void *__gc_stkf[] = {(void*)JL_GC_ENCODE_PUSH(1), jl_pgcstack, arg1};                                 \
  jl_pgcstack = (jl_gcframe_t*)__gc_stkf;

#define JL_GC_POP() (jl_pgcstack = jl_pgcstack->prev)

}

#endif //JLNODE_ADDON_JLSYMS_H
