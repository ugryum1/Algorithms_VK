#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

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

int minDepth(Node *root) {
    if (root == nullptr) {
        return 0;
    }

    if (root->left == nullptr && root->right == nullptr) {
        return 1;
    }

    if (root->left == nullptr) {
        return 1 + minDepth(root->right);
    }

    if (root->right == nullptr) {
        return 1 + minDepth(root->left);
    }

    return 1 + std::min(minDepth(root->left), minDepth(root->right));
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

    std::cout << minDepth(tree.getRoot()) << std::endl;

    return 0;
}
