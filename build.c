#include "yeb.h"

void cc(Cmd *cmd) { CMD_APPEND(cmd, "clang"); }

void cflags(Cmd *cmd) { CMD_APPEND(cmd, "-Wall", "-Wextra"); }

Cmd build_test() {
  Cmd cmd = {0};
  cc(&cmd);
  cflags(&cmd);
  CMD_APPEND(&cmd, "main.c", "-o", "test");
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
