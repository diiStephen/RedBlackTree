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

# Usage

The files rbtree.c and rbtree.h can be included with any C project and compiled
along with the rest of the source. The makefile included at the moment can be
used to build the test-engine program.

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

```C
/* Search and delete function - public. */
void* search_and_delete(struct RBTreeNode **, int);

/* Delete function - private. */
void* delete_node(struct RBTreeNode **, struct RBTreeNode *);
```

## Query Functions

# Testing

# Future Work
