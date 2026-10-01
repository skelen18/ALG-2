//nedokoncená implementace AVL stromu, ale v hodine bylo moc horko a uz jsem umiral
#include <iostream>
#include <algorithm>

using keyType = int;

class Node {
public:
    Node* left;
    Node* right;
    keyType key;
    int height;
    

    Node(keyType item) : left(nullptr), right(nullptr), key(item), height(0) {}
};


class AVLTree {
public:
    AVLTree() : root(nullptr) {}

    bool find(keyType item) const {
        return find(root,item);
    }

    void updateHeight(Node* node) {
        if(node == nullptr) {
            return;
        }
        updateHeight(node->left);
        updateHeight(node->right);
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    }

    void insert(keyType item) {
        root = insert(root, item);
    }

    void inOrderPrint() const {
        inOrderPrint(root);
        std::cout << "\n";
    }

private:
    Node* root;

    static int getHeight(const Node* node) {
        return node == nullptr ? -1 : node->height;
    }

    bool find(Node* node, keyType item) const {
        if(node == nullptr) {
            return false;
        }
        if(item < node->key) {
            return find(node->left, item);
        } else if(item > node->key) {
            return find(node->right, item);
        } else {
            return true;
        }
    }



    Node* insert(Node* node, keyType item) {
        if(node == nullptr) {
            return new Node(item);
        }
        if(item < node->key) {
            node->left = insert(node->left, item);
        } else if(item > node->key) {
            node->right = insert(node->right, item);
        } else {
            return node;
        }
        updateHeight(node);
        return balance(node);
    }

    static int balanceFactor(const Node* node) {
        return node == nullptr ? 0 : getHeight(node->left) - getHeight(node->right);
    }

    static Node* rotateRight(Node* node) {
        Node* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
        newRoot->height = 1 + std::max(getHeight(newRoot->left), getHeight(newRoot->right));
        return newRoot;
    }

    static Node* rotateLeft(Node* node) {
        Node* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
        newRoot->height = 1 + std::max(getHeight(newRoot->left), getHeight(newRoot->right));
        return newRoot;
    }

    static Node* balance(Node* node) {
        const int factor = balanceFactor(node);
        if(factor > 1) {
            if(balanceFactor(node->left) < 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }
        if(factor < -1) {
            if(balanceFactor(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }
        return node;
    }


    void inOrderPrint (Node* node) const {
        if(node == nullptr) {
            return;
        }
        inOrderPrint(node->left);
        std::cout << node->key << " ";
        inOrderPrint(node->right);
    }
};

int main() {
    AVLTree tree;
    return 0;
}