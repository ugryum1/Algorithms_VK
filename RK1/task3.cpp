#include <iostream>
#include <cassert>
#include <cmath>

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

        // удалить элемент по индексу
        void remove(int index) {
            if (index < 0 || index >= size) {
                throw std::out_of_range("Index out of range");
            }
            for (int i = index; i < size - 1; ++i) {
                arr[i] = arr[i + 1];
            }
            --size;
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

struct Point {
    int x, y;

    Point(int x = 0, int y = 0) : x(x), y(y) {}

    bool operator<(const Point& other) const {
        if (x != other.x) {
            return x < other.x;
        }
        return y < other.y;
    }
};

bool polarCompare(const Point& start, const Point& a, const Point& b) {
    int dx1 = a.x - start.x;
    int dy1 = a.y - start.y;
    int dx2 = b.x - start.x;
    int dy2 = b.y - start.y;

    int cross = dx1 * dy2 - dx2 * dy1;

    if (cross != 0) {
        return cross > 0;
    }

    int dist1 = dx1 * dx1 + dy1 * dy1;
    int dist2 = dx2 * dx2 + dy2 * dy2;
    return dist1 > dist2;
}

void insertionSort(Array<Point>& points, const Point& start) {
    int n = points.getSize();

    for (int i = 1; i < n; ++i) {
        Point elem = points[i];
        int j = i - 1;

        while (j >= 0 && polarCompare(start, points[j], elem)) {
            points[j + 1] = points[j];
            --j;
        }
        points[j + 1] = elem;
    }
}

int main() {
    int n;
    std::cin >> n;

    Array<Point> points;

    for (int i = 0; i < n; ++i) {
        int x, y;
        std::cin >> x >> y;
        points.add(Point(x, y));
    }

    int startIndex = 0;
    for (int i = 1; i < points.getSize(); ++i) {
        if (points[i] < points[startIndex]) {
            startIndex = i;
        }
    }

    Point startPoint = points[startIndex];

    points.remove(startIndex);

    insertionSort(points, startPoint);

    std::cout << startPoint.x << " " << startPoint.y << std::endl;
    for (int i = 0; i < points.getSize(); ++i) {
        std::cout << points[i].x << " " << points[i].y << std::endl;
    }

    return 0;
}
