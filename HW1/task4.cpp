/*
4.3 Планировщик процессов
В операционной системе Technux есть планировщик процессов.
Каждый процесс характеризуется:
    - приоритетом P
    - временем, которое он уже отработал t
    - временем, которое необходимо для завершения работы процесса T
Планировщик процессов выбирает процесс с минимальным значением P * (t + 1),
выполняет его время P и кладет обратно в очередь процессов.
Если выполняется условие t >= T, то процесс считается завершенным и удаляется из очереди.
Требуется посчитать кол-во переключений процессора.
Формат входных данных:  Сначала вводится кол-во процессов. После этого процессы в формате P T
Формат выходных данных: Кол-во переключений процессора.

in      out
-----------
3       18
1 10
1 5
2 5
*/

#include <iostream>
#include <cassert>
#include <stdexcept>

const int defaultSize = 5;

// массив для работы кучи
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

// базовый шаблонный компаратор
template <typename T>
struct DefaultComparator {
    bool operator()(const T &l, const T &r) const {
        return l < r;
    }
};

// двоичная куча
template <typename T, typename Comparator = DefaultComparator<T>>
class Heap {
    public:
        Heap(Comparator comp = Comparator()) : comp(comp) {}

        Heap(const Array<T> &_arr, Comparator comp = Comparator()) : arr(_arr), comp(comp) {
            buildHeap();
        }

        // добавление элемента
        void add(const T &element) {
            // сначала в конец массива, потом поднимаем на нужную позицию
            arr.add(element);
            siftUp(arr.getSize() - 1);
        }

        // извлечение корневого (для моей min-кучи - минимального) элемента
        T extract() {
            assert(!arr.isEmpty());

            // запоимнаем значение корня
            T result = arr[0];
            // переносим последний элемент в корень
            arr[0] = arr.last();
            arr.deleteLast();
            // ставим корень на нужную позицию
            if (!arr.isEmpty()) {
                siftDown(0);
            }
            return result;
        }

        // узнать корневой элемент
        const T& top() const {
            assert(!arr.isEmpty());
            return arr[0];
        }

        // проверка на пустоту
        bool isEmpty() const {
            return arr.isEmpty();
        }

        // узнать размер кучи
        int getSize() const {
            return arr.getSize();
        }

        ~Heap() {}
    private:
        Array<T> arr;
        Comparator comp;

        // построение кучи на массиве
        void buildHeap() {
            for (int i = arr.getSize() / 2 - 1; i >= 0; --i) {
                siftDown(i);
            }
        }

        // проталкивание элемента вниз
        void siftDown(int i) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;

            // ищем меньшего сына, если он есть
            if (left < arr.getSize() && comp(arr[left], arr[i])) {
                smallest = left;
            }
            if (right < arr.getSize() && comp(arr[right], arr[smallest])) {
                smallest = right;
            }

            // если меньший сын есть, то проталкиваем корень в него
            if (smallest != i) {
                std::swap(arr[i], arr[smallest]);
                siftDown(smallest);
            }
        }

        // проталкиваение элемената наверх
        void siftUp(int i) {
            while (i > 0) {
                int parent = (i - 1) / 2;
                if (!comp(arr[i], arr[parent])) {
                    return;
                }
                std::swap(arr[i], arr[parent]);
                i = parent;
            }
        }
};

// структура ОС из условия задачи
struct Technux {
    Technux() : P(0), t(0), T(0) {}
    Technux(int P, int T) : P(P), t(0), T(T) {}
    Technux(int P, int t, int T) : P(P), t(t), T(T) {}

    int P, t, T;

    int getValue() const {
        return P * (t + 1);
    }

    // перегружаем оператор ввода
    friend std::istream& operator>>(std::istream &input, Technux &technux) {
        input >> technux.P >> technux.T;
        technux.t = 0;
        return input;
    }
};

// кастомный компаратор для структуры Technux
struct TechnuxComparator {
    bool operator()(const Technux &l, const Technux &r) const {
        return l.getValue() < r.getValue();
    }
};

int main() {
    int n;
    std::cin >> n;

    // заполняем массив процессов
    Array<Technux> processes;
    for (int i = 0; i < n; ++i) {
        Technux technux;
        std::cin >> technux;
        processes.add(technux);
    }

    // строим кучу
    Heap<Technux, TechnuxComparator> heap(processes);
    int switches = 0;

    // считаем количество переключений процессов
    while (!heap.isEmpty()) {
        Technux current = heap.extract();

        current.t += current.P;
        ++switches;

        if (current.t < current.T) {
            heap.add(current);
        }
    }

    std::cout << switches << std::endl;

    return 0;
}
