/*
7_3. Binary MSD для long long.
Дан массив неотрицательных целых 64-разрядных чисел. Количество чисел не больше 106.
Отсортировать массив методом MSD по битам (бинарный QuickSort).

In              Out
---------------------------
3               4 7 1000000
4 1000000 7
*/

#include <iostream>
#include <cstdint>

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

void binaryMSD(Array<uint64_t> &arr, int l, int r, int bit) {
    if (l >= r || bit < 0) {
        return;
    }

    int i = l, j = r;

    // сортировка по текущему биту
    while (i <= j) {
        // ищем элемент с битом = 1 слева
        while (i <= j && ((arr[i] >> bit) & 1) == 0) {
            ++i;
        }
        // ищем элемент с битом = 0 справа
        while (i <= j && ((arr[j] >> bit) & 1) == 1) {
            --j;
        }
        // меняем местами
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            ++i;
            --j;
        }
    }

    // сортируем части для следующего бита
    if (l < j) {
        binaryMSD(arr, l, j, bit - 1);
    }

    if (i < r) {
        binaryMSD(arr, i, r, bit - 1);
    }
}

void binaryMSDSort(Array<uint64_t> &arr, int n) {
    if (arr.isEmpty()) {
        return;
    }
    binaryMSD(arr, 0, n - 1, 63); // 63 - старший бит
}

int main () {
    int n;
    uint64_t value;
    std::cin >> n;

    Array<uint64_t> arr;
    for (int i = 0; i < n; ++i) {
        std::cin >> value;
        arr.add(value);
    }

    binaryMSDSort(arr, n);

    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;

    return 0;
}
