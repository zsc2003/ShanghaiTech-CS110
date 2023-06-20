#ifndef __CS110_HW2_EXP__
#define __CS110_HW2_EXP__

#include "dir.h"
#include "file.h"
#include "node.h"

#define MAX_FT_NO 256

/* A file explorer is composed of root directory, current working directory and
 * a list of supported filetypes.
 * Each file type should be registered with a function before they are opened.
 */
struct explorer {
  struct directory *cwd;  /* current working directory */
  struct directory *root; /* root directory */
  open_func ft_open[MAX_FT_NO];
};

/* @function: `explorer_new`
 * @parameter: `void`
 * @return: The newly created explorer.
 * @description: Allocate a new piece of memory and create an explorer object on
 * it.
 * Also create an empty root directory and set current working directory to
 * root.
 * If any of the above procedure fails, release the already allocated memory and
 * return NULL.
 * On success, return the pointer to the newly create explorer
 * object. */
struct explorer *explorer_new(void);

/* @function: `explorer_release`
 * @parameter: `exp: struct explorer *` Self.
 * @return: `void`
 * @description: Deallocate a explorer and release the root directory.
 * Do nothing on NULL pointer. */
void explorer_release(struct explorer *exp);

/* @function: `explorer_read`
 * @parameter: `exp: struct explorer *` Self.
 * @parameter: `name: char *` The name of the file.
 * @parameter: `offset: int` The offset of the file.
 * @parameter: `bytes: int` The number of bytes to be read.
 * @parameter: `buf: char *` The buffer to store the readout bytes.
 * @return: `true` on success and `false` on error.
 *
 * @description: Read the range [offset, offset+bytes-1] from a file with
 * name equal to`name` to `buf`.
 *
 * If no file with name equal to`name` exists in current working directory,
 * return false.
 * On success, return true. */
bool explorer_read(const struct explorer *exp, const char *name, int offset,
                   int bytes, char *buf);

/* @function: `explorer_write`
 * @parameter: `exp: struct explorer *` Self.
 * @parameter: `name: char *` The name of the file.
 * @parameter: `offset: int` The offset of the file.
 * @parameter: `bytes: int` The number of bytes to write.
 * @parameter: `buf: char *` The data to be written.
 * @return: `true` on success and `false` on error.
 * @description: Write data from `buf` to the range [offset, offset+bytes-1] of
 * a file with name equal to`name`.
 * If no file with name equal to `name` exists in current working directory,
 * return false.
 * On success, return true. */
bool explorer_write(struct explorer *exp, const char *name, int offset,
                    int bytes, const char *buf);

/* @function: `explorer_create`
 * @parameter: `exp: struct explorer *` Self.
 * @parameter: `name: char *` The name of the file.
 * @parameter: `type: int` The type of the file.
 * @return: `true` on success and `false` on error.
 * @description: Create a file with name=`name` and type=`type`.
 * If a file or subdir with name=`name` already exists in current
 * working directory, return false.
 * On success, return true. */
bool explorer_create(struct explorer *exp, char *name, int type);

/* @function: `explorer_mkdir`
 * @parameter: `exp: struct explorer *` Self.
 * @parameter: `name: char *` The name of the directory.
 * @return: `true` on success and `false` on error.
 * @description: Create a directory with name equal to`name`. If a subdir with
 * name equal`name` already exists in current working directory, return false.
 * On success, return true. */
bool explorer_mkdir(struct explorer *exp, char *name);

/* @function: `explorer_delete`
 * @parameter: `exp: struct explorer *` Self.
 * @parameter: `name: char *` The name of the node.
 * @return: `true` on success and `false` on error.
 * @description: Remove a file or directory with name equal to `name` in current
 * working directory. If no file or subdir with name equal to `name` exists
 * current, do nothing and return false; On success, return true. */
bool explorer_delete(struct explorer *exp, const char *name);

/* @function: `explorer_contain`
 * @parameter: `exp: struct explorer *` Self.
 * @parameter: `name: char *` The name of the node.
 * @return: `true` if node exisits.
 * @description: Determine whether a file or directory with name equal to`name`
 * exists in current working directory. */
bool explorer_contain(struct explorer *exp, const char *name);

/* @function: `explorer_cdpar`
 * @parameter: `exp: struct explorer *`Self.
 * @return: `true` on success and `false` on error.
 * @description: Change directory to the parent directory of current working
 * directory.
 * If the parent directory does not exists, return false.
 * Return `true` on success.
 * Do nothing on `NULL` pointer and return false. */
bool explorer_cdpar(struct explorer *exp);

/* @function: `explorer_chdir`
 * @parameter: `exp: struct explorer *` Self
 * @parameter: `name: const char *` The name of the subdir.
 * @return: `void`
 * @description: Change current working directory to a subdir whose name matches
 * with `name`.
 * If such directory does not exists, return false.
 * Return `true` on success.
 * Do nothing on `NULL` pointer and return false. */
bool explorer_chdir(struct explorer *exp, const char *name);

/* @function: `explorer_support_filetype`
 * @parameter: `exp: struct explorer *` Self.
 * @parameter: `callback: open_func` The open function for the type.
 * @parameter: `filetype: int` The filetype.
 * @return: `true` on success and `false` on error.
 * @description: Register a new callback function for filetype number
 * `filetype`.
 * If we already have a callback function for `filetype`, return false.
 * Do nothing on `NULL` pointer and return false.
 * On success, return true. */
bool explorer_support_filetype(struct explorer *exp, open_func callback,
                               int filetype);

/* @function: `explorer_open`
 * @parameter: `exp: struct explorer *` Self.
 * @parameter: `name: const char *` The name of the file to be opened.
 * @return: `true` on success and `false` on error.
 * @description: Register a new callback function for filetype number
 * `filetype`.
 * If we are trying to open a directory, return false.
 * If we don't have a callback function for the, return false.
 * Do nothing on `NULL` pointer and return false.
 * On success, return true. */
bool explorer_open(const struct explorer *exp, const char *name);

/* @typedef: `find_func`
 * @type: `(char *, struct file *) -> void`
 * @description: Type of callback function in `explorer_search`. */
typedef void (*find_func)(char *, struct file *);

/* @function: `explorer_search_recursive`
 * @parameter: `exp: struct explorer *` Self.
 * @parameter: `name: char *` Name to be searched.
 * @parameter: `callback: find_func` The callback function called when we find
 * the file.
 * @return: `void`
 * @description: Recursively search for files with name equal to `name` in all
 * the descendants of the current working directory.
 *
 * For every file with name equal`name`, invoke the callback function
 * with the path relative to the current working directory and the file pointer.
 *
 * For example, if cwd is X and we search for a recursively.
 * dir X{
 *     file a,
 *     dir A{
 *         file a,
 *     },
 *     dir B{
 *         file a,
 *         dir C{
 *             file a
 *         },
 *         file B,
 *     }
 * }
 * the callback function should be called with
 * - callback(/a, file(X/a))
 * - callback(/A/a, file(X/A/a))
 * - callback(/B/a, file(X/B/a))
 * - callback(/B/C/a, file(X/B/C/a))
 *
 * You may traverse the directory tree in any order,
 * the order of discovery is not taken into account.
 */
void explorer_search_recursive(struct explorer *exp, const char *name,
                               find_func callback);
#endif
