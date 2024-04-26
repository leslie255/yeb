#pragma once

#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yeb_bootstrap();

#define DA_INIT_CAP 256

#define da_push(DA, ITEM)                                                      \
  do {                                                                         \
    __auto_type DA_ = (DA);                                                    \
    ++DA_->da_len;                                                             \
    if (DA_->da_items == NULL) {                                               \
      DA_->da_cap = DA_INIT_CAP;                                               \
      DA_->da_items = malloc(sizeof(DA_->da_items[0]) * DA_INIT_CAP);          \
      assert(DA_->da_items != NULL);                                           \
    } else if (DA_->da_len > DA_->da_cap) {                                    \
      DA_->da_cap *= 2;                                                        \
      DA_->da_items =                                                          \
          realloc(DA_->da_items, sizeof(DA_->da_items[0]) * DA_->da_cap);      \
      assert(DA_->da_items != NULL);                                           \
    }                                                                          \
    DA_->da_items[DA_->da_len - 1] = (ITEM);                                   \
  } while (0)

#define da_append(DA, ITEMS, N)                                                \
  do {                                                                         \
    __auto_type DA_ = (DA);                                                    \
    size_t n = (N);                                                            \
    size_t i = DA_->da_len;                                                    \
    DA_->da_len += n;                                                          \
    if (DA_->da_items == NULL) {                                               \
      DA_->da_cap = DA_INIT_CAP > n ? DA_INIT_CAP : n;                         \
      DA_->da_items = malloc(sizeof(DA_->da_items[0]) * DA_->da_cap);          \
      assert(DA_->da_items != NULL);                                           \
    } else if (DA_->da_len > DA_->da_cap) {                                    \
      DA_->da_cap *= 2;                                                        \
      DA_->da_cap = DA_->da_cap > DA_->da_len ? DA_->da_cap : DA_->da_len;     \
      DA_->da_items =                                                          \
          realloc(DA_->da_items, sizeof(DA_->da_items[0]) * DA_->da_cap);      \
      assert(DA_->da_items != NULL);                                           \
    }                                                                          \
    memcpy(&DA_->da_items[i], (ITEMS), sizeof(DA_->da_items[0]) * n);          \
  } while (0)

#define da_append_multiple(DA, ...)                                            \
  do {                                                                         \
    __auto_type DA__ = (DA);                                                   \
    typeof((DA__)->da_items[0]) items[] = {__VA_ARGS__};                       \
    da_append(DA__, items, sizeof(items) / sizeof(items[0]));                  \
  } while (0);

#define da_reserve(DA, N)                                                      \
  do {                                                                         \
    __auto_type arr = (DA);                                                    \
    size_t n = (N);                                                            \
    if (arr->da_items == NULL) {                                               \
      arr->da_cap = n;                                                         \
      arr->da_items = malloc(sizeof(arr->da_items[0]) * arr->da_cap);          \
      assert(arr->da_items != NULL);                                           \
    } else if (arr->da_len + n > arr->da_cap) {                                \
      arr->da_cap = arr->da_len + n;                                           \
      arr->da_items =                                                          \
          realloc(arr->da_items, sizeof(arr->da_items[0]) * arr->da_cap);      \
      assert(arr->da_items != NULL);                                           \
    }                                                                          \
  } while (0)

#define DECL_DA_STRUCT(T, NAME)                                                \
  typedef struct NAME {                                                        \
    T *da_items;                                                               \
    size_t da_len;                                                             \
    size_t da_cap;                                                             \
  } NAME;

#define DA_FOR(ARR, IDX, ITEM, BLOCK)                                          \
  __auto_type ARR_ = (ARR);                                                    \
  for (size_t IDX = 0; IDX < ARR_->da_len; ++IDX) {                            \
    __auto_type ITEM = da_get(ARR_, IDX);                                      \
    if (1)                                                                     \
      BLOCK;                                                                   \
  }

#define da_get(ARR, IDX) (&(ARR)->da_items[(IDX)])

#define da_get_checked(ARR, IDX)                                               \
  (assert((IDX) < (ARR)->da_len), &(ARR)->da_items[(IDX)])

DECL_DA_STRUCT(const char *, ConstStrings);

DECL_DA_STRUCT(char, DynString);

typedef struct cmd {
  ConstStrings args;
} Cmd;

#define CMD_APPEND(CMD, ...) da_append_multiple(&(CMD)->args, __VA_ARGS__)

void execute(Cmd cmd);

typedef uint8_t LogLevel;

#define LOG_LEVEL_INFO 0
#define LOG_LEVEL_WARNING 1
#define LOG_LEVEL_ERROR 2

#ifndef LOG_LEVEL
#define LOG_LEVEL LOGLEVEL_INFO
#endif

void lprintf(LogLevel level, const char *restrict fmt, ...);

#ifndef YEB_NO_IMPL

// #undef YEB_INTERNAL
#ifndef YEB_INTERNAL

#define YEB_ERROR_NOT_BOOTSTRAPPED()                                           \
  (printf("You need to call `yeb_bootstrap()` before doing anything in "       \
          "`build.c`\n"),                                                      \
   exit(1))

void execute(Cmd cmd) { YEB_ERROR_NOT_BOOTSTRAPPED(); }

void yeb_bootstrap() {
  FILE *yeb_is_bootstrapped_txt = fopen("yeb/yeb_is_bootstrapped.txt", "r");
  if (yeb_is_bootstrapped_txt != NULL) {
    fclose(yeb_is_bootstrapped_txt);
    printf("Yeb is bootstrapped, now do `./yeb/yeb` to build the project.");
    return;
  }

  int exit_code;
  const char *cmd;

#define yeb_bootstrap_execute(S)                                               \
  printf("$ %s\n", S);                                                         \
  cmd = S;                                                                     \
  exit_code = system(S);                                                       \
  if (exit_code != 0)                                                          \
    goto error;

  yeb_bootstrap_execute("mkdir -p yeb/");
  yeb_bootstrap_execute("cp yeb.h yeb/yeb.h");
  FILE *yeb_c = fopen("yeb/yeb.c", "w");
  assert(yeb_c != NULL);
  fprintf(yeb_c, //
          "#define YEB_INTERNAL\n"
          "#include \"yeb.h\"\n");
  fclose(yeb_c);
  yeb_bootstrap_execute("clang -c yeb/yeb.c -o yeb/yeb.o");
  FILE *run_c = fopen("yeb/run.c", "w");
  assert(run_c != NULL);
  fprintf(
      run_c,
      "#include <stdlib.h>\n"
      "int main() {\n"
      "  int exit_code;\n"
      "  exit_code = system(\"clang -c -DYEB_NO_IMPL build.c -o "
      "yeb/build.o\");\n"
      "  if (exit_code != 0) exit(exit_code);\n"
      "  exit_code = system(\"clang yeb/yeb.o yeb/build.o -o yeb/a.out\");\n"
      "  if (exit_code != 0) exit(exit_code);\n"
      "  exit_code = system(\"./yeb/a.out\");\n"
      "  exit(exit_code);\n"
      "}");
  fclose(run_c);
  yeb_bootstrap_execute("clang yeb/run.c -o yeb/yeb");
  yeb_bootstrap_execute("touch yeb/yeb_is_bootstrapped.txt");
  yeb_is_bootstrapped_txt = fopen("yeb/yeb_is_bootstrapped.txt", "w");
  fprintf(yeb_is_bootstrapped_txt,
          "This file is here so yeb knows it's already been bootstrapped.\n"
          "If you're experiencing problems with yeb, try removing this file "
          "(or the yeb/ directory) and bootstrap again.\n");
  fclose(yeb_is_bootstrapped_txt);
  printf("Yeb is bootstrapped, now do `./yeb/yeb` to build the project.\n");
  exit(0);

error:
  if (exit_code != 0) {
    printf("command `%s` exited with non-zero exit code (%d)", cmd, exit_code);
    exit(1);
  }
}

#else

void yeb_bootstrap(){};

static inline const char *concat_strings_with_space(ConstStrings strings) {
  if (strings.da_len == 0) {
    return "";
  }
  if (strings.da_len == 1) {
    return *da_get(&strings, 0);
  }
  DynString s = {0};
  DA_FOR(&strings, i, s_, {
    size_t len = strlen(*s_);
    da_append(&s, *s_, len);
    da_push(&s, ' ');
  });
  *da_get_checked(&s, s.da_len - 1) = '\0';
  return s.da_items;
}

void execute(Cmd cmd) {
  const char *s = concat_strings_with_space(cmd.args);
  printf("$ %s\n", s);
  system(s);
}

#endif // YEB_INTERNAL

#endif // YEB_NO_IMPL
