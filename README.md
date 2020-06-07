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

## Initialization
A new RBT can be intialized with the `init_rbtree` function. For example,
```C
  struct RBTreeNode * root = init_rbtree(10, NULL);
```

# Testing

# Future Work
