#include "file.h"
#include "explorer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* header files */
// construct a new file
struct file *file_new(int type, char *name) {
  /* Initialization */
  struct file *file = NULL;
  /* Check for integrity */
  if (!name || type < 0 || type >= MAX_FT_NO) {
    return NULL;
  }
  /* Allocate memory and initialze the file. */
  file = calloc(1, sizeof(struct file));
  file->type = type;
  file->size = DEFAULT_FILE_SIZE;
  file->data = calloc(file->size, 1);
  /* Crate associtate node and set it to base. */
  file->base = node_new(false, name, file);
  return file;
}

// release the file's pointer memory
void file_release(struct file *file) {
  /* Check for null pointer. */
  if (!file) {
    return;
  }
  /* relase the data. */
  /* Check if base has already been released. Prevent circular call. */
  if (file->base) {
    file->base->inner.file = NULL;
    node_release(file->base);
  }
  /* Release the resources. */
  free(file->data);
  free(file);
}

// write [offset, offset + bytes] from buf to file
bool file_write(struct file *file, int offset, int bytes, const char *buf) {
  /* YOUR CODE HERE */
  /* printf("NOT IMPLEMENTED\n"); */
  if(offset < 0)/* `offset` is negative */
    return false;
  if(bytes < 0)/* `offset + bytes` exceed the current file size */
    return false;
  if(!file)/* Handle `NULL` pointers properly */
    return false;
  if(!buf)/* Handle `NULL` pointers properly */
    return false;
  /*  return `false` if `offset` is negative or handle `NULL` pointers properly*/

  if(file->size < offset + bytes)
  {
    file->data = realloc(file->data, sizeof(char)*(offset + bytes));
    file->size = offset + bytes;
  }
  /* Enlarge the file size if `offset + bytes` exceed current file size */

  for(int i = 0; i < bytes; ++i)
    file->data[i + offset] = buf[i];
  /* [ `offset` , `offset` + bytes ) */
  return true;
}

// write [offset, offset + bytes] from file to buf
bool file_read(const struct file *file, int offset, int bytes, char *buf) {
  /* YOUR CODE HERE */
  /* printf("NOT IMPLEMENTED\n"); */
  if(!file)
    return false;/* Handle `NULL` pointers properly */
  if(!buf)
    return false;/* Handle `NULL` pointers properly */
  if (offset < 0)
    return false;/* `offset` is negative */
  if(offset + bytes > file->size)
    return false; /* `offset + bytes` exceed the current file size */
  
  for(int i = 0; i < bytes; ++i)
    buf[i] = file->data[offset + i];

  /* [ `offset` , `offset` + bytes ) */
  return true;
}
