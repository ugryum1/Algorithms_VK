#include <iostream>
#include <vector>
#include <string>

const size_t defaultSize = 8;

template <typename T, typename H>
class HashTable {
    public:
        HashTable() : status(defaultSize, {true, false}), table(defaultSize, ""),
                      keysCount(0) {}
        HashTable(H hasher) : status(defaultSize, {true, false}), hasher(hasher),
                              table(defaultSize, ""), keysCount(0) {}
        ~HashTable() = default;

        // добавить ключ
        bool addKey(const T &key);

        // удалить ключ
        bool deleteKey(const T &key);

        // проверить наличие ключа
        bool hasKey(const T &key);
    private:
        // статус элемента
        struct Status {
            bool isEmpty;
            bool isDeleted;
        };

        H hasher;
        std::vector<T> table;
        std::vector<Status> status;
        size_t keysCount;

        // квадратичное пробирование
        size_t probe(const size_t k, const size_t i) const {
            if (i == 0) {
                return k % table.size();
            }
            return (k % table.size() + i * (i + 1) / 2) % table.size();
        }

        // расширение таблицы
        void growTable();

        // проверка, нужно ли расширяться
        bool needRehash() {
            return static_cast<double>(keysCount) / table.size() >= 0.75;
        }
};

template <typename T, typename H>
bool HashTable<T, H>::addKey(const T &key) {
    if (needRehash()) {
        growTable();
    }

    size_t hash = hasher(key);
    size_t firstDeleted = -1;

    for (size_t i = 0; i < table.size(); ++i) {
        size_t index = probe(hash, i);

        if (!status[index].isEmpty && !status[index].isDeleted && table[index] == key) {
            return false;
        }

        if (firstDeleted == -1 && status[index].isDeleted) {
            firstDeleted = index;
        }

        if (status[index].isEmpty) {
            if (firstDeleted != -1) {
                index = firstDeleted;
            }

            table[index] = key;
            status[index].isEmpty = false;
            status[index].isDeleted = false;
            ++keysCount;
            return true;
        }
    }

    return false;
}

template <typename T, typename H>
bool HashTable<T, H>::deleteKey(const T &key) {
    size_t hash = hasher(key);

    for (size_t i = 0; i < table.size(); ++i) {
        size_t index = probe(hash, i);

        if (status[index].isEmpty && !status[index].isDeleted) {
            return false;
        }

        if (!status[index].isEmpty && !status[index].isDeleted && table[index] == key) {
            status[index].isDeleted = true;
            --keysCount;
            return true;
        }
    }

    return false;
}

template <typename T, typename H>
bool HashTable<T, H>::hasKey(const T &key) {
    size_t hash = hasher(key);

    for (size_t i = 0; i < table.size(); ++i) {
        size_t index = probe(hash, i);

        if (status[index].isEmpty && !status[index].isDeleted) {
            return false;
        }

        if (!status[index].isEmpty && !status[index].isDeleted && table[index] == key) {
            return true;
        }
    }

    return false;
}

template <typename T, typename H>
void HashTable<T, H>::growTable() {
    size_t newSize = table.size() * 2;
    std::vector<T> oldTable = table;
    std::vector<Status> oldStatus = status;

    table.assign(newSize, "");
    status.assign(newSize, {true, false});
    keysCount = 0;

    for (size_t i = 0; i < oldTable.size(); ++i) {
        if (!oldStatus[i].isEmpty && !oldStatus[i].isDeleted) {
            addKey(oldTable[i]);
        }
    }
}

class StringHasher {
    public:
        StringHasher(size_t p) : p(p) {}
        size_t operator()(const std::string &key) const {
            size_t hash = 0;
            for (const char &c : key) {
                hash = hash * p + c - 'a' + 1;
            }
            return hash;
        }
    private:
        size_t p;
};

int main() {
    StringHasher hasher(137);
	HashTable<std::string, StringHasher> table(hasher);

	char op{};
	std::string word;

	while (std::cin >> op >> word) {
		switch (op) {
            case '+':
                std::cout << (table.addKey(word) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (table.deleteKey(word) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                std::cout << (table.hasKey(word) ? "OK" : "FAIL") << std::endl;
                break;
            default:
                break;
        }
	}

    return 0;
}
