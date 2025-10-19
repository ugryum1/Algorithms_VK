/*
Требование для всех вариантов Задачи 5
Во всех задачах данного раздела необходимо реализовать
и использовать сортировку слиянием в виде шаблонной функции.
Решение должно поддерживать передачу функции сравнения снаружи.
Общее время работы алгоритма O(n log n).

5_3. Закраска прямой 1.
На числовой прямой окрасили N отрезков. Известны координаты левого и правого
концов каждого отрезка (Li и Ri). Найти длину окрашенной части числовой прямой.
In      Out
-----------
3       5
1 4
7 8
2 5
*/

#include <iostream>
#include <cassert>
#include <stdexcept>

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

// структура точки с параметрами координаты и "направления" (начало закрашенного участка или конец)
struct Point {
    Point(int coordinate = 0, int direction = 0) : coordinate(coordinate), direction(direction) {}

    int coordinate;
    int direction;

    // перегружаем оператор ввода
    friend std::istream& operator>>(std::istream &input, Point &technux) {
        input >> technux.coordinate >> technux.direction;
        return input;
    }
};

// базовый шаблонный компаратор
template <typename T>
struct DefaultComparator {
    bool operator()(const T &l, const T &r) const {
        return l < r;
    }
};

// компаратор для структуры Point
struct PointComparator {
    bool operator()(const Point &l, const Point &r) const {
        return l.coordinate < r.coordinate;
    }
};

// слияние двух отсортированных массивов
template <typename T, typename Comparator = DefaultComparator<T>>
Array<T> mergeTwoArrays(const Array<T> &arr1, const Array<T> &arr2, Comparator comp) {
    Array<T> arr = Array<T>();
    int i = 0, j = 0;
    int n = arr1.getSize(), m = arr2.getSize();

    while (i < n && j < m) {
        if (comp(arr1[i], arr2[j])) {
            arr.add(arr1[i++]);
        } else {
            arr.add(arr2[j++]);
        }
    }

    while (i < n) {
        arr.add(arr1[i++]);
    }

    while (j < m) {
        arr.add(arr2[j++]);
    }

    return arr;
}

// сортировка слиянием
template <typename T, typename Comparator = DefaultComparator<T>>
Array<T> mergeSort(Array<T> &arr, Comparator comp) {
    int size = arr.getSize();
    if (size <= 1) {
        return arr;
    }

    // левая половина
    Array<T> left;
    int middle = size / 2;
    for (int i = 0; i < middle; ++i) {
        left.add(arr[i]);
    }

    // правая половина
    Array<T> right;
    for (int i = middle; i < size; ++i) {
        right.add(arr[i]);
    }

    // сортируем половины рекурсивно
    left = mergeSort(left, comp);
    right = mergeSort(right, comp);

    return mergeTwoArrays(left, right, comp);
}

// нахождение длинны закрашенной части прямой
int calculatePaintedLength(Array<Point> &points) {
    if (points.isEmpty()) {
        return 0;
    }

    PointComparator comp;
    Array<Point> sortedPoints = mergeSort(points, comp);

    int totalLength = 0;
    int currentDepth = 0;
    int startPos = 0;

    for (int i = 0; i < sortedPoints.getSize(); ++i) {
        // начинается окрашенный отрезок
        if (currentDepth == 0 && sortedPoints[i].direction > 0) {
            startPos = sortedPoints[i].coordinate;
        }

        currentDepth += sortedPoints[i].direction;

        // заканчивается окрашенный отрезок
        if (currentDepth == 0) {
            totalLength += sortedPoints[i].coordinate - startPos;
        }
    }

    return totalLength;
}

int main() {
    int n;
    std::cin >> n;

    Array<Point> points;

    int start, end;
    for (int i = 0; i < n; ++i) {
        std::cin >> start >> end;
        // разбиваем начало и конец на 2 структуры Point
        points.add(Point(start, 1));
        points.add(Point(end, -1));
    }

    std::cout << calculatePaintedLength(points) << std::endl;

    return 0;
}
