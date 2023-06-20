#include "file_tester.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct file_tester *file_tester_create(char *name) {
  struct file_tester *tester = calloc(1, sizeof(struct file_tester));
  strncpy(tester->name, name, FILE_TESTER_NAME_L);
  tester->inner_file = NULL;
  tester->error = 0;
  printf("=== file test %s: START ===\n", tester->name);
  return tester;
}

void file_tester_destory(struct file_tester *tester) {
  if (tester->inner_file) {
    file_release(tester->inner_file);
  }
  if (tester->error) {
    printf("=== FAILED ===\n");
  } else {
    printf("=== PASS ===\n");
  }
  free(tester);
}

void file_tester_init(struct file_tester *tester, int type, char *name) {
  if (tester->error)
    return;
  assert(tester->inner_file == NULL);
  printf("Create File with type %d...", type);
  tester->inner_file = file_new(type, name);

  // test error
  if (type < 0) {
    if (tester->inner_file) {
      tester->error = 1;
      printf("\nfailed: expected error.\n");
    } else {
      printf("error.\n");
    }
    return;
  }
  tester->error = 1;
  // test NULL pointer
  if (tester->inner_file == NULL) {
    printf("failed: no file created.\n");
    return;
  }

  // test file type
  if (tester->inner_file->type != type) {
    printf("\nfailed: wrong file type.\ntype: %d, expected %d.\n",
           tester->inner_file->type, type);
    return;
  }

  // test file size
  if (tester->inner_file->size != DEFAULT_FILE_SIZE) {
    printf("\nfailed: wrong file size.\ntype: %d, expected %d.\n",
           tester->inner_file->size, DEFAULT_FILE_SIZE);
    return;
  }

  // test file name
  if (strncmp(tester->inner_file->base->name, name, MAX_NAME_LEN)) {
    printf("\nfailed: wrong file name.\nname: %s\n, expected %s.\n",
           tester->inner_file->base->name, name);
    return;
  }
  tester->error = 0;
  printf("success.\n");
}
void file_tester_write(struct file_tester *tester, int offset, int size,
                       char *buf) {
  int i;
  if (tester->error)
    return;
  printf("write %d bytes at pos %d...", size, offset);
  int expected_size = offset + size > tester->inner_file->size
                          ? offset + size
                          : tester->inner_file->size;

  // test file null
  if (!tester->inner_file) {
    if (file_write(tester->inner_file, offset, size, buf)) {
      tester->error = 1;
      printf("\nfailed: expected error.\n");
    } else {
      printf("error\n");
      return;
    }
  }

  char *expected_data = calloc(expected_size + 1, 1);
  char *readout_data = calloc(expected_size + 1, 1);
  tester->error = 1;

  strncpy(expected_data, tester->inner_file->data, tester->inner_file->size);
  strncpy(expected_data + offset, buf, size);
  // test return value
  if (file_write(tester->inner_file, offset, size, buf) < 0) {
    printf("\nfailed: unexpected error.\n");
    goto test_write_clear;
  }
  if (tester->inner_file->size != expected_size) {
    printf("\nfailed: wrong file size. \nsize: %d, expected: %d\n",
           tester->inner_file->size, expected_size);
    goto test_write_clear;
  }

  // test increase
  if (tester->inner_file->size != expected_size) {
    printf("\nfailed: wrong file size. \nsize: %d, expected: %d\n",
           tester->inner_file->size, expected_size);
    goto test_write_clear;
  }
  if (file_read(tester->inner_file, 0, expected_size, readout_data) < 0) {
    printf("\nfailed: unexpected error");
    goto test_write_clear;
  }

  // test data
  for (i = 0; i < expected_size; i++) {
    if (readout_data[i] != expected_data[i]) {
      printf("\nfailed: wrong datum at %d.\ndatum:%c, expected:%c\n", i,
             readout_data[i], expected_data[i]);
      goto test_write_clear;
    }
  };
  tester->error = 0;
  printf("success.\n");
test_write_clear:
  free(readout_data);
  free(expected_data);
}
void file_tester_read(struct file_tester *tester, int offset, int size,
                      char *expected) {
  int i = 0;
  if (tester->error)
    return;
  printf("read %d bytes at pos %d...", size, offset);
  // test file null
  if (!tester->inner_file) {
    if (file_read(tester->inner_file, offset, size, NULL)) {
      tester->error = 1;
      printf("\nfailed: expected error.\n");
    } else {
      printf("error\n");
    }
    return;
  }
  char *readout_data = calloc(size + 1, 1);
  // test read too much data
  if (offset + size > tester->inner_file->size) {
    if (file_read(tester->inner_file, offset, size, readout_data)) {
      tester->error = 1;
      printf("\nfailed: expected error.\n");
    } else {
      printf("error\n");
    }
    goto test_read_clear;
  }
  tester->error = 1;
  // test read normal
  if (!file_read(tester->inner_file, offset, size, readout_data)) {
    printf("\nfailed: unexpected error.\n");
    goto test_read_clear;
  }
  // test data
  for (i = 0; i < size; i++) {
    if (readout_data[i] != expected[i]) {
      printf("\nfailed: wrong datum at %d.\ndatum:%c, expected:%c\n",
             offset + i, readout_data[i], expected[i]);
      goto test_read_clear;
    }
  };
  tester->error = 0;
  printf("success\n");
test_read_clear:
  free(readout_data);
}
