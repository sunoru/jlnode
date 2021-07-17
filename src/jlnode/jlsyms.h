#ifndef JLNODE_ADDON_JLSYMS_H
#define JLNODE_ADDON_JLSYMS_H

#include <cassert>
#include <csetjmp>
#include <uv.h>
#include <napi.h>

typedef void jl_module_t;
typedef struct _jl_value_t jl_value_t;
typedef jl_value_t jl_function_t;
#define STORE_ARRAY_LEN
#define JL_DATA_TYPE
#define STATIC_INLINE static inline

// Copied from julia.h
// Keeping only necessary symbols here.
extern "C" {

typedef struct _jl_sym_t {
    JL_DATA_TYPE
    struct _jl_sym_t *left;
    struct _jl_sym_t *right;
    uintptr_t hash;
} jl_sym_t;

const char *jl_symbol_name(jl_sym_t *s);

typedef struct {
    JL_DATA_TYPE
    size_t length;
} jl_svec_t;

typedef struct {
    /*
      how - allocation style
      0 = data is inlined, or a foreign pointer we don't manage
      1 = julia-allocated buffer that needs to be marked
      2 = malloc-allocated pointer this array object manages
      3 = has a pointer to the object that owns the data
    */
    uint16_t how: 2;
    uint16_t ndims: 9;
    uint16_t pooled: 1;
    uint16_t ptrarray: 1; // representation is pointer array
    uint16_t hasptr: 1; // representation has embedded pointers
    uint16_t isshared: 1; // data is shared by multiple Arrays
    uint16_t isaligned: 1; // data allocated with memalign
} jl_array_flags_t;

typedef struct {
    JL_DATA_TYPE
    void *data;
#ifdef STORE_ARRAY_LEN
    size_t length;
#endif
    jl_array_flags_t flags;
    uint16_t elsize;  // element size including alignment (dim 1 memory stride)
    uint32_t offset;  // for 1-d only. does not need to get big.
    size_t nrows;
    union {
        // 1d
        size_t maxsize;
        // Nd
        size_t ncols;
    };
    // other dim sizes go here for ndims > 2

    // followed by alignment padding and inline data, or owner pointer
} jl_array_t;

typedef struct {
    JL_DATA_TYPE
    jl_sym_t *name;
    struct _jl_module_t *module;
    jl_svec_t *names;  // field names
    // `wrapper` is either the only instantiation of the type (if no parameters)
    // or a UnionAll accepting parameters to make an instantiation.
    jl_value_t *wrapper;
    jl_svec_t *cache;        // sorted array
    jl_svec_t *linearcache;  // unsorted array
    intptr_t hash;
    struct _jl_methtable_t *mt;
    jl_array_t *partial;     // incomplete instantiations of this type
} jl_typename_t;

typedef struct {
    uint32_t nfields;
    uint32_t npointers; // number of pointers embedded inside
    int32_t first_ptr; // index of the first pointer (or -1)
    uint16_t alignment; // strictest alignment over all fields
    uint16_t haspadding: 1; // has internal undefined bytes
    uint16_t fielddesc_type: 2; // 0 -> 8, 1 -> 16, 2 -> 32
    // union {
    //     jl_fielddesc8_t field8[nfields];
    //     jl_fielddesc16_t field16[nfields];
    //     jl_fielddesc32_t field32[nfields];
    // };
    // union { // offsets relative to data start in words
    //     uint8_t ptr8[npointers];
    //     uint16_t ptr16[npointers];
    //     uint32_t ptr32[npointers];
    // };
} jl_datatype_layout_t;

typedef struct _jl_datatype_t {
    JL_DATA_TYPE
    jl_typename_t *name;
    struct _jl_datatype_t *super;
    jl_svec_t *parameters;
    jl_svec_t *types;
    jl_svec_t *names;
    jl_value_t *instance;  // for singletons
    const jl_datatype_layout_t *layout;
    int32_t size; // TODO: move to _jl_datatype_layout_t
    int32_t ninitialized;
    uint32_t hash;
    uint8_t abstract;
    uint8_t mutabl;
    // memoized properties
    uint8_t hasfreetypevars; // majority part of isconcrete computation
    uint8_t isconcretetype; // whether this type can have instances
    uint8_t isdispatchtuple; // aka isleaftupletype
    uint8_t isbitstype; // relevant query for C-api and type-parameters
    uint8_t zeroinit; // if one or more fields requires zero-initialization
    uint8_t isinlinealloc; // if this is allocated inline
    uint8_t has_concrete_subtype; // If clear, no value will have this datatype
} jl_datatype_t;

extern jl_value_t *jl_nothing;

extern jl_datatype_t *jl_simplevector_type;

jl_value_t *jl_typeof(jl_value_t *v);

#define jl_svec_len(t)              (((jl_svec_t*)(t))->length)
#define jl_svec_set_len_unsafe(t, n) (((jl_svec_t*)(t))->length=(n))
#define jl_svec_data(t) ((jl_value_t**)((char*)(t) + sizeof(jl_svec_t)))

#define jl_typeis(v, t) (jl_typeof(v)==(jl_value_t*)(t))

STATIC_INLINE jl_value_t *jl_svecref(void *t, size_t i) {
    assert(jl_typeis((jl_value_t *) t, jl_simplevector_type));
    assert(i < jl_svec_len(t));
    return jl_svec_data(t)[i];
}

extern jl_module_t *jl_base_module;


extern jl_value_t *jl_call(jl_function_t *f, jl_value_t **args, int32_t nargs);
extern jl_value_t *jl_call1(jl_function_t *f, jl_value_t *a);
extern jl_value_t *jl_call2(jl_function_t *f, jl_value_t *a, jl_value_t *b);
extern jl_value_t *jl_call3(jl_function_t *f, jl_value_t *a, jl_value_t *b, jl_value_t *c);

extern jl_value_t *jl_cstr_to_string(const char *str);
extern jl_value_t *jl_eval_string(const char *str);

extern jl_value_t *jl_get_nth_field(jl_value_t *v, size_t i);

extern void jl_set_nth_field(jl_value_t *v, size_t i, jl_value_t *rhs);

extern jl_value_t *jl_box_voidpointer(void *x);
extern jl_value_t *jl_box_uint64(uint64_t x);
extern void *jl_unbox_voidpointer(jl_value_t *v);
extern uint64_t jl_unbox_uint64(jl_value_t *v);

STATIC_INLINE jl_svec_t *jl_field_names(jl_datatype_t *st) {
    jl_svec_t *names = st->names;
    if (!names)
        names = st->name->names;
    return names;
}

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
