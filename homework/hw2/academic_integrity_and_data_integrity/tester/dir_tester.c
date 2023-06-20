#include "dir_tester.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool check_success(struct dir_tester *self, bool success, bool result,
                          char *success_tip, char *failed_tip);
struct dir_tester *dir_tester_create(char *name) {
  struct dir_tester *self = calloc(1, sizeof(struct dir_tester));
  strncpy(self->name, name, DIR_TESTER_NAME_L);
  self->error = 0;
  printf("=== dir test %s: START ===\n", self->name);
  self->inner_dir = dir_new(name);
  return self;
}
void dir_tester_destory(struct dir_tester *self) {
  if (self->inner_dir) {
    dir_release(self->inner_dir);
  }
  if (self->error) {
    printf("=== FAILED ===\n");
  } else {
    printf("=== PASS ===\n");
  }
  free(self);
}

void dir_tester_find_node(struct dir_tester *self, const char *name,
                          bool success) {
  if (self->error)
    return;
  bool result = !dir_find_node(self->inner_dir, name);
  if (!check_success(self, success, result, "exists", "non-exists"))
    return;
  printf("exists.\n");
}
void dir_tester_add_file(struct dir_tester *self, int type, char *name,
                         bool success) {
  if (self->error)
    return;
  printf("add file...");
  bool result = dir_add_file(self->inner_dir, type, name);
  if (!check_success(self, success, result, "success", "error"))
    return;
  self->error = 1;
  struct node *node = dir_find_node(self->inner_dir, name);
  if (!node) {
    printf("\nfailed: file does not exist.\n");
    return;
  }
  if (node->is_dir) {
    printf("\nfailed: add dir instead of file.\n");
    return;
  }
  self->error = 0;
  printf("success.\n");
}
void dir_tester_add_subdir(struct dir_tester *self, char *name, bool success) {
  if (self->error)
    return;
  printf("add subdir...");
  bool result = dir_add_subdir(self->inner_dir, name);
  if (!check_success(self, success, result, "success", "error"))
    return;
  self->error = 1;
  struct node *node = dir_find_node(self->inner_dir, name);
  if (!node) {
    printf("\nfailed: dir does not exist.\n");
    return;
  }
  if (!node->is_dir) {
    printf("\nfailed: add file instead of dir.\n");
    return;
  }
  self->error = 0;
  printf("success.\n");
}
void dir_tester_delete(struct dir_tester *self, const char *name,
                       bool success) {
  printf("remove subordinate...");
  bool result = dir_delete(self->inner_dir, name);
  if (!check_success(self, success, result, "success", "error"))
    return;
  struct node *node = dir_find_node(self->inner_dir, name);
  if (node) {
    self->error = 1;
    printf("\nfailed: subordinate still exists.\n");
    return;
  }
  printf("success.\n");
}

void dir_tester_size(struct dir_tester *self, int size) {
  printf("checking size...");
  if (self->inner_dir->size != size) {
    printf("\nfailed: wrong size.\nsize: %d, expected: %d",
           self->inner_dir->size, size);
    return;
  }
  printf("success.\n");
}

void dir_tester_capacity(struct dir_tester *self, int capacity) {
  printf("checking capcity...");
  if (self->inner_dir->capacity != capacity) {
    printf("\nfailed: wrong capacity.\napacity: %d, expected: %d",
           self->inner_dir->capacity, capacity);
    return;
  }
  printf("success.\n");
}

static bool check_success(struct dir_tester *self, bool success, bool result,
                          char *success_tip, char *failed_tip) {

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
