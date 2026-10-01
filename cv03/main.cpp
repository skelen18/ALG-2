//nedokoncená implementace AVL stromu, ale v hodine bylo moc horko a uz jsem umiral
#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

using keyType = int;

using std::vector;

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
    Node* root;

    AVLTree() : root(nullptr) {}

    ~AVLTree() {
        clear(root);    
    }

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

    void erase(keyType item) {
        root = erase(root, item);
    }

    void postorderPrint() const {
        postorderPrint(root);
        std::cout << "\n";
    }

    void postorderPrint(Node* node) const {
        if(node == nullptr) {
            return;
        }
        postorderPrint(node->left);
        postorderPrint(node->right);
        std::cout << node->key << " ";
    }
    
    void preOrderPrint() const {
        preOrderPrint(root);
        std::cout << "\n";
    }

    void preOrderPrint(Node* node) const {
        if(node == nullptr) {
            return;
        }
        std::cout << node->key << " ";
        preOrderPrint(node->left);
        preOrderPrint(node->right);
    }

    void inOrderPrint() const {
        inOrderPrint(root);
        std::cout << "\n";
    }

    void printLevelOrder() const {
        if (root == nullptr) {
            std::cout << "\n";
            return;
        }

        std::queue<const Node*> nodes;
        nodes.push(root);

        while (!nodes.empty()) {
            const std::size_t levelSize = nodes.size();
            for (std::size_t i = 0; i < levelSize; ++i) {
                const Node* node = nodes.front();
                nodes.pop();
                std::cout << node->key << " ";

                if (node->left != nullptr) {
                    nodes.push(node->left);
                }
                if (node->right != nullptr) {
                    nodes.push(node->right);
                }
            }
            std::cout << "\n";
        }
    }

private:
    void clear(Node* node) {
        if(node == nullptr) {
            return;
        }
        clear(node->left);
        clear(node->right);
        delete node;
    }

    

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

    static Node* minimum(Node* node) {
        while(node != nullptr && node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    Node* erase(Node* node, keyType item) {
        if(node == nullptr) {
            return nullptr;
        }

        if(item < node->key) {
            node->left = erase(node->left, item);
        } else if(item > node->key) {
            node->right = erase(node->right, item);
        } else {
            if(node->left == nullptr || node->right == nullptr) {
                Node* child = node->left != nullptr ? node->left : node->right;
                delete node;
                return child;
            }

            Node* successor = minimum(node->right);
            node->key = successor->key;
            node->right = erase(node->right, successor->key);
        }

        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
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

vector<int> readIntegersFromFile(const std::string& filename) {
    std::ifstream file(filename);
    vector<int> numbers;
    int number;

    while (file >> number) {
        numbers.push_back(number);
    }

    return numbers;
}

int main(int argc, char* argv[] ) {

    vector<int> numbers = readIntegersFromFile("input.txt");
    AVLTree tree;

    for(int number : numbers) {
        tree.insert(number);
    }

    tree.postorderPrint();
    tree.preOrderPrint();
    tree.inOrderPrint();
    tree.updateHeight(tree.root);
    tree.printLevelOrder();

    
    return 0;
}