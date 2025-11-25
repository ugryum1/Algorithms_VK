/*
Дано число N < 10**6 и последовательность целых чисел из [-2**31..2**31] длиной N.

Требуется построить бинарное дерево, заданное наивным порядком вставки.

Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
то узел K добавляется в правое поддерево root; иначе в левое поддерево root.

Требования: Рекурсия запрещена.
Решение должно поддерживать передачу функции сравнения снаружи.

2_3. Выведите элементы в порядке post-order (снизу вверх).
in          out
------------------
3           1 3 2
2 1 3
------------------
3           3 2 1
1 2 3
------------------
3           2 1 3
3 1 2
*/


#include <iostream>
#include <vector>

// Узел дерева
template <typename T>
struct Node {
    T value;
    Node *left;
    Node *right;

    Node(const T &value) : value(value), left(nullptr), right(nullptr) {}
};

// Базовый компаратор
template <typename T>
struct DefaultComparator {
    bool operator()(const T &left, const T &right) const {
        return left < right;
    }
};

// Компаратор для узлов дерева
template <typename T>
struct NodeComparator {
    bool operator() (const Node<T> &left, const Node<T> &right) const {
        return left.value < right.value;
    }
};

// Класс дерево
template <typename T, typename Comparator = DefaultComparator<T>>
class Tree {
    public:
        Tree() : root(nullptr) {}
        ~Tree();

        void add(const T &value);
        void print() const;
    private:
        Node<T> *root;
        Comparator comp;
};

// Деструктор
template <typename T, typename Comparator>
Tree<T, Comparator>::~Tree() {
    if (root == nullptr) {
        return;
    }

    std::vector<Node<T>*> stack{};
    stack.push_back(root);

    while (!stack.empty()) {
        Node<T>* node = stack.back();
        stack.pop_back();

        if (node->left != nullptr) {
            stack.push_back(node->left);
        }
        if (node->right != nullptr) {
            stack.push_back(node->right);
        }

        delete node;
    }
}

// Функция добавления элемента в дерево
template <typename T, typename Comparator>
void Tree<T, Comparator>::add(const T &value) {
    // Если в корне ничего нет - записываем в корень
    if (root == nullptr) {
        root = new Node<T>(value);
        return;
    }

    Node<T> *current = root;

    // Для сравнение компаратором
    Node<T> temp(value);

    while (true) {
        if ( !comp(temp, *current) /* эквивалентно value >= current->value */) {
            // Идём в правое поддерево
            if (current->right == nullptr) {
                current->right = new Node<T>(value);
                return;
            }
            current = current->right;
        } else {
            // Идём в левое поддерево
            if (current->left == nullptr) {
                current->left = new Node<T>(value);
                return;
            }
            current = current->left;
        }
    }
}

// Печать дерева
template <typename T, typename Comparator>
void Tree<T, Comparator>::print() const {
    if (root == nullptr) {
        return;
    }

    std::vector<Node<T>*> stack{};

    Node<T> *previous = nullptr;
    Node<T> *current = root;

    while (!stack.empty() || current != nullptr) {
        // Двигаемся влево до упора
        if (current != nullptr) {
            stack.push_back(current);
            current = current->left;
        } else {
            // Последний посещённый элемент
            Node<T> *node = stack.back();

            // Если у него есть правый непосещённый потомок, переходим в него
            if (node->right != nullptr && node->right != previous) {
                current = node->right;
            } else {
                // Иначе можно печатать
                std::cout << node->value << " ";
                previous = node;
                stack.pop_back();
            }
        }
    }

    std::cout << std::endl;
}

int main() {
    int n{};
    long long int value{};
    std::cin >> n;

    Tree<long long int, NodeComparator<long long int>> tree;

    for (size_t i = 0; i < n; ++i) {
        std::cin >> value;
        tree.add(value);
    }

    tree.print();

    return 0;
}
