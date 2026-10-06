#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>


using itemType = int;

struct Node {
    int key;
    Node* left;
    Node* right;
    int heigth;
    Node(int k) : key(k), left(nullptr), right(nullptr), heigth(0) {}
};


class AVLTree {
private:
    Node* root;

    int heigth(Node* node){
        if( node == nullptr ) return -1;
        return node->heigth;
    }


    int balanceFactor(Node* node){
        if( node == nullptr ) return 0;
        return heigth(node->left) - heigth(node->right);
    }

    void updateHeigth(Node* node){
        if( node == nullptr ) return;
        node->heigth = 1 + std::max(heigth(node->left), heigth(node->right));
    }

    Node* rotateRight(Node* r) {

        Node* c = r->left;

        Node* t2 = c->right;

        c->right = r;
        r->left = t2;

        updateHeigth(r);
        updateHeigth(c);

        return c;
    }

    Node* rotateLeft(Node* r){

        Node* c = r->right;

        Node* t2 = c->left;

        c->left = r;
        r->right = t2;

        updateHeigth(r);
        updateHeigth(c);

        return c;
    }

    Node* balance(Node* node) {
        updateHeigth(node );

        int bf = balanceFactor(node);

        if (bf > 1) {
            if (balanceFactor(node->left) < 0){
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }


        if (bf < -1) {
            if (balanceFactor(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }

        return node;

    }

    Node* insert(Node* node, itemType key){
        if( node == nullptr ){
            return new Node(key);
        }

        if(key < node->key ){
            node->left = insert(node->left, key);
        } 
        else if (key > node->key){
            node->right = insert(node->right, key);
        } 
        else {
            return node;
        }

        return balance(node);

    }

    bool containsKey(Node* node, itemType key){
        if( node == nullptr ) return false;

        if( key < node->key){
            return containsKey(node->left, key);
        } 
        else if (key > node->key){
            return containsKey(node->right, key);
        } 
        else {
            return true;
        }
    }

    Node* findMin(Node* node){
        Node* curr = node;
        while(curr->left != nullptr){
            curr = curr->left;
        }

        return curr;
    }

    Node* erase(Node* node, itemType key){
        if(node == nullptr){
            return nullptr;
        }

        if(key < node->key){
            node->left =erase(node->left, key);
        } 
        else if(key > node->key){
            node->right =erase(node->right, key);
        } 
        else {
            if(node->left == nullptr){
                Node* temp = node->right;
                delete node;
                return temp;
            } else if(node->right == nullptr){
                Node* temp = node->left;
                delete node;
                return temp;
            }

            Node* temp = findMin(node->right);

            node->key = temp->key;
            node->right = erase(node->right, temp->key);
        }

        return balance(node);
    }

    void inorderPrint(Node* node) const {
        if(node == nullptr) return;
        inorderPrint(node->left);

        std::cout << node->key << " ";
        inorderPrint(node->right);
    }

    void preorderPrint(Node* node) const {
        if(node == nullptr) return;
        std::cout << node->key << " ";

        preorderPrint(node->left);
        preorderPrint(node->right);
    }

    void postorderPrint(Node* node) const {
        if(node == nullptr) return;
        postorderPrint(node->left);
        postorderPrint(node->right);

        std::cout << node->key << " ";
    }

    void clear(Node* node){
        if( node == nullptr ) return;
        clear(node->left);
        clear(node->right);

        delete node;
    }

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree(){
        clear(root);
    }

    void insert(itemType key) {
        root = insert(root, key);
    }

    void erase(itemType key) {
        root = erase(root, key);
    }

    bool containsKey(itemType key){
        return containsKey(root, key);
    }

    void printPreorder() const {
        preorderPrint(root);
        std::cout << "\n";
    }

    void printPostorder() const {
        postorderPrint(root);
        std::cout << "\n";
    }

    void printInorder() const {
        inorderPrint(root);
        std::cout << "\n";
    }
};

std::vector<int> readIntegersFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<int> numbersVec;

    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return numbersVec;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream lineAsStream(line);
        int num;
        while (lineAsStream >> num) {
            numbersVec.push_back(num);
        }
    }

    file.close();
    return numbersVec;
}

int main(int argc, char* argv[]){
    if (argc < 3) {
        return 1;
    }

    std::vector<int> data = readIntegersFromFile(argv[1]);
    std::vector<int> toDelete = readIntegersFromFile(argv[2]);

    
    AVLTree tree;
    for(const int item : data){
        tree.insert(item);
    }

    for(const int item : toDelete){
        tree.erase(item);
    }

    tree.printPostorder();
    tree.printPreorder();
    tree.printInorder();

    return 0;
}