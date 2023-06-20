#ifndef FILE_TESTER_H
#define FILE_TESTER_H
#include "../file.h"

#define FILE_TESTER_NAME_L 20
struct file_tester {
  char name[FILE_TESTER_NAME_L];
  struct file *inner_file;
  int error;
};

struct file_tester *file_tester_create(char *test_name);
void file_tester_destory(struct file_tester *tester);
void file_tester_init(struct file_tester *tester, int type, char *name);
void file_tester_write(struct file_tester *tester, int offset, int size,
                       char *buf);
void file_tester_read(struct file_tester *tester, int offset, int size,
                      char *expected);
#endif
