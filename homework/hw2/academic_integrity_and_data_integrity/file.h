#ifndef __CS110_HW2_FILE__
#define __CS110_HW2_FILE__

#include "node.h"

#define DEFAULT_FILE_SIZE 0

/* A file is an object for data storage.*/
struct file {
  int type;          /* file type supported by the explore */
  int size;          /* number of bytes stored in the file */
  char *data;        /* data stored in byte array */
  struct node *base; /* associate node struct */
};

/* @typedef: `open_func`
 * @type: `(struct file *) -> void`
 * @description: Functions used to open certain file type.
 * Each file type is associated with one function of `open_func`.
 * type which is called when file of this type is opened by the explore. */
typedef void (*open_func)(struct file *);

/* @function: `file_new`
 * @parameter: `type: int` The type of the file.
 * @parameter: `name: char *` The name of the file.
 * @return: The newly created file.
 *
 * @description: Allocate a piece of memory and create a file object on it.
 * The file size should be `DEFAULT_FILE_SIZE` and the data should be zerod.
 * The file type is specified by the `type` parameter.
 * If any step of the procedure fails, return `NULL`.
 * Also deallocate the acquired memory on error to prevent leaking.
 * Return a pointer to the newly created file if success. */
struct file *file_new(int type, char *name);

/* @function: `file_release`
 * @parameter: `file: struct file *` Self.
 * @return: `void`
 *
 * @description: Release the memory allocated for a file object.
 * Handle `NULL` pointers properly: do nothing.
 * Note: file objects are embeded in node objects, and `node_release` shall call
 * `file_release`. */
void file_release(struct file *file);

/* @function: `file_write`
 * @parameter: `file: struct file *` Self.
 * @parameter: `offset: int` The offset of the file.
 * @parameter: `bytes: int` The number of bytes written to the file.
 * @parameter: `buf: const char *` The source of the data.
 * @return: `true` on success and `false` on error.
 *
 * @description: Overwrite the file data within the range from `offset` to
 * `offset + bytes` (exclusive) with the data from `buf`.
 * Enlarge the file size if `offset + bytes` exceed current file size.
 * The newly enlarged file should have size exactly equal to `offset + bytes`.
 *
 * Do nothing if `offset` is negative and return `false`.
 * Handle `NULL` pointers properly: do nothing, return `false`.*/
bool file_write(struct file *file, int offset, int bytes, const char *buf);

/* @function: `file_read`
 * @parameter: `file: struct file *` Self.
 * @parameter: `offset: int` The offset of the file.
 * @parameter: `bytes: int` The number of bytes to read.
 * @parameter: `buf: char *` The buffer to store the bytes read out.
 * @return: `true` on success and `false` on error.
 *
 * @description: Read the file data within the range from `offset` to
 * `offset + bytes` (exclusive) to `buf`.
 *
 * Do nothing if `offset` is negative and return `false`.
 * Do nothing if `offset + bytes` exceed the current file size and return
 * `false`.
 * Handle `NULL` pointers properly: do nothing, return `false`.*/
bool file_read(const struct file *file, int offset, int bytes, char *buf);

#endif
