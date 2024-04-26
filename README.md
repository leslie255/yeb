# YEB

**Build system of C, by C, for C**

Bootstrapped, zero install build system for C inspired by [nobuild](https://github.com/tsoding/nobuild?tab=readme-ov-file).

## Getting started

- Yeb does not require installing anything other than a C compiler
- Download `yeb.h` into your project directory (henceforth referred to as `project/`) from the [Releases](https://github.com/leslie255/yeb/releases) page (Do not use the one in the repo! It's not stable!).
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
