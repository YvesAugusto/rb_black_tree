using namespace std; 
#include <bits/stdc++.h> 

enum COLOR { RED, BLACK }; 
  
class No { 
public: 
  int val; 
  COLOR color; 
  No *left, *right, *father; 
  
  No(int val) : val(val) { 
    father = left = right = NULL; 
  
    // No is created during insertion 
    // No is red at insertion 
    color = RED; 
  } 
  
  // returns pointer to uncle 
  No *uncle() { 
    // If no father or grandfather, then no uncle 
    if (father == NULL or father->father == NULL) 
      return NULL; 
  
    if (father->isOnLeft()) 
      // uncle on right 
      return father->father->right; 
    else
      // uncle on left 
      return father->father->left; 
  } 
  
  // check if No is left child of father 
  bool isOnLeft() { return this == father->left; } 
  
  // returns pointer to brother 
  No *brother() { 
    // brother null if no father 
    if (father == NULL) 
      return NULL; 
  
    if (isOnLeft()) 
      return father->right; 
  
    return father->left; 
  } 
  
  // moves No down and moves given No in its place 
  void moveDown(No *nfather) { 
    if (father != NULL) { 
      if (isOnLeft()) { 
        father->left = nfather; 
      } else { 
        father->right = nfather; 
      } 
    } 
    nfather->father = father; 
    father = nfather; 
  } 
  
  bool hasRedChild() { 
    return (left != NULL and left->color == RED) or 
           (right != NULL and right->color == RED); 
  } 
}; 
  
class RBTree { 
  No *root; 
  
  // left rotates the given No 
  void giraEsquerda(No *x) { 
    // new father will be No's right child 
    No *nfather = x->right; 
  
    // update root if current No is root 
    if (x == root) 
      root = nfather; 
  
    x->moveDown(nfather); 
  
    // connect x with new father's left element 
    x->right = nfather->left; 
    // connect new father's left element with No 
    // if it is not null 
    if (nfather->left != NULL) 
      nfather->left->father = x; 
  
    // connect new father with x 
    nfather->left = x; 
  } 
  
  void giraDireita(No *x) { 
    // new father will be No's left child 
    No *nfather = x->left; 
  
    // update root if current No is root 
    if (x == root) 
      root = nfather; 
  
    x->moveDown(nfather); 
  
    // connect x with new father's right element 
    x->left = nfather->right; 
    // connect new father's right element with No 
    // if it is not null 
    if (nfather->right != NULL) 
      nfather->right->father = x; 
  
    // connect new father with x 
    nfather->right = x; 
  } 
  
  void swapColors(No *x1, No *x2) { 
    COLOR aux; 
    aux = x1->color; 
    x1->color = x2->color; 
    x2->color = aux; 
  } 
  
  void swapValues(No *u, No *v) { 
    int aux; 
    aux = u->val; 
    u->val = v->val; 
    v->val = aux; 
  } 
  
  // fix red red at given No 
  void fixRedRed(No *x) { 
    // if x is root color it black and return 
    if (x == root) { 
      x->color = BLACK; 
      return; 
    } 
  
    // initialize father, grandfather, uncle 
    No *father = x->father, *grandfather = father->father, 
         *uncle = x->uncle(); 
  
    if (father->color != BLACK) { 
      if (uncle != NULL && uncle->color == RED) { 
        // uncle red, perform recoloring and recurse 
        father->color = BLACK; 
        uncle->color = BLACK; 
        grandfather->color = RED; 
        fixRedRed(grandfather); 
      } else { 
        // Else perform LR, LL, RL, RR 
        if (father->isOnLeft()) { 
          if (x->isOnLeft()) { 
            // for left right 
            swapColors(father, grandfather); 
          } else { 
            giraEsquerda(father); 
            swapColors(x, grandfather); 
          } 
          // for left left and left right 
          giraDireita(grandfather); 
        } else { 
          if (x->isOnLeft()) { 
            // for right left 
            giraDireita(father); 
            swapColors(x, grandfather); 
          } else { 
            swapColors(father, grandfather); 
          } 
  
          // for right right and right left 
          giraEsquerda(grandfather); 
        } 
      } 
    } 
  } 
  
  // find No that do not have a left child 
  // in the subtree of the given No 
  No *sucessor(No *x) { 
    No *aux = x; 
  
    while (aux->left != NULL) 
      aux = aux->left; 
  
    return aux; 
  } 
  
  // find No that replaces a deleted No in BST 
  No *BSTreplace(No *x) { 
    // when No have 2 children 
    if (x->left != NULL and x->right != NULL) 
      return sucessor(x->right); 
  
    // when leaf 
    if (x->left == NULL and x->right == NULL) 
      return NULL; 
  
    // when single child 
    if (x->left != NULL) 
      return x->left; 
    else
      return x->right; 
  } 
  
  // deletes the given No 
  void removeNode(No *v) { 
    No *u = BSTreplace(v); 
  
    // True when u and v are both black 
    bool uvBlack = ((u == NULL or u->color == BLACK) and (v->color == BLACK)); 
    No *father = v->father; 
  
    if (u == NULL) { 
      // u is NULL therefore v is leaf 
      if (v == root) { 
        // v is root, making root null 
        root = NULL; 
      } else { 
        if (uvBlack) { 
          // u and v both black 
          // v is leaf, fix double black at v 
          fixDoubleBlack(v); 
        } else { 
          // u or v is red 
          if (v->brother() != NULL) 
            // brother is not null, make it red" 
            v->brother()->color = RED; 
        } 
  
        // delete v from the tree 
        if (v->isOnLeft()) { 
          father->left = NULL; 
        } else { 
          father->right = NULL; 
        } 
      } 
      delete v; 
      return; 
    } 
  
    if (v->left == NULL or v->right == NULL) { 
      // v has 1 child 
      if (v == root) { 
        // v is root, assign the value of u to v, and delete u 
        v->val = u->val; 
        v->left = v->right = NULL; 
        delete u; 
      } else { 
        // Detach v from tree and move u up 
        if (v->isOnLeft()) { 
          father->left = u; 
        } else { 
          father->right = u; 
        } 
        delete v; 
        u->father = father; 
        if (uvBlack) { 
          // u and v both black, fix double black at u 
          fixDoubleBlack(u); 
        } else { 
          // u or v red, color u black 
          u->color = BLACK; 
        } 
      } 
      return; 
    } 
  
    // v has 2 children, swap values with sucessor and recurse 
    swapValues(u, v); 
    removeNode(u); 
  } 
  
  void fixDoubleBlack(No *x) { 
    if (x == root) 
      // Reached root 
      return; 
  
    No *brother = x->brother(), *father = x->father; 
    if (brother == NULL) { 
      // No sibiling, double black pushed up 
      fixDoubleBlack(father); 
    } else { 
      if (brother->color == RED) { 
        // brother red 
        father->color = RED; 
        brother->color = BLACK; 
        if (brother->isOnLeft()) { 
          // left case 
          giraDireita(father); 
        } else { 
          // right case 
          giraEsquerda(father); 
        } 
        fixDoubleBlack(x); 
      } else { 
        // brother black 
        if (brother->hasRedChild()) { 
          // at least 1 red children 
          if (brother->left != NULL and brother->left->color == RED) { 
            if (brother->isOnLeft()) { 
              // left left 
              brother->left->color = brother->color; 
              brother->color = father->color; 
              giraDireita(father); 
            } else { 
              // right left 
              brother->left->color = father->color; 
              giraDireita(brother); 
              giraEsquerda(father); 
            } 
          } else { 
            if (brother->isOnLeft()) { 
              // left right 
              brother->right->color = father->color; 
              giraEsquerda(brother); 
              giraDireita(father); 
            } else { 
              // right right 
              brother->right->color = brother->color; 
              brother->color = father->color; 
              giraEsquerda(father); 
            } 
          } 
          father->color = BLACK; 
        } else { 
          // 2 black children 
          brother->color = RED; 
          if (father->color == BLACK) 
            fixDoubleBlack(father); 
          else
            father->color = BLACK; 
        } 
      } 
    } 
  } 
  
  // prints level order for given No 
  void levelOrder(No *x) { 
    if (x == NULL) 
      // return if No is null 
      return; 
  
    // queue for level order 
    queue<No *> q; 
    No *curr; 
  
    // push x 
    q.push(x); 
  
    while (!q.empty()) { 
      // while q is not empty 
      // dequeue 
      curr = q.front(); 
      q.pop(); 
  
      // print No value 
      cout << curr->val << " "; 
  
      // push children to queue 
      if (curr->left != NULL) 
        q.push(curr->left); 
      if (curr->right != NULL) 
        q.push(curr->right); 
    } 
  } 
  
  // prints inorder recursively 
  void inorder(No *x) { 
    if (x == NULL) 
      return; 
    inorder(x->left); 
    cout << x->val << " "; 
    inorder(x->right); 
  } 
  
public: 
  // constructor 
  // initialize root 
  RBTree() { root = NULL; } 
  
  No *getRoot() { return root; } 
  
  // searches for given value 
  // if found returns the No (used for delete) 
  // else returns the last No while traversing (used in insert) 
  No *search(int n) { 
    No *aux = root; 
    while (aux != NULL) { 
      if (n < aux->val) { 
        if (aux->left == NULL) 
          break; 
        else
          aux = aux->left; 
      } else if (n == aux->val) { 
        break; 
      } else { 
        if (aux->right == NULL) 
          break; 
        else
          aux = aux->right; 
      } 
    } 
  
    return aux; 
  } 
  
  // inserts the given value to tree 
  void insert(int n) { 
    No *no = new No(n); 
    if (root == NULL) { 
      // when root is null 
      // simply insert value at root 
      no->color = BLACK; 
      root = no; 
    } else { 
      No *aux = search(n); 
  
      if (aux->val == n) { 
        // return if value already exists 
        return; 
      } 
  
      // if value is not found, search returns the No 
      // where the value is to be inserted 
  
      // connect new No to correct No 
      no->father = aux; 
  
      if (n < aux->val) 
        aux->left = no; 
      else
        aux->right = no; 
  
      // fix red red voilaton if exists 
      fixRedRed(no); 
    } 
  } 
  
  // utility function that deletes the No with given value 
  void deleteByVal(int n) { 
    if (root == NULL) 
      // Tree is empty 
      return; 
  
    No *v = search(n), *u; 
  
    if (v->val != n) { 
      cout << "No No found to delete with value:" << n << endl; 
      return; 
    } 
  
    removeNode(v); 
  } 
  
  // prints inorder of the tree 
  void printInOrder() { 
    cout << "Inorder: " << endl; 
    if (root == NULL) 
      cout << "Tree is empty" << endl; 
    else
      inorder(root); 
    cout << endl; 
  } 
  
  // prints level order of the tree 
  void printLevelOrder() { 
    cout << "Level order: " << endl; 
    if (root == NULL) 
      cout << "Tree is empty" << endl; 
    else
      levelOrder(root); 
    cout << endl; 
  } 
}; 
  
int main() { 
  RBTree tree; 
  
  tree.insert(7); 
  tree.insert(3); 
  tree.insert(18); 
  tree.insert(10); 
  tree.insert(22); 
  tree.insert(8); 
  tree.insert(11); 
  tree.insert(26); 
  tree.insert(2); 
  tree.insert(6); 
  tree.insert(13); 
  
  tree.printInOrder(); 
  tree.printLevelOrder(); 
  
  cout<<endl<<"------------ Deleting 18, 11, 3, 10, 22 -------------"<<endl; 
  
  tree.deleteByVal(18); 
  tree.deleteByVal(11); 
  tree.deleteByVal(3); 
  tree.deleteByVal(10); 
  tree.deleteByVal(22); 
  
  tree.printInOrder(); 
  tree.printLevelOrder(); 
  return 0; 
} 
