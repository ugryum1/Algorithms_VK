/*
Дано множество целых чисел из [0..10^9] размера n.
Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
    10%  перцентиль
    медиана
    90%  перцентиль

Требования: к дополнительной памяти: O(n).
Среднее время работы: O(n)
Должна быть отдельно выделенная функция partition.
Рекурсия запрещена.
Решение должно поддерживать передачу функции сравнения снаружи.

6_4. Реализуйте стратегию выбора опорного элемента “случайный элемент”.
Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.

In                      Out
---------------------------
10                      2
1 2 3 4 5 6 7 8 9 10    6
                        10
*/

#include <iostream>
#include <cassert>
#include <stdexcept>
#include <fstream>

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

        // проверка на пустоту
        bool isEmpty() const {
            return size == 0;
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

template <typename T, typename Comparator = std::less<T>>
int partition(Array<T> &arr, int l, int r, Comparator cmp = Comparator()) {
    // случайный выбор опорного элемента
    int pivotIndex = l + (std::rand() % (r - l + 1));
    std::swap(arr[pivotIndex], arr[r]);
    T pivot = arr[r];

    int i = r - 1; // конец группы элементов, строго меньших опорного
    int j = r - 1; // первый нерассмотренный элемент

    while (j >= l) {
        // Если arr[j] > опорного
        if (cmp(pivot, arr[j])) {
            std::swap(arr[i], arr[j]);
            i--;
        }
        j--;
    }

    // перемещаем опорный элемент на правильную позицию
    std::swap(arr[i + 1], arr[r]);

    return i + 1;
}

template <typename T, typename Comparator = std::less<T>>
T kthStatistic(Array<T> &arr, int k, int l, int r, Comparator cmp = Comparator()) {
    int left = l, right = r;

    while (left <= right) {
        int pivotPos = partition(arr, left, right, cmp);

        if (pivotPos == k) {
            return arr[pivotPos];
        } else if (pivotPos > k) {
            right = pivotPos - 1;
        } else {
            left = pivotPos + 1;
        }
    }

    return arr[k];
}

int main() {
    int n, value;
    std::cin >> n;

    Array<int> arr;
    for (int i = 0; i < n; ++i) {
        std::cin >> value;
        arr.add(value);
    }

    std::cout << kthStatistic(arr, static_cast<int>(n * 0.1), 0, n - 1) << std::endl;
    std::cout << kthStatistic(arr, static_cast<int>(n * 0.5), 0, n - 1) << std::endl;
    std::cout << kthStatistic(arr, static_cast<int>(n * 0.9), 0, n - 1) << std::endl;

    return 0;
}
