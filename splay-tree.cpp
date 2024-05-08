#include <bits/stdc++.h>
using namespace std;

ifstream fin ("abce.in");
ofstream fout ("abce.out");

struct node {
    int key;
    bool isEmpty;
    node *left, *right;
};

node* newNode(int key) {
    node* Node = new node();
    Node->key = key;
    Node->left = Node->right = NULL;
    return (Node);
}

node *rightRotate(node *x) {
    node *y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

node *leftRotate(node *x) {
    node *y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

node *splay(node *root, int key) {
    if (root == NULL || root->key == key)
        return root;
    if (root->key > key) {
        if (root->left == NULL) return root;
        if (root->left->key > key) {
            root->left->left = splay(root->left->left, key);
            root = rightRotate(root);
        } else if (root->left->key < key) {
            root->left->right = splay(root->left->right, key);
            if (root->left->right != NULL)
                root->left = leftRotate(root->left);
        }
        return (root->left == NULL)? root: rightRotate(root);
    } else {
        if (root->right == NULL) return root;
        if (root->right->key > key) {
            root->right->left = splay(root->right->left, key);
            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        } else if (root->right->key < key) {
            root->right->right = splay(root->right->right, key);
            root = leftRotate(root);
        }
        return (root->right == NULL)? root: leftRotate(root);
    }
}

node *insert(node *root, int k) {
    if (root == NULL) return newNode(k);
    root = splay(root, k);
    if (root->key == k) return root;
    node *newnode = newNode(k);
    if (root->key > k) {
        newnode->right = root;
        newnode->left = root->left;
        root->left = NULL;
    } else {
        newnode->left = root;
        newnode->right = root->right;
        root->right = NULL;
    }
    return newnode;
}

void preOrder(node *root) {
    if (root != NULL) {
        fout << root->key << " ";
        preOrder(root->left);
        preOrder(root->right);
    }
}

void inOrder(node *root, int val1, int val2) {
    if (root != NULL) {
        inOrder(root->left, val1, val2);
        if (root->key >= val1 && root -> key <= val2)
            fout << root->key << " ";
        inOrder(root->right, val1, val2);
    }
}

node *search(node *root, int key) {
    return splay(root, key);
}

node* delete_key(node* root, int key) {
    node* temp;
    if (!root)
        return NULL;
    root = splay(root, key);
    if (key != root->key)
        return root;
    if (!root->left) {
        temp = root;
        root = root->right;
    } else {
        temp = root;
        root = splay(root->left, key);
        root->right = temp->right;
    }
    free(temp);
    return root;
}

node* get_pred(node* root, int key) {
    if (!root)
        return NULL;
    node* pred = NULL;
    while (root) {
        if (root->key < key) {
            pred = root;
            root = root->right;
        } else if (root->key > key) {
            root = root->left;
        } else {
            return root;
        }
    }
    return pred;
}

node* get_succ(node* root, int key) {
    if (!root)
        return NULL;
    node* succ = NULL;
    while (root) {
        if (root->key > key) {
            succ = root;
            root = root->left;
        } else if (root->key < key) {
            root = root->right;
        } else {
            return root;
        }
    }
    return succ;
}

int main() {
    node *root = new node();
    root->isEmpty = true;
    int q;
    fin >> q;
    while (q--) {
        int task, nr;
        fin >> task >> nr;
        if (task == 1) {
            if (root->isEmpty) {
                root = newNode(nr);
                root->isEmpty = false;
            } else {
                root = insert(root, nr);
            }
        } else if (task == 2) {
            if (!root->isEmpty)
                root = delete_key(root, nr);
        } else if (task == 3) {
            if (!root->isEmpty) {
                root = search(root, nr);
                if (root->key == nr)
                    fout << "1\n";
                else fout << "0\n";
            } else fout << "0\n";
        } else if (task == 4) {
            fout << get_pred(root, nr)->key << "\n";
        } else if (task == 5) {
            fout << get_succ(root, nr)->key << "\n";
        } else if (task == 6) {
            int nr2;
            fin >> nr2;
            inOrder(root, nr, nr2);
            fout << "\n";
        }
    }
    return 0;
}
