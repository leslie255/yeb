#include "yeb.h"

void cc(Cmd *cmd) { CMD_APPEND(cmd, "clang"); }

void cflags(Cmd *cmd) { CMD_APPEND(cmd, "-Wall", "-Wextra"); }

Cmd build_test_main() {
  Cmd cmd = {0};
  cc(&cmd);
  cflags(&cmd);
  CMD_APPEND(&cmd, "test_main.c", "-o", "test");
  return cmd;
}

int main(int argc, char **argv) {
  // DynString cmd = {0};
  // dynstring_append_cstr(&cmd, "./yeb/a.out");
  // printf("%s\n", cmd.cstr);
  // return 0;

  yeb_bootstrap();
  for (int i = 1; i < argc; ++i) {
    printf("%s\n", argv[i]);
  }
  execute(build_test_main());
  return 0;
}
