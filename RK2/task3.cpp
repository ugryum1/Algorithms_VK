#include <iostream>
#include <string>
#include <unordered_set>
#include <sstream>

struct Node {
    std::string product_name;
    int color;
    int size;

    bool operator==(const Node &other) const {
        return product_name == other.product_name &&
               color == other.color &&
               size == other.size;
    }
};

namespace std {
    template<>
        struct hash<Node> {
            std::size_t operator()(const Node& node) const noexcept {
                std::size_t h1 = std::hash<std::string>{}(node.product_name);
                std::size_t h2 = std::hash<int>{}(node.color);
                std::size_t h3 = std::hash<int>{}(node.size);

                // Хеш функция
                return h1 ^ (h2 << 1) ^ (h3 << 2);
            }
        };
}

int main() {
    std::unordered_set<Node> nodes;
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }

        std::istringstream iss(line);
        char operation;
        Node node;

        iss >> operation >> node.product_name >> node.color >> node.size;

        bool result = false;

        switch (operation) {
            case '+': {
                auto [i, inserted] = nodes.insert(node);
                result = inserted;
                break;
            }
            case '-': {
                size_t erased = nodes.erase(node);
                result = (erased > 0);
                break;
            }
            case '?': {
                result = (nodes.find(node) != nodes.end());
                break;
            }
            default:
                continue;
        }

        std::cout << (result ? "OK" : "FAIL") << std::endl;
    }

    return 0;
}
