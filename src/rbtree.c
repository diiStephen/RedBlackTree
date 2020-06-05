#include "errors.h"
#include "rbtree.h"
#include<stdio.h>
#include<stdlib.h>

struct RBTreeNode* init_rbtree_node(struct RBTreeNode *p, struct RBTreeNode *l,
  struct RBTreeNode *r, int k, void *d, color_t c, bool s) {

  struct RBTreeNode *node = NULL;

  if ((node = malloc(sizeof(struct RBTreeNode))) == NULL )
    display_error(MEM_ERROR);


  node->parent = p;
  node->left   = l;
  node->right  = r;
  node->key    = k;
  node->data   = d;
  node->c      = c;
  node->isSen  = s;

  return node;

}

/**
Function for destroying RBTreeNodes. Accepts a pointer
to a node to be destroyed. The node's satelite data pointer
must be handled by the caller. The convention here is that the
data field of the node struct must be assigned to 0 to indicate
that the data has been handled.

NOTE: There may be memory leaks if the satelite data is not handled properly.

@param node Pointer to node to be destroyed.
**/
void dest_rbtree_node(struct RBTreeNode *node) {
  if (node->data == 0) {
    free_rbtree_node(&node);
  } else {
    display_error(INV_DELOC);
  }
}

/**
Specialized free function to avoid dangling pointers with
RBTreeNodes after they have been removed. Once the node has been
freed the reference is nullified to ensure we don't have a dangling
pointer.

NOTE: There may be other references to the same block of data, though. We must
be careful to avoid any such situation. This may be particuarly relevant to
the sentinel node where it will be the child of many nodes in the RBT.

@param node Double pointer to RBTreeNode to be freed.
**/
void free_rbtree_node(struct RBTreeNode **node) {
  if(node != NULL) {
    free(*node);
    *node = NULL;
  }
}

/**
Function to construct a new RBT with intial key k and initial satelite
data pointer d. Function creates a root node and the sentinel node. The
sentinel node can always be accessed by using root->parent. This sentinel
will be useful in simplifying the implementation of the RBT operations.

NOTE: All operations will need to take into account the sentinel node.

@param k Initial key of the root data.
@param d Inital satelite data pointer, becomes root node's associated data.
@return Pointer to root node of the rbtree.
**/
struct RBTreeNode* init_rbtree(int k, void *d) {

  //Allocate sentinel node.
  struct RBTreeNode *sentinel =
    init_rbtree_node(NULL, NULL, NULL, 0, NULL, BLACK, true);

  //Allocate root node with parent,left,right = sentinel.
  struct RBTreeNode *root =
    init_rbtree_node(sentinel, sentinel, sentinel, k, d, BLACK, false);
  return root;
}

/**
Function to free memory used by entire RBT. The function
post-order walks the tree, freeing each node.

CAUTION: This function assumes that all data associated to
each node has already been destroyed.

@param root Root of RBT to be de-allocated.
**/
void dest_rbtree(struct RBTreeNode *root) {

  if (root->left != NULL)
    dest_rbtree(root->left);
  if (root->right != NULL)
    dest_rbtree(root->right);

  root->data = 0;
  dest_rbtree_node(root);

}

/**
Insert data into the RBT. Ordereding is
given by the integer key associated with the satelite
data.

New nodes inserted to the RBT are colored RED. The rest of the insert
procedure is essentially the same as that for a standard BST, however
NULL pointers are replaced with a reference to the sentinel (obtainable
as *root->parent here), and an auxillary procedure, to fix any RBT property
violations, is called at the end.

@param k Key associated with data. Used for searching, ordering, etc.
@param data Data associated with the node. void* for generic data.
@return Pointer to the new node inserted into the tree.
**/
struct RBTreeNode* insert(int k, void *data, struct RBTreeNode **root) {

  // Create new node.
  struct RBTreeNode *newest =
    init_rbtree_node(NULL, (*root)->parent, (*root)->parent, k, data, RED, false);

  struct RBTreeNode *s = (*root)->parent; // Reference to sentinel.
  struct RBTreeNode *walk = *root;      // Walk startes at the root.
  struct RBTreeNode *parent = (*root)->parent; // Trailing pointer used for insert.

  // Find the appropriate spot for the node.
  while (walk != s) { //While we haven't reached the sentinel.
    parent = walk;
    if (k < walk->key) {
      walk = walk->left;
    } else {
      walk = walk->right;
    }
  }
  newest->parent = parent; //Correctly deals with the sentinel.

  // Update pointers.
  if (parent == s) {
    *root = newest; //Tree was empty.
  } else if (k < parent->key) {
    parent->left = newest;
  } else {
    parent->right = newest;
  }

  insert_fixup(root, newest); //Fix any voilations.

  return newest;
}

/**
Correct any violations of the RBT properties after performing
a standard BST insert. There are only two possible property
violations (See notes for full details).


@param root Root of RBT where insertion has taken place.
@param newest New node inserted into the tree which violate RBT properties.
**/
void insert_fixup(struct RBTreeNode **root, struct RBTreeNode *newest) {

  struct RBTreeNode *uncle = NULL; // Uncle of newest.

  while(newest->parent->c == RED) { // Violation of IV

    // parent is left child of grandparent.
    if (newest->parent == newest->parent->parent->left) {

      uncle = newest->parent->parent->right; // Uncle is right child of grandparent.

      if (uncle->c == RED) { // Case 1

        newest->parent->c = BLACK;        //Change parent to BLACK.
        uncle->c = BLACK;                 //Change uncle's color to BLACK.
        newest->parent->parent->c = RED;  //Grandparent becomes RED.
        newest = newest->parent->parent;      //Newest violation may now be the grandparent.

      } else {

        if (newest == newest->parent->right) { // Case 2
          newest = newest->parent;
          left_rotate(root, newest); //Rotate to become Case 3.
        }

        // Case 3
        newest->parent->c = BLACK; //Note: This terminates the loop.
        newest->parent->parent->c = RED;
        right_rotate(root, newest->parent->parent);

      } // End cases 2, 3

    } else { // parent is right child of grandparent.

      uncle = newest->parent->parent->left; // Uncle is left child of grandparent.

      if (uncle->c == RED) { // Case 4

        newest->parent->c = BLACK; // Same idea as above.
        uncle->c = BLACK;
        newest->parent->parent->c = RED;
        newest = newest->parent->parent; // Newest violation may now be the grandparent.

      } else {

        if (newest == newest->parent->left) { // Case 5
          newest = newest->parent;
          right_rotate(root, newest); // Rotate to become case 6. Do we right_rotate?
        }

        // Case 6
        newest->parent->c = BLACK;
        newest->parent->parent->c = RED;
        left_rotate(root, newest->parent->parent); // left_rotate?

      } // End cases 5, 6

    } // End parent is right child of grandparent.
  } // Corrected property IV violation.

  (*root)->c = BLACK; //Correct property II violation.

}

/**
Public delete function. Removes a node with the given key
in RB-tree rooted at *root. First the function finds a node
with the given key, then calls the delete_node function on
that node.

@param key Key of node to be removed from the tree.
@param root Pointer to the root pointer of the rbtree.
@return Pointer to data removed from the tree, or null if node with
given key does not exist.
**/
void* delete(int key, struct RBTreeNode *root) {
  struct RBTreeNode *result = search(key, root);
  if (result == NULL)
    return result;
  else
    return delete_node(&root, result);
}

/**
Private utility function for deleting nodes. Removes the node
pointed to by node from the rbtree.

@param root Pointer to the root pointer of the RBT where the deletion
is to take place.
@param node Pointer to RBTreeNode pointer to be removed from the tree.
@return pointer to satelite data associated to the node being removed
from the tree.
**/
void* delete_node(struct RBTreeNode **root, struct RBTreeNode *node) {

  struct RBTreeNode *s          = (*root)->parent; // Sentinel.
  struct RBTreeNode *deleted    = node;  //Save pointer to the node.
  struct RBTreeNode *replace    = NULL;  //Node removed or moved in T.
  struct RBTreeNode *moved      = NULL;  //Node moved to replace's position.
  void *response                = NULL;  //ptr to hold the response.
  color_t originalColor         = BLACK; //Original color of replace.

  // Replace is either node to be deleted, or node to be moved.
  replace = node;
  originalColor = replace->c;

  if (node->left == s) { // One child on the right, or none.

    moved = node->right;
    transplant(root, node, node->right); // Case I

  } else if (node->right == s) { // One child on the left.

    moved = node->left;
    transplant(root, node, node->left); // Case II

  } else { // Cases III  i.e. two children.

    replace = minimum(node->right); // Node will be replaced with its successor.
    originalColor = replace->c;     // We need to save the original color of the node we're moving.
    moved = replace->right;         // Note, replace has no left child. This will be moved into replace's position.

    if (replace->parent == node) { // Case III-A, replace = node->right.
      moved->parent = replace; // Note: moved may be s, the sentinel.
    } else { // Case III-B, replace != node->right, but is contained in the subtree root at node->right.
      transplant(root, replace, replace->right);
      replace->right = node->right;
      replace->right->parent = replace;
    }

    transplant(root, node, replace);
    replace->left = node->left;
    replace->left->parent = replace;
    replace->c = node->c;

  }

  deleted->parent = deleted; //Convention for deleted node.
  deleted->right = NULL;
  deleted->left = NULL;
  response = deleted->data;
  deleted->data = 0; // Convention for deleted node.
  dest_rbtree_node(deleted); // Delete the memory for the node.

  //if (originalColor == BLACK)
    //RBT-Delete-Fixup(root, moved);

  return response;
}

/**
Private utility function used in the delete procedure. Reaplaces
the subtree rooted at node dest with subtree rooted at node src.

NOTE: I suspect there is a memory leak here, i.e. dest.

@param root Root of RB-tree where transplant is taking place.
@param dest Destination of replacement.
@param src Source of replacement.
**/
void transplant(struct RBTreeNode **root, struct RBTreeNode *dest,
  struct RBTreeNode *src) {

    struct RBTreeNode *s = (*root)->parent; //Sentinel.

    if (dest->parent == s) {
      *root = src; // dest is the root.
    } else if (dest == dest->parent->left) {
      dest->parent->left = src; // dest is the left child of its parent.
    } else {
      dest->parent->right = src; // dest is the right child of its parent.
    }

    // Unconditional -- the sentinel makes the Null check unnecessary.
    src->parent = dest->parent; // src parent ptr is updated.

  }

/**
Perform a recursive search of the RB-tree rooted at root.
Uses the BST property for fast searches. Let h = height(T),
then the running time of search is O(h).

@param key Key associated to the node being searched.
@param root Root of the RB-tree being searched.
@return Pointer to node with given key, or null if search fails.
**/
struct RBTreeNode* search(int key, struct RBTreeNode *root) {
  validate(root, false);

  struct RBTreeNode *walk = root;

  if (walk->isSen == true) {
    return NULL; // Don't return the sentinel.
  } else if (key < walk->key) {
    search(key, walk->left);
  } else if (key > walk->key) {
    search(key, walk->right);
  } else {
    return walk;
  }
}

/**
Return a pointer to the node of the RB-tree with minimum key value. The search
is done by recursively following the left pointers of each node from the root.

@param root Root of the tree to get minimum key.
@return Pointer to node of the tree with minimum key, or null if the tree is
empty.
**/
struct RBTreeNode* minimum(struct RBTreeNode *root) {
  validate(root, false);
  if (root->isSen == true || root->left->isSen == true) // Short circuting.
    return root;
  else
    minimum(root->left);
}

/**
Return a pointer to the node of the RB-tree with maximum key value. The search
is done by recursively following the right pointers of each node from the root.

@param root Root of the tree to get minimum key.
@return Pointer to node of the tree with maximum key, or null if the tree is
empty.
**/
struct RBTreeNode* maximum(struct RBTreeNode *root) {
  validate(root, false);
  if (root->isSen == true || root->right->isSen == true) // Short circuting.
    return root;
  else
    maximum(root->right);
}

/**
Return a pointer to the node of the RB-tree with key that is the
predecessor of the key given node.

@param node Pointer to node of RB-tree to find predecessor of.
@return pointer to predecessor node, or null.
**/
struct RBTreeNode* predecessor(struct RBTreeNode *node) {
  validate(node, true);

  if (node->left != NULL) {
    return maximum(node->left);
  }

  struct RBTreeNode *trace = node->parent; //Ascending the tree.
  while(trace != NULL && node == trace->left) {
    node = trace;
    trace = trace->parent;
  }
  return trace;

}

/**
Return a pointer to the node of the RB-tree with key that is the
successor of key of the given node.

@param node Pointer to node of RB-tree to find successor of.
@return pointer to successor node, or null.
**/
struct RBTreeNode* successor(struct RBTreeNode *node) {
  validate(node, true);

  if (node->right != NULL) {
    return minimum(node->right);
  }

  struct RBTreeNode *trace = node->parent; // Ascending the tree.
  while(trace != NULL && node == trace->right) {
    node = trace;
    trace = trace->parent;
  }
  return trace;

}

/**
Validate a the RBTreeNode pointed to by node.

Convention: A node removed from the tree will have its parent pointer
assigned to itself. This is used to indicate a defunct node of the tree.

Some functions allow a null pointer as node, others do not, so
the chkNull parameter here specifies if a null node is considered
invalid.

@param node Pointer to RBTreeNode to be validated
@param chkNull Consider null pointers invalid?
**/
void validate(struct RBTreeNode *node, bool chkNull) {

  switch (chkNull) {

    case true:
      if (node == NULL || node->parent == node)
        display_error(INV_NODE);
      break;

    case false:
      if (node != NULL && node->parent == node)
        display_error(INV_NODE);
      break;
  }
}

/**
Implementation of the Left-Rotate(T, x) algorithm. Used for re-balancing
the RBT after update operations. This operation is a structural update, i.e.
by re-arranging pointers. The running time is O(1). Furthermore, a left-rotate
preserves the BST properties.

left_rotate assumes that node->right != s, the senitnel.

@param root Root of the RBT containing node.
@node Node of the tree to Left-Rotate.
**/
void left_rotate(struct RBTreeNode **root, struct RBTreeNode *node) {

  struct RBTreeNode *r = node->right; // r replaces node at node's position.

  node->right = r->left; // Left subtree of r becomes node's right subtree.

  if (r->left->isSen == false) // Update parent pointer of r's left child.
    r->left->parent = node;

  r->parent = node->parent; // r's parent becomes x's former parent.

  if (node->parent->isSen == true) {        //Node was root.
    *root = r;
  } else if (node == node->parent->left) {  // Node is a left child.
    node->parent->left = r;
  } else {                                  // Node must be the right child.
    node->parent->right = r;
  }

  r->left = node; // Place node in it's proper place.
  node->parent = r;

}

/**
Implementation of the Right-Rotate(T, x) algorithm. Used for re-balancing
the RBT after update operations. This operation is a structural update, i.e.
by re-arranging pointers. The running time is O(1). Furthermore, a right-rotate
preserves the BST properties.

right_rotate assumes that node->left != s, the senitnel.

Question: Do I need double pointers here to perform these updates? Pass
by value vs. pass by reference w.r.t the root pointer, I believe.

@param root Root of the RBT containing node.
@node Node of the tree to Right-Rotate.
**/
void right_rotate(struct RBTreeNode **root, struct RBTreeNode *node) {

  struct RBTreeNode *r = node->left; // r places node in at node's position.

  node->left = r->right; //Left subtree of node becomes right subtree of r.

  if (r->right->isSen == false) //s.p is meaningless here.
    r->right->parent = node;

  r->parent = node->parent; // r must take node's place.

  if (node->parent->isSen == true) { // node is T's root.
    *root = r;
  } else if (node == node->parent->left) { //node is a left child.
    node->parent->left = r;
  } else {
    node->parent->right = r; // node is a right child.
  }

  r->right = node;
  node->parent = r;

}
