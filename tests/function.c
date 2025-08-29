#include <stdio.h>
#include <stdlib.h>
#include "uprintf.h"

typedef int *(*fun0_t)(void);

typedef struct {
    int num;
    fun0_t f0;
} Result;

typedef Result *(*fun_t)(void);
typedef fun_t (*fun1_t)(void);

typedef struct {
    fun1_t f;
    fun1_t *fp;
} Functions;

typedef struct {
    void *(*alloc)(size_t);
    void *(*realloc)(void *, size_t);
    void (*free)(void *);
} AllocatorMethods;

static int num = 200;

int *fun0(void) { return &num; }

static Result result = {
    .num = 100,
    .f0 = fun0,
};

Result *fun(void) { return &result; }

fun_t fun1(void) { return fun; }

fun1_t fun2(void) { return fun1; }

void variadic(int count, ...) { (void) count; }

void *custom_alloc(size_t size) {
    (void) size;
    return NULL;
}

void *custom_realloc(void *data, size_t new_size) {
    (void) data;
    (void) new_size;
    return NULL;
}

void custom_free(void *data) { (void) data; }

int main(void) {
    uprintf("Variadic function: %S\n", &variadic);

    uprintf("&fun2: %S\n", &fun2);
    uprintf("fun2(): %S\n", fun2());
    uprintf("fun2()(): %S\n", fun2()());
    uprintf("fun2()()(): %S\n", fun2()()());
    uprintf("fun2()()().num: %S\n", &fun2()()()->num);
    uprintf("fun2()()().f0: %S\n", fun2()()()->f0);
    uprintf("&fun2()()().f0: %S\n", &fun2()()()->f0);
    uprintf("fun2()()().f0(): %S\n", fun2()()()->f0());

    fun_t var_fun = fun;
    fun1_t var_fun1 = fun1;
    fun_t *ptr_fun = &var_fun;

    uprintf("&var_fun: %S\n", &var_fun);
    uprintf("var_fun(): %S\n", var_fun());
    uprintf("var_fun().num: %S\n", &var_fun()->num);
    uprintf("var_fun().f0: %S\n", var_fun()->f0);
    uprintf("&var_fun().f0: %S\n", &var_fun()->f0);
    uprintf("var_fun().f0(): %S\n", var_fun()->f0());
    uprintf("ptr_fun: %S\n", ptr_fun);
    uprintf("(*ptr_fun)(): %S\n", (*ptr_fun)());
    uprintf("(*ptr_fun)().num: %S\n", &(*ptr_fun)()->num);
    uprintf("(*ptr_fun)().f0: %S\n", (*ptr_fun)()->f0);
    uprintf("&(*ptr_fun)().f0: %S\n", &(*ptr_fun)()->f0);
    uprintf("(*ptr_fun)().f0(): %S\n", (*ptr_fun)()->f0());

    Functions functions = {
        .f = fun1,
        .fp = &var_fun1,
    };

    uprintf("&functions: %S\n", &functions);
    uprintf("&functions.f: %S\n", &functions.f);
    uprintf("functions.f(): %S\n", functions.f());
    uprintf("functions.f()(): %S\n", functions.f()());
    uprintf("functions.f()().num: %S\n", &functions.f()()->num);
    uprintf("functions.f()().f0: %S\n", functions.f()()->f0);
    uprintf("&functions.f()().f0: %S\n", &functions.f()()->f0);
    uprintf("functions.f()().f0()(): %S\n", functions.f()()->f0());
    uprintf("functions.fp: %S\n", functions.fp);
    uprintf("(*functions.fp)(): %S\n", (*functions.fp)());
    uprintf("(*functions.fp)()(): %S\n", (*functions.fp)()());
    uprintf("(*functions.fp)()().num: %S\n", &(*functions.fp)()()->num);
    uprintf("(*functions.fp)()().f0: %S\n", (*functions.fp)()()->f0);
    uprintf("&(*functions.fp)()().f0: %S\n", &(*functions.fp)()()->f0);
    uprintf("(*functions.fp)()().f0()(): %S\n", (*functions.fp)()()->f0());

    AllocatorMethods libc = {malloc, realloc, free};
    AllocatorMethods custom = {custom_alloc, custom_realloc, custom_free};
    uprintf("Allocator methods: %S\n%S\n", &libc, &custom);

#ifndef __clang__
    uprintf("libc functions: %S, %S, %S\n", &malloc, &printf, &fread);
#endif

    return _upf_test_status;
}
