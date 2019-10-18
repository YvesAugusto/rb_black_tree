/** C++ implementation for Red-Black Tree Insertion 
   This code is adopted from the code provided by 
   Dinesh Khandelwal in comments **/
#include <bits/stdc++.h> 
using namespace std; 
  
enum Color {RED, BLACK}; 
  
struct No{ 
    int data; 
    bool color; 
    No *left, *right, *father; 
  
    // Constructor 
    No(int data) 
    { 
       this->data = data; 
       left = right = father = NULL; 
       this->color = RED; 
    } 
}; 
  
// Class to represent Red-Black Tree 
class RBTree { 
private: 
    No *root; 
protected: 
    void giraEsquerda(No *&, No *&); 
    void giraDireita(No *&, No *&); 
    void Corrigir(No *&, No *&); 
public: 
    // Construtor 
    RBTree() { root = NULL; } 
    void insert(const int &n); 
    void inorder(); 
    void levelOrder(); 
}; 
  
// Ordem contrária
void inorderHelper(No *root) { 
    if (root == NULL) 
        return; 
  
    inorderHelper(root->left); 
    cout << root->data << "  "; 
    inorderHelper(root->right); 
} 
  
// Inserir
No* insertBST(No* root, No *pt) { 
    // Se a árvore esvaziou-se, retorna um novo nó
    if (root == NULL) 
       return pt; 
  
    // Caso não, prossegue
    if (pt->data < root->data) 
    { 
        root->left  = insertBST(root->left, pt); 
        root->left->father = root; 
    } 
    else if (pt->data > root->data) 
    { 
        root->right = insertBST(root->right, pt); 
        root->right->father = root; 
    } 
  
    // Retorna o ponteiro sem alterações quaisquer
    return root; 
} 
  
// Ordena inversamente 
void levelOrderHelper(No *root) { 
    if (root == NULL) 
        return; 
  
    std::queue<No *> q; 
    q.push(root); 
  
    while (!q.empty()) 
    { 
        No *temp = q.front(); 
        cout << temp->data << "  "; 
        q.pop(); 
  
        if (temp->left != NULL) 
            q.push(temp->left); 
  
        if (temp->right != NULL) 
            q.push(temp->right); 
    } 
} 
  
void RBTree::giraEsquerda(No *&root, No *&pt) { 
    No *pt_right = pt->right; 
  
    pt->right = pt_right->left; 
  
    if (pt->right != NULL) 
        pt->right->father = pt; 
  
    pt_right->father = pt->father; 
  
    if (pt->father == NULL) 
        root = pt_right; 
  
    else if (pt == pt->father->left) 
        pt->father->left = pt_right; 
  
    else
        pt->father->right = pt_right; 
  
    pt_right->left = pt; 
    pt->father = pt_right; 
} 
  
void RBTree::giraDireita(No *&root, No *&pt) { 
    No *pt_left = pt->left; 
  
    pt->left = pt_left->right; 
  
    if (pt->left != NULL) 
        pt->left->father = pt; 
  
    pt_left->father = pt->father; 
  
    if (pt->father == NULL) 
        root = pt_left; 
  
    else if (pt == pt->father->left) 
        pt->father->left = pt_left; 
  
    else
        pt->father->right = pt_left; 
  
    pt_left->right = pt; 
    pt->father = pt_left; 
} 
  
// Conserta os erros, ajustando a árvore às regras 
void RBTree::Corrigir(No *&root, No *&pt) { 
    No *father_pt = NULL; 
    No *grand_father_pt = NULL; 
  
    while ((pt != root) && (pt->color != BLACK) && 
           (pt->father->color == RED)) { 
  
        father_pt = pt->father; 
        grand_father_pt = pt->father->father; 
  
        /*  1 caso: 
            Pai do nó é filho esquero do avô do nó*/
        if (father_pt == grand_father_pt->left) { 
  
            No *uncle_pt = grand_father_pt->right; 
  
            /* Case : 1 
               O tio do nó é vermelho
            */
            if (uncle_pt != NULL && uncle_pt->color == RED) 
            { 
                grand_father_pt->color = RED; 
                father_pt->color = BLACK; 
                uncle_pt->color = BLACK; 
                pt = grand_father_pt; 
            } 
  
            else{ 
                /* Case : 2 
                   o nó é filho direito do seu pai 
                   giraEsquerda */
                if (pt == father_pt->right) 
                { 
                    giraEsquerda(root, father_pt); 
                    pt = father_pt; 
                    father_pt = pt->father; 
                } 
  
                /* Case : 3 
                   O nó é filho esquerda de seu pai 
                   giraDireita */
                giraDireita(root, grand_father_pt); 
                swap(father_pt->color, grand_father_pt->color); 
                pt = father_pt; 
            } 
        } 

        /* Case : B 
           Pai do nó é filho direito do avô do nó */
        else{ 
            No *uncle_pt = grand_father_pt->left; 
  
            /*  Case : 1 
                O tio do nó é igualmente vermehlo 
                Troca cor */
            if ((uncle_pt != NULL) && (uncle_pt->color == RED)) 
            { 
                grand_father_pt->color = RED; 
                father_pt->color = BLACK; 
                uncle_pt->color = BLACK; 
                pt = grand_father_pt; 
            } 
            else
            { 
                /* Case : 2 
                   O nó é filho esquerdo de seu pai 
                   giraDireita */
                if (pt == father_pt->left) 
                { 
                    giraDireita(root, father_pt); 
                    pt = father_pt; 
                    father_pt = pt->father; 
                } 
  
                /* Case : 3 
                   O nó é filho direito do seu pai
                   giraEsquerda */
                giraEsquerda(root, grand_father_pt); 
                swap(father_pt->color, grand_father_pt->color); 
                pt = father_pt; 
            } 
        } 
    } 
  
    root->color = BLACK; 
} 
  
// Function to insert a new No with given data 
void RBTree::insert(const int &data) { 
    No *pt = new No(data); 
  
    // Do a normal BST insert 
    root = insertBST(root, pt); 
  
    // fix Red Black Tree violations 
    Corrigir(root, pt); 
} 
  
// Function to do inorder and level order traversals 
void RBTree::inorder()     {  inorderHelper(root);} 
void RBTree::levelOrder()  {  levelOrderHelper(root); } 
  
// Driver Code 
int main() { 
    RBTree tree; 
  
    tree.insert(7); 
    tree.insert(6); 
    tree.insert(5); 
    tree.insert(4); 
    tree.insert(3); 
    tree.insert(2); 
    tree.insert(1); 
  
    cout << "INor Traversal of Created Tree\n"; 
    tree.inorder(); 
  
    cout << "\n\nLevel Order Traversal of Created Tree\n"; 
    tree.levelOrder(); 
  
    return 0; 
} 
