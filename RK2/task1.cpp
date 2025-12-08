#include <iostream>
#include <sstream>
#include <string>

struct Node {
    int value;
    Node *left;
    Node *right;

    Node(const int &value) : value(value), left(nullptr), right(nullptr) {}
};

class Tree {
    public:
        Tree() : root(nullptr) {}
        ~Tree() {
            clear(root);
        }

        void add(const int &value) {
            if (root == nullptr) {
                root = new Node(value);
                return;
            }

            Node *current = root;

            while (true) {
                if (value >= current->value) {
                    // Идем вправо
                    if (current->right != nullptr) {
                        current = current->right;
                    } else {
                        current->right = new Node(value);
                        return;
                    }
                } else {
                    // Идем влево
                    if (current->left != nullptr) {
                        current = current->left;
                    } else {
                        current->left = new Node(value);
                        return;
                    }
                }
            }
        }

        Node* getRoot() {
            return root;
        }
    private:
        Node *root;

        void clear(Node *node) {
            if (node != nullptr) {
                clear(node->left);
                clear(node->right);
                delete node;
            }
        }
};

bool checkSimilarity(Node *node, int firstValue) {
    if (node == nullptr) {
        return true;
    }

    if (node->value != firstValue) {
        return false;
    }

    return (checkSimilarity(node->left, firstValue) &&
            checkSimilarity(node->right, firstValue));
}

bool isSimilarValues(Node *root) {
    if (root == nullptr) {
        return true;
    }

    int firstValue = root->value;
    return checkSimilarity(root, firstValue);
}

int main() {
    Tree tree;
    std::string line;

    std::getline(std::cin, line);

    std::istringstream iss(line);
    int value;

    while (iss >> value) {
        tree.add(value);
    }

    if (isSimilarValues(tree.getRoot())) {
        std::cout << 1 << std::endl;
    } else {
        std::cout << 0 << std::endl;
    }

    return 0;
}
