#include "explorer_tester.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool check_success(struct explorer_tester *self, bool success,
                          bool result, char *success_tip, char *failed_tip);
struct explorer_tester *explorer_tester_create(char *name) {
  struct explorer_tester *self = calloc(1, sizeof(struct explorer_tester));
  strncpy(self->name, name, EXP_TESTER_NAME_L);
  self->inner_explore = explorer_new();
  self->error = 0;
  printf("=== explorer test %s: START ===\n", self->name);
  return self;
}
void explorer_tester_destory(struct explorer_tester *self) {
  if (self->inner_explore) {
    explorer_release(self->inner_explore);
  }
  if (self->error) {
    printf("=== FAILED ===\n");
  } else {
    printf("=== PASS ===\n");
  }
  free(self);
}

void explorer_tester_chdir(struct explorer_tester *self, char *name,
                           bool success) {
  if (self->error) {
    return;
  }
  printf("change dir...");
  int result = explorer_chdir(self->inner_explore, name);

  if (!check_success(self, success, result, "success", "error"))
    return;
  if (strncmp(self->inner_explore->cwd->base->name, name, MAX_NAME_LEN)) {
    self->error = 1;
    printf("\nfailed: wrong cwd.\ncwd:%s\n, expected:%s\n",
           self->inner_explore->cwd->base->name, name);
    return;
  }
  printf("success.\n");
}
void explorer_tester_support_file_type(struct explorer_tester *self, int type,
                                       open_func callback, bool success) {
  if (self->error)
    return;
  printf("add support file type...");
  bool result = explorer_support_filetype(self->inner_explore, callback, type);
  if (!check_success(self, success, result, "success", "error"))
    return;
  printf("success.\n");
}

void explorer_tester_cdpar(struct explorer_tester *self, char *name,
                           bool success) {
  if (self->error)
    return;
  printf("entering parent...");
  bool result = explorer_cdpar(self->inner_explore);
  if (!check_success(self, success, result, "success", "error"))
    return;
  if (strncmp(self->inner_explore->cwd->base->name, name, MAX_NAME_LEN)) {
    printf("\nfailed: wrong cwd.\ncwd: %s\nexpected:%s\n",
           self->inner_explore->cwd->base->name, name);
    self->error = 1;
    return;
  }
  printf("success.\n");
}
void explorer_tester_open(struct explorer_tester *self, char *name,
                          bool success) {
  if (self->error)
    return;
  printf("open file %s...\n", name);
  bool result = explorer_open(self->inner_explore, name);
  if (!check_success(self, success, result, "success", "failed"))
    return;
  printf("success.\n");
}
void explorer_tester_read(struct explorer_tester *self, char *name, int offset,
                          int bytes, bool success, char *expected) {
  if (self->error)
    return;
  printf("read file...");
  char *buf = calloc(bytes + 1, 1);
  bool result = explorer_read(self->inner_explore, name, offset, bytes, buf);
  if (!check_success(self, success, result, "success", "error"))
    goto clear;
  if (strncmp(buf, expected, bytes)) {
    printf("\nfailed: wrong answer.\nresult: %s\nexpected:%s\n", buf, expected);
    self->error = 1;
    goto clear;
  }
  printf("success.\n");
clear:
  free(buf);
}
void explorer_tester_write(struct explorer_tester *self, char *name, int offset,
                           int bytes, const char *buf, bool success) {
  if (self->error)
    return;
  printf("write file...");
  bool result = explorer_write(self->inner_explore, name, offset, bytes, buf);
  if (!check_success(self, success, result, "success", "error")) {
    return;
  }
  printf("success.\n");
}
void explorer_tester_create_file(struct explorer_tester *self, char *name,
                                 int type, bool success) {
  if (self->error)
    return;
  printf("create file...");
  bool result = explorer_create(self->inner_explore, name, type);
  if (!check_success(self, success, result, "success", "error"))
    return;
  if (!explorer_contain(self->inner_explore, name)) {
    self->error = 1;
    printf("\nfailed: does not contain the file.\n");
    return;
  }
  printf("success.\n");
}
void explorer_tester_mkdir(struct explorer_tester *self, char *name,
                           bool success) {
  if (self->error)
    return;
  printf("create dir...");
  bool result = explorer_mkdir(self->inner_explore, name);
  if (!check_success(self, success, result, "success", "error"))
    return;
  if (!explorer_contain(self->inner_explore, name)) {
    self->error = 1;
    printf("\nfailed: does not contain the file.\n");
    return;
  }
  printf("success.\n");
}
void explorer_tester_delete(struct explorer_tester *self, char *name,
                            bool success) {
  {
    if (self->error)
      return;
    printf("delete file or dir...");
    bool result = explorer_delete(self->inner_explore, name);
    if (!check_success(self, success, result, "success", "error"))
      return;
    if (explorer_contain(self->inner_explore, name)) {
      self->error = 1;
      printf("\nfailed: still contain the file.\n");
      return;
    }
    printf("success.\n");
  }
}
void explorer_tester_search_recursive(struct explorer_tester *self,
                                      const char *name) {
  if (self->error)
    return;
  printf("open the file...\n");
  explorer_open(self->inner_explore, name);
}

static bool check_success(struct explorer_tester *self, bool success,
                          bool result, char *success_tip, char *failed_tip) {

  if (success != result) {
    if (result) {
      printf("\nfailed.\nresult: %s, expected : %s.\n", success_tip,
             failed_tip);
    } else {
      printf("\nfailed.\nresult: %s, expected : %s.\n", failed_tip,
             success_tip);
    }
    self->error = 1;
    return false;
  }
  if (!result) {
    printf("%s.\n", failed_tip);
    return false;
  }
  return true;
}
