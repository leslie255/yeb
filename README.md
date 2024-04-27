# YEB

Bootstrapped, zero install build system for C.

inspired by [nobuild](https://github.com/tsoding/nobuild?tab=readme-ov-file).

## Getting started

- Yeb does not require installing anything other than a C compiler
- Download `yeb.h` into your project directory (henceforth referred to as `project/`) from the
[Releases](https://github.com/leslie255/yeb/releases) page (Do not use the one in the repo! It's not stable!).
- Create `project/main.c`:
```c
#include <stdio.h>

int main() {
    printf("hello, world\n");
    return 0;
}
```

- Create `project/build.c`, the build description file for yeb:
```c
#include "yeb.h"

void cc(Cmd *cmd) {
    CMD_APPEND(cmd, "clang");
}

void cflags(Cmd *cmd) {
    CMD_APPEND(cmd, "-Wall", "-Wextra");
}

Cmd build_test() {
    Cmd cmd = {0};
    cc(&cmd);
    cflags(&cmd);
    CMD_APPEND(&cmd, "main.c", "-o", "bin/project");
    return cmd;
}

int main() {
    yeb_bootstrap();
    execute(build_test());
    return 0;
}
```

- Bootstrapping yeb

Yeb is bootstrapped, it generates itself (the `yeb/` directory) from `yeb.h`.

This needs to be done every time someone build the project for the first time, but not for the second time onwards.

```bash
$ cc build.c && ./a.out
$ rm -f a.out
```

- Build and run the project!
```bash
$ mkdir bin/
$ yeb/yeb
$ ./bin/project
```

- Put these into `.gitignore`:
```
yeb/
bin/
a.out
```

### So how do I ship my source code that uses yeb for build system?

You just ship the `yeb.h` and `build.c` in the project root.

## Dynamic array and string API

`yeb.h` provides `da_`/`DA_` macros for working with dynamic arrays:

```c
// da_ macros work on any struct with `type *da_items`, `size_t da_len`, `size_t da_cap` fields:

DECL_DA_STRUCT(int, IntArray);
// This is equivalent to:
// typedef struct IntArray {
//     int *dat_items;
//     size_t dat_len;
//     size_t dat_cap;
// } IntArray;

Ints xs = {0};                          // Use `{0}` for initialization.
da_push(&xs, 10);                       // `da_push` for adding one item
int arr[] = {20, 30, 40};
da_append(&xs, arr, 3);                 // `da_append` to add elements from slice.
da_append_multiple(&xs, 50, 60);        // `da_append_multiple` for a shorthand of `da_append`.
DA_FOR(&xs, i, x, {                     // `DA_FOR` for iterating through elements.
    assert(*da_get(&xs, i) == *x);      // Use `da_get` or `da_get_checked` to get an element in array.
    assert(*da_get_checked(&xs, i) == *x);
    printf("%d\n", *x);                 // Note that you have to use `*x` to get an element from array in `DA_FOR`!
});
da_insert(&xs, 1, 15);                  // `da_insert` to insert an element.
da_insert(&xs, 2, 16);
da_remove_last(&xs);                    // `da_remove_last` to remove last item.
da_remove(&xs, 1);                      // `da_remove` to remove element at index.
printf("-------------\n");
DA_FOR(&xs, i, x, printf("%d\n", *x));  // `DA_FOR` can also be used like this for single expressions!
```

`yeb.h` also provides the `DynString` type and `dynstring_` functions/macros for working with dynamic strings:

```c
DynString s = dynstring_new();          // Unlike dynamic arrays, you have to use `dynstring_new` instead of `{0}`.
                                        // for initialization.
                                        // This is because `DynString` always keeps a null character at the end so even
                                        // empty strings have allocation.
dynstring_append_literal(&s, "hello, ");
const char *world = "world";
dynstring_append_cstr(&s, world);     
dynstring_push(&s, '\n');     
printf("%c\n", *dynstring_get(&s, 0));
printf("%c\n", *dynstring_get_checked(&s, 1));
// `DynString` always keeps a null character from behind, which is why you can just use `.cstr` for getting the C string.
printf("%s\n", s.cstr);
```

## In what way is this better than Make?

Makefiles are good for scripting, but it sucks when dealing with variables and functions, for example, this is what you
do in a makefile to discriminate between operating systems:

```makefile
ifeq ($(OS),Windows_NT)
    # Windows specific things
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        # Linux specific things
    else
        ifeq ($(UNAME_S),Darwin)
            # MacOS specific things
        else
            $(error Unsupported OS!)
        endif
    endif
endif
```

And this is what you do in C:

```c
#if defined(__WIN32__)
// Windows specific things.
#elif __linux__
// Linux specific things.
#elif __apple__
// MacOS specific things.
#else
printf("Unsupported OS!")
exit(1);
#end
```

So the argument I'm making here is simply that the cumbersomeness of dealing with variables, functions and control
statements in makefile is less cumbersome than the cumbersomeness of dealing with shell commands in C (which can be
made more or less streamlined with tooling).

Also yeb requires nothing on top of a C compiler (which you need anyways for a C project).

## It's not aimed for big projects

If you're working on a production project with a dozen dependencies managed by CMAKE, this isn't for you. This is just
a thing I made for my own side projects, you can use it on yours if you're interested.

# LICENSE

This project is licensed under 2-Clause BSD.
