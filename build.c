#include "yeb.h"

int main() {
  yeb_bootstrap();
  execute(CMD("echo", "\"hello, world!\""));
  return 0;
}
