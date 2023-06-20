#include "dir.h"
#include "explorer.h"
#include "file.h"
#include "node.h"
#include <stdlib.h>
#include <string.h>
/* header files */
// static bool dir_add_sub(struct directory *dirnode, struct node *sub)/* static function */
// {
    // return;
// } 

//construct a new directory
struct directory *dir_new(char *name) {
  /* Initialization */
  struct directory *dir = NULL;
  /* Check for null pointer */
  if (!name) {
    return NULL;
  }
  /* Allocate memory */
  dir = calloc(1, sizeof(struct directory));
  dir->capacity = DEFAULT_DIR_SIZE;
  dir->subordinates = calloc(dir->capacity, sizeof(struct node*));
  dir->size = 0;/* added myself to make sure the size is 0 */
  dir->parent = NULL;
  /* Create base node */
  dir->base = node_new(true, name, dir);
  return dir;
}

#include <stdio.h>

// release the pointer memory of the directory
void dir_release(struct directory *dir) {
  /* Initialization */
  int i = 0;
  if (!dir) { /* handle null pointer*/
    return;
  }
  /* Release all the subordiniates */
  for (i = 0; i < dir->size; i++) {
    node_release(dir->subordinates[i]);
  }
  /* Release the resources */
  /* Check if base has already been released. Prevent circular call. */
  if (dir->base) {
    dir->base->inner.dir = NULL;
    node_release(dir->base);
  }
  /* Release data and self. */
  free(dir->subordinates);
  free(dir);
}

// find if there has a directory named 'name'
struct node *dir_find_node(const struct directory *dir, const char *name) {
  /* YOUR CODE HERE */
  /* printf("NOT IMPLEMENTED\n"); */
  if(!dir) /*handle null pointer*/
    return NULL;
  if(!name)/*handle null pointer*/
    return NULL;
  if(strcmp(dir->base->name, name) == 0)
    return dir->base;/* matched the dir */
  for(int i = 0; i < dir->size ; ++i)
    if(strcmp(dir->subordinates[i]->name, name) == 0)
      return dir->subordinates[i];/* matched the subordinate node */

  return NULL;/* do not matched any node*/
}

// add a file to the dir
bool dir_add_file(struct directory *dir, int type, char *name) {
  /* YOUR CODE HERE */
  /*printf("NOT IMPLEMENTED\n");*/
  if(!dir) /*handle null pointer*/
    return false;
  if(!name)/*handle null pointer*/
    return false;
  for(int i = 0; i < dir->size ; ++i)
  {
    // if(dir->subordinates[i]->is_dir == false)/* it should be a file*/
    // { do not need to consider about that
      if(strcmp(dir->subordinates[i]->name, name) == 0)  
        return false; /* `name` already exists */
    // }
  }
    
  if(dir->size == dir->capacity)
  {
    dir->capacity *= 2; /* enlarge the capacity of dir*/
    dir->subordinates = (struct node**)realloc(dir->subordinates, sizeof(struct node*) * dir->capacity);
    for(int i = dir->capacity / 2; i < dir->capacity; ++i)
      dir->subordinates[i] = NULL; /* init unused as null pointer */
  }
  
  struct file* new_file = file_new(type, name);
  if(!new_file) /* make file fail*/
    return false;

  dir->subordinates[dir->size] = node_new(false, name, new_file); /* set up a new file*/
  ++dir->size;/* enlarge the size of dir*/
  return true;/*success*/
}

// add a sub-directory to dir
bool dir_add_subdir(struct directory *dir, char *name) {
  /* YOUR CODE HERE */
  /*printf("NOT IMPLEMENTED\n"); */
  if(!dir) /*handle null pointer*/
    return false;
  if(!name)/*handle null pointer*/
    return false;
  for(int i = 0; i < dir->size ; ++i)
  {
    // if(dir->subordinates[i]->is_dir == true)/* it should be a dir*/
    // { do not need to consider about that
      if(strcmp(dir->subordinates[i]->name, name) == 0)  
        return false; /* `name` already exists */
    // }
  }

  if(dir->size == dir->capacity)
  {
    dir->capacity *= 2; /* enlarge the capacity of dir*/
    dir->subordinates = (struct node**)realloc(dir->subordinates, sizeof(struct node*) * dir->capacity);
    for(int i = dir->capacity / 2; i < dir->capacity; ++i)
      dir->subordinates[i] = NULL; /* init unused as null pointer */
  }

  struct directory* new_dir = dir_new(name);
  if(!new_dir)/* make file fail*/
    return false;

  new_dir->parent = dir; /* important!! do not forget to set the parent*/

  dir->subordinates[dir->size] = node_new(true, name, new_dir); /* set up a new dir*/
  ++dir->size;/* enlarge the size of dir*/
  return true;/*success*/
}

// delete a directory
bool dir_delete(struct directory *dir, const char *name) {
  /* YOUR CODE HERE */
  /* printf("NOT IMPLEMENTED\n"); */
  if(!dir) /* Handle `NULL` pointer */
    return false;
  if(!name) /* Handle `NULL` pointer */
    return false;
  for(int i = 0; i < dir->size; ++i)
  {
    // if(dir->subordinates[i]->is_dir == true)/* deal with dir*/
    {
      if(strcmp(dir->subordinates[i]->name, name) == 0)
      { /* matched the dir that need to be deleted*/
        node_release(dir->subordinates[i]);
        dir->subordinates[i] = dir->subordinates[dir->size - 1]; /* important!!! remove the last one into the middle*/
        --dir->size;
        return true;
      }
    }
  }
  return false; /* no subordinate is called `name` */
}
