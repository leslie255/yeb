# YEB

**Build system of C, by C, for C**

Bootstrapped, zero install build system for C inspired by [nobuild](https://github.com/tsoding/nobuild?tab=readme-ov-file).

## Getting started

- Created the project directory, henceforth called `project/`.
- Download `yeb.h` into `project/yeb.h` from Github Release page.
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
    CMD_APPEND(&cmd, "main.c", "-o", "project");
    return cmd;
}

int main() {
    yeb_bootstrap();
    Cmd cmd = {0};
    cc(&cmd);
    cflags(&cmd);
    execute(build_test());
    return 0;
}
```

- Bootstrapping yeb

Yeb is bootstrapped, it generates itself (the `yeb/` directory) from `yeb.h`:
```bash
$ cc build.c && ./a.out
```

- Build and run the project!
```bash
$ yeb/yeb
$ ./project
```
