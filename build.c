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
  yeb_bootstrap();
  for (int i = 1; i < argc; ++i) {
    printf("%s\n", argv[i]);
  }
  Cmd cmd = {0};
  cc(&cmd);
  cflags(&cmd);
  execute(build_test_main());
  return 0;
}
