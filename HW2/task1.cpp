#include <iostream>
#include <vector>
#include <string>
#include <sstream>

const size_t defaultSize = 8;

template <typename T, typename H>
class HashTable {
    public:
        // конструкторы и деструктор
        HashTable();
        HashTable(H hasher);
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
            bool isEmpty; // флаг пустоты ячейки
            bool isDeleted; // флаг удаления ячейки
        };

        H hasher;
        std::vector<T> table;
        std::vector<Status> status; // ветктор статусов для каждой ячейки
        size_t keysCount;

        // квадратичное пробирование
        size_t probe(const size_t k, const size_t i) const;

        // расширение таблицы
        void growTable();

        // проверка, нужно ли расширяться
        bool needRehash();
};

template <typename T, typename H>
HashTable<T, H>::HashTable() : hasher(), status(defaultSize, {true, false}),
                               table(defaultSize, ""), keysCount(0) {}

template <typename T, typename H>
HashTable<T, H>::HashTable(H hasher) : hasher(hasher), status(defaultSize, {true, false}),
                                       table(defaultSize, ""), keysCount(0) {}

// добавить ключ
template <typename T, typename H>
bool HashTable<T, H>::addKey(const T &key) {
    if (needRehash()) {
        growTable();
    }

    size_t hash = hasher(key);
    size_t firstDeleted = table.size();

    for (size_t i = 0; i < table.size(); ++i) {
        size_t index = probe(hash, i);

        // если ключ уже есть
        if (!status[index].isEmpty && !status[index].isDeleted && table[index] == key) {
            return false;
        }

        if (firstDeleted == table.size() && status[index].isDeleted) {
            // нашли первый удалённый элемент
            firstDeleted = index;
        }

        // для первой найденной пустой ячейки
        if (status[index].isEmpty) {
            // если есть возможность записать в удалённую ячайку - пользуемся
            if (firstDeleted != table.size()) {
                index = firstDeleted;
            }

            table[index] = key;
            status[index].isEmpty = false;
            status[index].isDeleted = false;
            ++keysCount;
            return true;
        }
    }

    if (firstDeleted != table.size()) {
        table[firstDeleted] = key;
        status[firstDeleted].isEmpty = false;
        status[firstDeleted].isDeleted = false;
        ++keysCount;
        return true;
    }

    return false;
}

// удалить ключ
template <typename T, typename H>
bool HashTable<T, H>::deleteKey(const T &key) {
    size_t hash = hasher(key);

    for (size_t i = 0; i < table.size(); ++i) {
        size_t index = probe(hash, i);

        // если ячейка пустая и не удалена, то мы не не можем её удалить
        if (status[index].isEmpty && !status[index].isDeleted) {
            return false;
        }

        // если ячейка содержит искомый ключ и не удалена - удаляем
        if (!status[index].isEmpty && !status[index].isDeleted && table[index] == key) {
            status[index].isDeleted = true;
            --keysCount;
            return true;
        }
    }

    return false;
}

// проверить наличие ключа
template <typename T, typename H>
bool HashTable<T, H>::hasKey(const T &key) {
    size_t hash = hasher(key);

    for (size_t i = 0; i < table.size(); ++i) {
        size_t index = probe(hash, i);

        // если ячейка пустая и не удалена, ключа нет
        if (status[index].isEmpty && !status[index].isDeleted) {
            return false;
        }

        // если ключ есть в ячейке и он не удалён - нашли
        if (!status[index].isEmpty && !status[index].isDeleted && table[index] == key) {
            return true;
        }
    }

    return false;
}

// квадратичное пробирование
template <typename T, typename H>
size_t HashTable<T, H>::probe(const size_t k, const size_t i) const {
    size_t m = table.size();
    size_t base = k % m;

    if (i == 0) {
        return base;
    }

    size_t offset = (i * (i + 1) / 2) % m;
    return (base + offset) % m;
}

// расширение таблицы
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

// проверка, нужно ли расширяться
template <typename T, typename H>
bool HashTable<T, H>::needRehash() {
    return static_cast<double>(keysCount) / table.size() >= 0.75;
}

// функтор для вычисления хеша строки
class StringHasher {
    public:
        StringHasher(size_t p) : p(p) {}
        size_t operator()(const std::string &key) const {
            size_t hash = 0;
            for (const char &c : key) {
                hash = hash * p + static_cast<unsigned char>(c - 'a' + 1);
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
	std::string word{};

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
