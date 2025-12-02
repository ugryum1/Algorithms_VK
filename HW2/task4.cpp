/*
Задача 4. Использование АВЛ-дерева

Требование для всех вариантов Задачи 4
Решение должно поддерживать передачу функции сравнения снаружи.

4_1. Солдаты. В одной военной части решили построить в одну шеренгу по росту.
Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя,
а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены
по росту – сначала самые высокие, а в конце – самые низкие. За расстановку солдат
отвечал прапорщик, который заметил интересную особенность – все солдаты в части
разного роста. Ваша задача состоит в том, чтобы помочь прапорщику правильно
расставлять солдат, а именно для каждого приходящего солдата указывать,
перед каким солдатом в строе он должен становится.

Требования: скорость выполнения команды - O(log n).

Формат входных данных.
Первая строка содержит число N – количество команд (1 ≤ N ≤ 30 000).
В каждой следующей строке содержится описание команды:
число 1 и X если солдат приходит в строй
(X – рост солдата, натуральное число до 100 000 включительно)
и число 2 и Y если солдата, стоящим в строе на месте Y надо удалить из строя.
Солдаты в строе нумеруются с нуля.

Формат выходных данных.
На каждую команду 1 (добавление в строй) вы должны выводить число K – номер позиции,
на которую должен встать этот солдат (все стоящие за ним двигаются назад).

in              out
--------------------
5               0
1 100           0
1 200           2
1 50            1
2 1
1 150

*/

#include <iostream>
#include <functional>


template <typename T, class Comparator = std::greater<T>>
class AvlTree {
    struct Node {
        Node(const T &data) : data(data), left(nullptr), right(nullptr),
                              height(1), count(1) {}

        T data;
        Node *left;
        Node *right;
        size_t height;
        size_t count; // количество узлов поддерева
    };

public:
    AvlTree(Comparator comp = Comparator()) : root(nullptr), comp(comp) {}

    ~AvlTree(){
        destroyTree(root);
    }

    void Add(const T &data) {
        root = addInternal(root, data);
    }

    bool Has(const T &data) {
        Node *tmp = root;
        while (tmp) {
            if (!comp(tmp->data, data) && !comp(data, tmp->data)) {
                return true;
            } else if (comp(data, tmp->data)) {
                tmp = tmp->left;
            } else {
                tmp = tmp->right;
            }
        }

        return false;
    }

    void Delete(const T &data) {
        root = deleteInternal(root, data);
    }

    // найти позицию для вставки - сколько элементов больше данного
    size_t GetPosition(const T &data) {
        return getPosition(root, data);
    }

    // найти элемент по позиции
    T GetByPosition(size_t position) {
        return getByPosition(root, position);
    }

    // удалить элемент по позиции
    void DeleteByPosition(size_t position) {
        T data = GetByPosition(position);
        Delete(data);
    }

private:
    Node *root;
    Comparator comp;

    size_t getCount(Node *node) {
        return node ? node->count : 0;
    }

    void fixCount(Node *node) {
        if (node) {
            node->count = getCount(node->left) + getCount(node->right) + 1;
        }
    }

    void destroyTree(Node *node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    Node* findAndRemoveMin(Node *node, Node *&minNode) {
        if (!node->left) {
            minNode = node;
            return node->right;
        }

        node->left = findAndRemoveMin(node->left, minNode);
        fixCount(node);
        return doBalance(node);
    }

    Node* findAndRemoveMax(Node *node, Node *&maxNode) {
        if (!node->right) {
            maxNode = node;
            return node->left;
        }

        node->right = findAndRemoveMax(node->right, maxNode);
        fixCount(node);
        return doBalance(node);
    }


    Node* deleteInternal(Node *node, const T &data) {
        if (!node) {
            return nullptr;
        }

        if (comp(data, node->data)) {
            node->left = deleteInternal(node->left, data);
        } else if (comp(node->data, data)) {
            node->right = deleteInternal(node->right, data);
        } else {
            Node *left = node->left;
            Node *right = node->right;

            delete node;

            if (!right) {
                return left;
            }
            if (!left) {
                return right;
            }

            // выбираем поддерево на основе сравнения глубин
            // (берем из более глубокого)
            if (getHeight(left) > getHeight(right)) {
                // берем максимальный элемент из левого поддерева
                Node *maxNode = nullptr;
                left = findAndRemoveMax(left, maxNode);
                maxNode->left = left;
                maxNode->right = right;
                fixCount(maxNode);
                return doBalance(maxNode);
            } else {
                // берем минимальный элемент из правого поддерева
                Node *minNode = nullptr;
                right = findAndRemoveMin(right, minNode);
                minNode->right = right;
                minNode->left = left;
                fixCount(minNode);
                return doBalance(minNode);
            }
        }

        fixCount(node);
        return doBalance(node);
    }

    Node* addInternal(Node *node, const T &data) {
        if (!node) {
            return new Node(data);
        }

        if (comp(data, node->data)) {
            node->left = addInternal(node->left, data);
        } else {
            node->right = addInternal(node->right, data);
        }

        fixCount(node);
        return doBalance(node);
    }

    size_t getHeight(Node *node) {
        return node ? node->height : 0;
    }

    void fixHeight(Node *node) {
        if (node) {
            node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
        }
    }

    int getBalance(Node *node) {
        return getHeight(node->right) - getHeight(node->left);
    }

    Node* rotateLeft(Node *node) {
        Node *tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        fixHeight(node);
        fixHeight(tmp);
        fixCount(node);
        fixCount(tmp);
        return tmp;
    }

    Node* rotateRight(Node *node) {
        Node *tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        fixHeight(node);
        fixHeight(tmp);
        fixCount(node);
        fixCount(tmp);
        return tmp;
    }

    Node* doBalance(Node *node) {
        if (!node) {
            return nullptr;
        }

        fixHeight(node);
        fixCount(node);

        switch (getBalance(node)) {
            case 2:
                if (getBalance(node->right) < 0) {
                    node->right = rotateRight(node->right);
                }
                return rotateLeft(node);
            case -2:
                if (getBalance(node->left) > 0) {
                    node->left = rotateLeft(node->left);
                }
                return rotateRight(node);
            default:
                return node;
        }
    }

    // получить позицию элемента (сколько элементов больше него)
    size_t getPosition(Node* node, const T& data) {
    if (!node) {
        return 0;
    }

    if (!comp(node->data, data) && !comp(data, node->data)) {
        return getCount(node->left);
    }
    if (comp(data, node->data)) {
        return getPosition(node->left, data);
    } else {
        return getCount(node->left) + 1 + getPosition(node->right, data);
    }
}

    // получить элемент по позиции
    T getByPosition(Node* node, size_t position) {
        size_t leftCount = getCount(node->left);

        if (position == leftCount) {
            return node->data;
        } else if (position < leftCount) {
            return getByPosition(node->left, position);
        } else {
            return getByPosition(node->right, position - leftCount - 1);
        }
    }
};

// компаратор для сортировки по убыванию роста
struct ReverseComparator {
    bool operator()(int a, int b) const {
        return a > b;
    }
};

int main() {
    int n;
    std::cin >> n;

    AvlTree<int, ReverseComparator> tree;
    for (int i = 0; i < n; ++i) {
        int command;
        std::cin >> command;

        if (command == 1) {
            int x;
            std::cin >> x;
            size_t pos = tree.GetPosition(x);
            std::cout << pos << std::endl;
            tree.Add(x);
        } else if (command == 2) {
            int y;
            std::cin >> y;
            tree.DeleteByPosition(y);
        }
    }
    return 0;
}
