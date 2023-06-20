#include "bool.h"
#include "dir.h"
#include "explorer.h"
#include "file.h"
#include "tester/dir_tester.h"
#include "tester/explorer_tester.h"
#include "tester/file_tester.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void fill_random(char *buf, int size) {
  int i = 0;
  for (i = 0; i < size; i++) {
    if (rand() % 2) {
      buf[i] = 'A' + rand() % 26;
    } else {
      buf[i] = 'a' + rand() % 26;
    }
  }
}

/* file tester */
const int FILE_SIZE = 10;
void test_file_init() {
  struct file_tester *tester = file_tester_create("init");
  file_tester_init(tester, 0, "a");
  file_tester_destory(tester);
}

void test_file_write_inbound() {
  char buf_write[FILE_SIZE + 1];
  fill_random(buf_write, FILE_SIZE);

  struct file_tester *tester = file_tester_create("write inbound");
  file_tester_init(tester, 0, "b");
  file_tester_write(tester, 0, FILE_SIZE, buf_write);
  file_tester_read(tester, 0, FILE_SIZE, buf_write);
  file_tester_destory(tester);
}

void test_file_write_outbound() {
  /* Initialization */
  const int WRITE_OFFSET = 5;
  char buf_write[FILE_SIZE + WRITE_OFFSET + 1];
  fill_random(buf_write, FILE_SIZE);

  struct file_tester *tester = file_tester_create("write outbound");
  file_tester_init(tester, 0, "c");
  file_tester_write(tester, WRITE_OFFSET, FILE_SIZE, buf_write);
  file_tester_read(tester, WRITE_OFFSET, FILE_SIZE, buf_write);
  file_tester_destory(tester);
}

void test_file_read_inbound() {
  struct file_tester *tester = file_tester_create("read inbound");
  file_tester_init(tester, 0, "read inbound");
  file_tester_write(tester, 0, 10, "abcdefghij");
  file_tester_read(tester, 0, 3, "abc");
  file_tester_read(tester, 7, 3, "hij");
  file_tester_read(tester, 10, 0, "");
  file_tester_destory(tester);
}

void test_file_read_outbound() {
  struct file_tester *tester = file_tester_create("read outbound");
  file_tester_init(tester, 0, "a");
  file_tester_write(tester, 0, 10, "abcdefghij");
  file_tester_read(tester, 11, 11, "");
  file_tester_read(tester, 11, 0, "");
  file_tester_read(tester, 0, 11, "");
  file_tester_destory(tester);
}

void test_file_read_write() {
  struct file_tester *tester = file_tester_create("read write");
  file_tester_init(tester, 0, "read iobound");
  file_tester_write(tester, 0, 3, "abc");
  file_tester_read(tester, 0, 3, "abc");
  file_tester_read(tester, 3, 3, "");
  file_tester_write(tester, 3, 3, "def");
  file_tester_read(tester, 3, 3, "def");
  file_tester_destory(tester);
}

/* dir tests*/

void test_dir_add_file() {
  struct dir_tester *tester = dir_tester_create("add file");
  dir_tester_add_file(tester, 1, "a", true);
  dir_tester_add_file(tester, 1, "a", false);
  dir_tester_destory(tester);
}

void test_dir_add_dir() {
  struct dir_tester *tester = dir_tester_create("add dir");
  dir_tester_add_subdir(tester, "a", true);
  dir_tester_add_file(tester, 1, "a", false);
  dir_tester_add_subdir(tester, "a", false);
  dir_tester_destory(tester);
}

void test_dir_add_delete() {
  struct dir_tester *tester = dir_tester_create("add delete");
  dir_tester_add_subdir(tester, "a", true);
  dir_tester_add_file(tester, 1, "a", false);
  dir_tester_delete(tester, "a", true);
  dir_tester_add_file(tester, 1, "a", true);
  dir_tester_add_subdir(tester, "a", false);
  dir_tester_delete(tester, "a", true);
  dir_tester_delete(tester, "a", false);
  dir_tester_destory(tester);
}

void test_dir_size() {
  int i = 0;
  struct dir_tester *tester = dir_tester_create("size");
  char name[] = "a";
  for (i = 0; i < 16; i++) {
    dir_tester_add_file(tester, 1, name, true);
    name[0]++;
  }
  dir_tester_size(tester, 16);
  dir_tester_capacity(tester, 16);
  dir_tester_add_file(tester, 1, name, true);
  dir_tester_size(tester, 17);
  dir_tester_capacity(tester, 32);
  name[0] = 'a';
  for (i = 0; i < 14; i++) {
    dir_tester_delete(tester, name, true);
    name[0]++;
  }
  dir_tester_size(tester, 3);
  dir_tester_capacity(tester, 32);

  dir_tester_destory(tester);
}

/* explorer tests */
#define CHECKSUM_BYTES 16
static int checksum[CHECKSUM_BYTES];
static void checksum_reset() { memset(checksum, 0, sizeof(checksum)); }
static void checksum_test(int *err) {
  int i = 0;
  for (i = 0; i < CHECKSUM_BYTES; i++)
    if (checksum[i] != 0)
      *err = 1;
}
static void checksum_upd_bytes(const char *bytes, int n) {
  int i = 0;
  for (i = 0; i < n; i++)
    checksum[i % 8] ^= bytes[i];
}
static void checksum_upd_file(struct file *file) {
  checksum_upd_bytes(file->data, file->size);
}

void test_explorer_init() {
  struct explorer_tester *tester = explorer_tester_create("init");
  explorer_tester_destory(tester);
}

void test_explorer_file1() {
  char buf_w[1000];
  struct explorer_tester *tester = explorer_tester_create("file-io");
  explorer_tester_create_file(tester, "plaintext", 3, true);
  explorer_tester_create_file(tester, "plaintext", 4, false);

  fill_random(buf_w, 1000);
  explorer_tester_write(tester, "plaintext", 3, 1000, buf_w, true);
  explorer_tester_read(tester, "plaintext", 3, 1000, true, buf_w);

  explorer_tester_destory(tester);
}
void test_explorer_file2() {
  const int len_a = 100, len_b = 1000, len_c = 3000;
  char buf_a[len_a], buf_b[len_b], buf_c[len_c];
  struct explorer_tester *tester = explorer_tester_create("file-open");

  checksum_reset();
  fill_random(buf_a, len_a), checksum_upd_bytes(buf_a, len_a);
  fill_random(buf_b, len_b), checksum_upd_bytes(buf_b, len_b);
  fill_random(buf_c, len_c), checksum_upd_bytes(buf_c, len_c);

  explorer_tester_support_file_type(tester, 1, checksum_upd_file, true);
  explorer_tester_support_file_type(tester, 2, checksum_upd_file, true);
  explorer_tester_support_file_type(tester, 3, checksum_upd_file, true);
  explorer_tester_create_file(tester, "A", 1, true);
  explorer_tester_create_file(tester, "B", 2, true);
  explorer_tester_create_file(tester, "C", 3, true);
  explorer_tester_write(tester, "A", 11 * CHECKSUM_BYTES, len_a, buf_a, true);
  explorer_tester_write(tester, "B", 22 * CHECKSUM_BYTES, len_b, buf_b, true);
  explorer_tester_write(tester, "C", 33 * CHECKSUM_BYTES, len_c, buf_c, true);
  explorer_tester_open(tester, "A", true);
  explorer_tester_open(tester, "B", true);
  explorer_tester_open(tester, "C", true);

  checksum_test(&tester->error);

  explorer_tester_destory(tester);
}
void test_explorer_dir1() {
  struct explorer_tester *tester = explorer_tester_create("dir-simple");

  explorer_tester_mkdir(tester, "AAAAAAA", true);
  explorer_tester_mkdir(tester, "AAAAAAA", false);
  explorer_tester_mkdir(tester, "BBBBBBBBBBB", true);
  explorer_tester_mkdir(tester, "BBBBBBBBBBB", false);

  explorer_tester_cdpar(tester, NULL, false);
  explorer_tester_chdir(tester, "AAAAAAA", true);
  explorer_tester_chdir(tester, "XXX", false);
  explorer_tester_cdpar(tester, "root", true);

  explorer_tester_destory(tester);
}

void test_explorer_dir2() {
  struct explorer_tester *tester = explorer_tester_create("dir-simple");

  explorer_tester_mkdir(tester, "AAAAAAA", true);
  explorer_tester_mkdir(tester, "AAAAAAA", false);
  explorer_tester_mkdir(tester, "BBBBBBBBBBB", true);
  explorer_tester_mkdir(tester, "BBBBBBBBBBB", false);

  explorer_tester_cdpar(tester, NULL, false);
  explorer_tester_chdir(tester, "AAAAAAA", true);
  explorer_tester_chdir(tester, "XXX", false);
  explorer_tester_cdpar(tester, "root", true);

  explorer_tester_delete(tester, "A", false);
  explorer_tester_delete(tester, "AAAAAAA", true);
  explorer_tester_delete(tester, "BBBBBBBBBBB", true);

  explorer_tester_destory(tester);
}

static char *expected_path[] = {
    "/root/X/lilly",
    "/root/X/A/lilly",
    "/root/X/B/lilly",
    "/root/X/B/C/lilly",
};
static int search_err = 0;
static void search_check_path(char *path) {
  int i = 0;
  for (i = 0; i < 4; i++) {
    if (path && strcmp(expected_path[i], path) == 0)
      return;
  }
  search_err = 1;
}
static void search_callback(char *path, struct file *file) {
  printf("path: %s\n", path);
  search_check_path(path);
  checksum_upd_file(file);
}

void test_explorer_search() {
  struct explorer_tester *tester = explorer_tester_create("recursive-search");

  checksum_reset();
  char buf[373];

  explorer_tester_mkdir(tester, "X", true);
  explorer_tester_chdir(tester, "X", true);
  { // in X
    explorer_tester_create_file(tester, "lilly", 0, true);
    fill_random(buf, 373), checksum_upd_bytes(buf, 373);
    explorer_tester_write(tester, "lilly", 2 * CHECKSUM_BYTES, 373, buf, true);

    explorer_tester_create_file(tester, "pili", 0, true);
    fill_random(buf, 373);
    explorer_tester_write(tester, "pili", 0, 373, buf, true);

    explorer_tester_create_file(tester, "wala", 0, true);
    fill_random(buf, 373);
    explorer_tester_write(tester, "wala", 0, 373, buf, true);

    explorer_tester_mkdir(tester, "A", true);
    explorer_tester_chdir(tester, "A", true);
    { // in root/X/A
      explorer_tester_create_file(tester, "lilly", 0, true);
      fill_random(buf, 373), checksum_upd_bytes(buf, 373);
      explorer_tester_write(tester, "lilly", 2 * CHECKSUM_BYTES, 373, buf,
                            true);
    }
    explorer_tester_cdpar(tester, "X", true);

    explorer_tester_mkdir(tester, "B", true);
    explorer_tester_chdir(tester, "B", true);
    { // in root/X/B
      explorer_tester_create_file(tester, "lilly", 0, true);
      fill_random(buf, 373), checksum_upd_bytes(buf, 373);
      explorer_tester_write(tester, "lilly", 3 * CHECKSUM_BYTES, 373, buf,
                            true);

      explorer_tester_mkdir(tester, "C", true);
      explorer_tester_chdir(tester, "C", true);
      { // in root/X/B/C
        explorer_tester_create_file(tester, "lilly", 0, true);
        fill_random(buf, 373), checksum_upd_bytes(buf, 373);
        explorer_tester_write(tester, "lilly", 4 * CHECKSUM_BYTES, 373, buf,
                              true);
      }
      explorer_tester_cdpar(tester, "B", true);
    }
    explorer_tester_cdpar(tester, "X", true);
  }
  explorer_tester_cdpar(tester, "root", true);

  explorer_search_recursive(tester->inner_explore, "lilly", search_callback);
  tester->error |= search_err;

  explorer_tester_destory(tester);
}

int main() {
  test_file_write_inbound();
  test_file_write_outbound();
  test_file_read_inbound();
  test_file_read_outbound();
  test_file_read_write();

  test_dir_add_file();
  test_dir_add_dir();
  test_dir_add_delete();
  test_dir_size();

  test_explorer_init();
  test_explorer_file1();
  test_explorer_file2();
  test_explorer_dir1();
  // test_explorer_dir2();
  test_explorer_search();
  return 0;
}
