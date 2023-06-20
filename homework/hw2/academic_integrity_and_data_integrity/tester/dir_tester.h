#ifndef DIR_TESTER_H
#define DIR_TESTER_H
#include "../dir.h"
#include "../file.h"
#include "../node.h"

#define DIR_TESTER_NAME_L 20
struct dir_tester {
  char name[DIR_TESTER_NAME_L + 1];
  struct directory *inner_dir;
  int error;
};

struct dir_tester *dir_tester_create(char *name);
void dir_tester_destory(struct dir_tester *self);
void dir_tester_find_node(struct dir_tester *self, const char *name,
                          bool success);
void dir_tester_add_file(struct dir_tester *self, int type, char *name,
                         bool success);
void dir_tester_add_subdir(struct dir_tester *self, char *name, bool success);
void dir_tester_delete(struct dir_tester *self, const char *name, bool success);
void dir_tester_size(struct dir_tester *self, int size);
void dir_tester_capacity(struct dir_tester *self, int capacity);

#endif