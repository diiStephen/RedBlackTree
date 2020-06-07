# RedBlackTree

An implementation of the Red-Black Tree data structure in C. The interface
is designed to be simple to use and generic with the use of a void * field
for satellite data. Keys, at the moment, are assumed to be integers. Thus,
the data structure can be used to store a collection of `int`s or as a map for
more complicated data.

#  Background

Let T be a **red-black tree** (RBT). Then T is a **binary search tree** (BST) with
guaranteed height O(lg n) where `n = size(T)`, i.e. the number of nodes in T.
Thus, an RBT is balanced. Thus, operations like search, insert, delete, minimum,
maximum, etc... run in O(lg n) worst-case time.

Full details can be found in the docs directory (if they are available). These
notes will cover the full details of the RBT as a data-structure.

# Compilation
The included makefile includes two build targets: default and engine.

The *default* target will build a shared object library, `rbtree.so` in the `build/`
directory. This can then be used just like any other shared object library.

The *engine* target builds the test-engine program, also in the `build/`
directory. This program can be used to write tests for the RBT. Further
details regarding `test-engine` are available below in the "Testing" section.

Additionally, the files rbtree.c, rbtree.h, errors.c, and errors.h can be
included with any C project and compiled along with the rest of the source. Thus,
you don't have to use the makefile if it's not necessary.

# Usage



**NOTE**: The functions described here are "public" in the sense
that they are meant for external use. The library includes many "private"
support functions that are not meant to be invoked by the user. They are
mostly called during re-balancing procedures after insertions and deletions.
Using them outside of their correct context will violate the integrity
of the data structure. Consider yourself warned.

## Initialization and Destruction

A new RBT is intialized with the `init_rbtree` function. For example,

```C
struct RBTreeNode *root = init_rbtree(10, NULL);
```

will create a new RBT, rooted at the node pointed to by `root`, with key 10
and `NULL` satellite data. Of course, `typedef` can be used to simplify the
code, but the choice is up to the user. All updates and queries for the tree
will be done through use of the `root` pointer.

An RBT with root pointer `root` can be destroyed using the `dest_rbtree`
function with takes a double pointer to the root. E.g.

```C
dest_rbtree(&root);
```

**NOTE**: `dest_rbtree` assumes that any satellite data contained in the nodes
have been handled. Otherwise, this may lead to memory leaks. It is the user's
responsibility to deallocate memory created for satellite data.


## Update Functions
The RBT supports the following *update* functions, i.e. these functions
modify the tree.

```C
/* Insertion function. */
struct RBTreeNode* insert(struct RBTreeNode **, int, void *);
```
The `insert` function will insert a new node into the tree with
given `int` key and `void *` satellite data. This operation takes O(lg n) time
even when re-balancing occurs.

There are two delete functions that remove nodes from the tree. Both functions
perform the same operation fundamentally. The RBT may be re-balanced and
re-colored based on the update. Regardless, the time is always O(lg n) for
deletions.

```C
/* Search and delete function . */
void* search_and_delete(struct RBTreeNode **, int);
```
`search_and_delete` searches the tree given by the first parameter for a node
with key given by the second parameter. If a node with the given key exists,
then it will be removed and the memory for that node will be destroyed. A pointer
to the satellite data of the node will be returned, or `NULL` if a node with
the given key is not found in the tree.

```C
/* Delete function. */
void* delete_node(struct RBTreeNode **, struct RBTreeNode *);
```
`delete_node` accepts a tree and a node. The function will remove the given
node from the tree and return a pointer to the satellite data. No memory
is freed for this function. It is assumed that the node is a valid node of
the tree.


## Query Functions
Many of the query functions are self-explanatory. Their name
implies their use. All of them, besides the `height` function,
run in O(lg n) time. The `height` function runs in O(n) time.

```C
/* Search the tree for a given key. */
struct RBTreeNode* search(int, struct RBTreeNode *);
```
Search for a node with the key given as the first parameter. The second
parameter is a pointer to the root of the (sub)tree to be searched. If a node
with the key is found, then a pointer to the node is returned, otherwise a null
pointer is returned.

```C
/* Search tree for node with minimum key. */
struct RBTreeNode* minimum(struct RBTreeNode *);
```
Search for and return a pointer to a node with minimum key in the tree
rooted at the given pointer.  

```C
/* Search tree for node with maximum key. */
struct RBTreeNode* maximum(struct RBTreeNode *);
```
Search for and return a pointer to a node with maximum key in the tree
rooted at the given pointer.   

```C
/* Find predecessor node. */
struct RBTreeNode* predecessor(struct RBTreeNode *);
```
Search for and return a pointer to the node that is the predecessor
of the given node in the tree.

```C
/* Find successor node. */
struct RBTreeNode* successor(struct RBTreeNode *);
```
Search for and return a pointer to the node that is the successor
of the given node in the tree.

```C
/* Compute the height of the RBT. */
int height(struct RBTreeNode *);
```
Compute the height of the tree given by the parameter. This function is mostly
used for testing purposes.

# Testing
Using the included makefile, a test-engine program can be compiled and
linked using the `engine` target of the makefile. I.e. `make engine` will
build an executable called *test-engine* in a directory called `build/`.

Large test cases can be constructed to test the running times of the
code.

The *test-engine* program accepts commands from `stdin` and writes
its output to `stdout`. The program continues to read input until the
`end` commands is issued by the user.    

Commands accepted from `stdin` for building an RBT and performing queries.
As commands are accepted from `stdin`, redirection can be used to
create test files. The `test-cases/` subdirectory of the `tests/` includes
some examples.

As this is for test purposes, only `int`s are allowed as
parameters to commands.

Accepted commands and their formats:

1. ins x -- insert x into the tree.

2. del x -- remove x from the tree.

3. srh x -- search for x in tree.

4. max -- display max key in the tree.

5. min -- display min key in the tree.

6. prt -- print tree in-order.

7. rot -- print details of root node.

8. hit -- print height of the tree.

9. end -- shutdown the test program.


# Future Work

- [ ] Further modularization.

- [ ] More user-friendly memory management.

- [ ] Better runtime-error reporting system. These features are started by
the errors.c file, but it is essentially a place-holder right now.

# Further Remarks

# References
