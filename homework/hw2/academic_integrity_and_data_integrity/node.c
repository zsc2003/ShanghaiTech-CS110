#include "node.h"
#include "dir.h"
#include "file.h"
#include <stdlib.h>
#include <string.h>

struct node *node_new(bool is_dir, char *name, void *inner) {
  /* Initialization */
  struct node *node = NULL;
  /* Check for null pointer */
  if (!name || !inner) {
    return NULL;
  }
  /* Allocate memory */
  node = calloc(1, sizeof(struct node));
  /* Copy the name. */
  strncpy(node->name, name, DEFAULT_DIR_SIZE);
  /* Initialization */
  node->is_dir = is_dir;
  node->inner.dir = inner;
  return node;
}

void node_release(struct node *sub) {
  /* Check for null pointer */
  if (!sub) {
    return;
  }
  /* Call the destructors based on the types */
  if (sub->is_dir) {
    /* Check if inner dir has already been released. */
    if (sub->inner.dir) {
      sub->inner.dir->base = NULL;
      dir_release(sub->inner.dir);
    }
  } else {
    /* Check if innder file has already been released. */
    if (sub->inner.file) {
      sub->inner.file->base = NULL;
      file_release(sub->inner.file);
    }
  }
  /* release the resources */
  free(sub);
}
