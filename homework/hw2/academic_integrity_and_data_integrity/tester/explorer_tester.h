#ifndef EXP_TESTER_H
#define EXP_TESTER_H
#include "../explorer.h"

#define EXP_TESTER_NAME_L 20
struct explorer_tester {
  char name[EXP_TESTER_NAME_L + 1];
  struct explorer *inner_explore;
  int error;
};

struct explorer_tester *explorer_tester_create(char *name);
void explorer_tester_destory(struct explorer_tester *self);
void explorer_tester_cdpar(struct explorer_tester *self, char *name,
                           bool success);
void explorer_tester_chdir(struct explorer_tester *self, char *name,
                           bool success);
void explorer_tester_support_file_type(struct explorer_tester *self, int type,
                                       open_func callback, bool success);
void explorer_tester_open(struct explorer_tester *self, char *name,
                          bool success);
void explorer_tester_read(struct explorer_tester *self, char *name, int offset,
                          int bytes, bool sucess, char *expected);
void explorer_tester_write(struct explorer_tester *self, char *name, int offset,
                           int bytes, const char *buf, bool success);
void explorer_tester_create_file(struct explorer_tester *self, char *name,
                                 int type, bool success);
void explorer_tester_mkdir(struct explorer_tester *self, char *name,
                           bool success);
void explorer_tester_delete(struct explorer_tester *self, char *name,
                            bool success);

#endif
