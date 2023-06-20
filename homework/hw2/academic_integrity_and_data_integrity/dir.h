#ifndef __CS110_HW2_DIR__
#define __CS110_HW2_DIR__

#include "bool.h"
#include "file.h"

/* @macro: DEFAULT_DIR_SIZE
 * @description: when a directory is created, the capacity should be
 * `DEFAULT_DIR_SIZE` */
#define DEFAULT_DIR_SIZE 16

/* A directory contains subordinates. Each of them is either a file or a
 * directory.
 * The capacity of a directory should expand automatically. */
struct directory {
  int size;     /* actual number of subordinates in this directory */
  int capacity; /* the maximum number of subordinates can be stored */
  struct node **subordinates; /* list of pointers to subordinates */
  struct directory *parent;   /* the directory in which this node resides */
  struct node *base;          /* the node structure associated with directory*/
};

/* @function: `dir_new`
 * @parameter: `name: char*` The name of the dir.
 * @return: The created directory.
 * @description: Allocate a piece of memory and create a directory object on it.
 * The initial capacity of this directory should be `DEFAULT_DIR_SIZE` and the
 * initial size ought to be zero.
 * If any step of the procedure fails, return NULL.
 * Also deallocate the acquired memory on error to prevent leakage.
 * Return a pointer to the newly created directory if success. */
struct directory *dir_new(char *name);

/* @function: `dir_release`
 * @parameter: `dir: struct directory *` Self.
 * @return: `void`.
 *
 * @description: Release the memory allocated for a file directory.
 * Remove all the descendants recursively.
 * Handle `NULL` pointers properly: do nothing.
 * Note: dir objects are embeded in node objects, and `node_release` shall call
 * `dir_release`. */
void dir_release(struct directory *dir);

/* @function: `dir_find_node`
 * @parameter: `dir: struct directory *` Self.
 * @parameter: `name: char *` The name of the node.
 * @return: The node found, could be either a file or a directory. Return NULL
 * if no such node exisit.
 *
 * @description: Find a subordinate node in `dir` with exactly matched `name`.
 * Handle `NULL` pointers properly: do nothing, return `NULL`. */
struct node *dir_find_node(const struct directory *dir, const char *name);

/* @function: `dir_add_file`
 * @parameter: `dir: struct directory *` Self.
 * @parameter: `type: int` The type of the file.
 * @parameter: `name: char *` The name of the file.
 * @return: `true` on success and `false` on error.
 *
 * @description: Make `file` a subordinate of `dir` with name equal to `name`.
 * Return `false` if a subordinate called `name` already exists.
 * Enlarge the capacity to `2 * current_capacity` if `size` is to exceed
 * `capacity`.
 * Handle `NULL` pointers properly: do nothing, return `false`.*/
bool dir_add_file(struct directory *dir, int type, char *name);

/* @function: `dir_add_subdir`
 * @parameter: `dir: struct directory *` Self.
 * @parameter: `name: char *` The name of the directory
 * @return: `true` on success and `false` on error.
 *
 * @description: Make `sub` a subordinate of `dir` with name equal to`name`.
 * Do nothing if a subordinate called `name` already exists.
 * Enlarge the capacity to `2 * current_capacity` if `size` is to exceed
 * `capacity`.
 * Set the parent correctly, after creating the new dir successfully.
 * Handle `NULL` pointers properly: do nothing, return `false`.*/
bool dir_add_subdir(struct directory *dir, char *name);

/* @function: `dir_delete`
 * @parameter: `dir: struct directory *` Self.
 * @parameter: `name: char *` The name of the node.
 * @return: `true` on success and `false` on error.
 *
 * @description: Remove and release the subordinate of `dir` with name equal to
 * `name`.
 * Do nothing and return `false` if no subordinate is called `name`.
 * The capacity of `dir` should not be changed.
 * Handle `NULL` pointers properly: do nothing, return `false`.*/
bool dir_delete(struct directory *dir, const char *name);

#endif
