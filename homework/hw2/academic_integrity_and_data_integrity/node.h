#ifndef __CS110_HW2_NODE__
#define __CS110_HW2_NODE__

#include "bool.h"

#define MAX_NAME_LEN 32

struct directory;
struct file;

/* A node is a node on the directory tree, which is the smallest unit in
 * the explore.*/
struct node {
  char name[MAX_NAME_LEN]; /* the name of the node */
  bool is_dir;             /* whether this is a directory node */
  union {
    struct directory *dir;
    struct file *file;
  } inner; /* either a directory or a file */
};

/* @function: `node_new`
 * @parameter: `is_dir: bool`, `name: char *`, `inner: void *`
 * @description: Allocate a piece of memory and create a node object on it.
 * The node `name`, `is_dir` and `inner` are given in the parameter.
 * The ownership of `inner` is moved into this function,
 * and the created `node` is responsible for releasing `inner`.
 *
 * If any step of the procedure fails, return `NULL`.
 * Also deallocate the acquired memory on error to prevent leaking.
 * Return a pointer to the newly created node if success. */
struct node *node_new(bool is_dir, char *name, void *inner);

/* @function: `node_release`
 * @parameter: `node: struct node *`
 * @return: `void`
 * @description: Release the memory allocated for a node object.
 * Also release the embeded file or directory.
 * Handle `NULL` pointers properly: do nothing. */
void node_release(struct node *sub);

#define NODE_DIR(n) (((struct node *)n)->inner.dir)
#define NODE_FILE(n) (((struct node *)n)->inner.file)

#endif
