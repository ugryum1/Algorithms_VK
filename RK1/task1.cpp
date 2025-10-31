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

template <typename T>
int findStartIndex(Array<T> arr, int n) {
    if (n <= 1) {
        return 0;
    }

    int left = 0, right = n - 1;

    // уже отсортирован
    if (arr[left] < arr[right]) {
        return 0;
    }

    while (left <= right) {
        int middle = left + (right - left) / 2;

        if (middle < n - 1 && arr[middle] > arr[middle + 1]) {
            return middle + 1;
        }

        if (middle > 0 && arr[middle] < arr[middle - 1]) {
            return middle;
        }

        if (arr[middle] > arr[0]) {
            left = middle + 1;
        } else {
            right = middle - 1;
        }
    }

    return 0;
}

int main() {
    int n, value;
    std::cin >> n;

    Array<int> arr;
    for (int i = 0; i < n; ++i) {
        std::cin >> value;
        arr.add(value);
    }

    std::cout << findStartIndex(arr, n) << std::endl;

    return 0;
}
