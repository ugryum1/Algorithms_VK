#include <iostream>
#include <cassert>
#include <string>

const int defaultSize = 5;

template <typename T>
class Array {
    public:
        // не забываем про привило трёх: КК, Д, ОП
        Array(int capacity = defaultSize) : size(0), capacity(capacity) {
            arr = new T[capacity];
        }

        Array(const Array &other) : size(other.size), capacity(other.capacity) {
            arr = new T[capacity];
            for (int i = 0; i < size; ++i) {
                arr[i] = other.arr[i];
            }
        }

        Array& operator=(const Array &other) {
            if (this != &other) {
                delete[] arr;

                size = other.size;
                capacity = other.capacity;

                arr = new T[capacity];
                for (int i = 0; i < size; ++i) {
                    arr[i] = other.arr[i];
                }
            }
            return *this;
        }

        // доступ к элементу по индексу
        T& operator[](int i) {
            if (i < 0 || i >= size) {
                throw std::out_of_range("Index out of range");
            }
            return arr[i];
        }

        // константная версия
        const T& operator[](int i) const {
            if (i < 0 || i >= size) {
                throw std::out_of_range("Index out of range");
            }
            return arr[i];
        }

        // добавление элемента в конец массива
        void add(T element) {
            if (size >= capacity) {
                extend();
            }
            arr[size++] = element;
        }

        // посмотреть последний элемент массива
        T& last() {
            assert(!isEmpty());
            return arr[size - 1];
        }

        // константная версия
        const T& last() const {
            assert(!isEmpty());
            return arr[size - 1];
        }

        // проверка на пустоту
        bool isEmpty() const {
            return size == 0;
        }

        // удаление последнего элемента
        void deleteLast() {
            assert(!isEmpty());
            --size;
        }

        // узнать размер массива
        int getSize() const{
            return size;
        }

        ~Array() {
            delete[] arr;
        }

    private:
        T *arr;
        int size; // реальный размер, т.е. сколько храним элементов
        int capacity; // количество ячеек

        // расширение в 2 раза
        void extend() {
            T *newArr = new T[capacity * 2];

            for (int i = 0; i < size; ++i) {
                newArr[i] = arr[i];
            }

            delete[] arr;
            arr = newArr;
            capacity *= 2;
        }
};

bool isCorrect (std::string s) {
    Array<char> stack;

    for (char c : s) {
        if (c == '(') {
            stack.add(c);
        } else {
            if (stack.isEmpty()) {
                return false;
            }
            stack.deleteLast();
        }
    }

    return stack.isEmpty();
}

int main() {
    std::string s;
    std::getline(std::cin, s);

    if (isCorrect(s)) {
        std::cout << "YES" << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }

    return 0;
}
