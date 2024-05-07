#include "yeb.h"

bool is_release = false;

void cc(Cmd *cmd) { CMD_APPEND(cmd, "cc"); }

void cflags(Cmd *cmd) {
  CMD_APPEND(cmd, "-Wall", "-Wextra", "--std=gnu2x");
  if (is_release)
    CMD_APPEND(cmd, "-O2");
  else
    CMD_APPEND(cmd, "-O1", "-g", "-DDEBUG");
}

Cmd build_test_main() {
  Cmd cmd = {0};
  cc(&cmd);
  cflags(&cmd);
  CMD_APPEND(&cmd, "test_main.c", "-o", "test");
  return cmd;
}

DECL_DA_STRUCT(int, Ints);

int main(int argc, char **argv) {
  yeb_bootstrap();
  for (int i = 0; i < argc; ++i) {
    if (strcmp(argv[i], "--release") == 0) {
      is_release = true;
    }
  }

  execute(build_test_main());
  return 0;
}
