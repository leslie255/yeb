# YEB

Bootstrapped, zero install build system for C inspired by
[nobuild](https://github.com/tsoding/nobuild?tab=readme-ov-file).

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
statements in makefile is less cumbersome than the cumbersomeness of dealing with shell commands in C (which I can
provide tools to make somewhat streamlined).

Also yeb requires nothing on top of a C compiler (which you need anyways for a C project).

## So how do I ship my source code that uses yeb for build system?

You just ship the `yeb.h` and `build.c` in the project root.

## It's not aimed for big projects

This is just a thing I made for my own side projects, if you're working on a production project with a dozen
dependencies managed by CMAKE, this is not for you.


