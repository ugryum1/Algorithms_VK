/*
2_2. Дан массив целых чисел А[0..n-1].
Известно, что на интервале [0, m] значения массива строго возрастают,
а на интервале [m, n-1] строго убывают. Найти m за O(log m).

Требования:  Время работы O(log m). Внимание!
В этой задаче сначала нужно определить диапазон для бинарного поиска размером
порядка m с помощью экспоненциального поиска, а потом уже в нем делать бинарный поиск.

2 ≤ n ≤ 10000.
Тест:
in                      out
10
1 2 3 4 5 6 7 6 5 4     6
*/

#include <iostream>

// сюда попадают только массивы с длиной >= 3 => можно сравнивать
// средний элемент с обоими соседями
int binSearch(int *arr, int left, int right) {
    int middle{};

    while (left < right) {
        middle = left + (right - left) / 2;

        // если arr[middle] больше своих соседей, то это нужный элемент
        if (arr[middle - 1] < arr[middle] && arr[middle + 1] < arr[middle]) {
            return middle;
        }

        // если последовательность возрастает, то идём вправо
        if (arr[middle] > arr[middle - 1]) {
            left = middle + 1;
        } else {
            // иначе налево
            right = middle - 1;
        }
    }

    return left;
}

int expSearch(int *arr, int n) {
    // для маленьких массивов
    if (n == 2) {
        return arr[0] > arr[1] ? 0 : 1;
    }

    // создаём правую границу и двигаем её, пока на ней не будет убывания
    int right = 1;
    while (right < n && arr[right - 1] < arr[right]) {
        right *= 2;
    }

    // сначала определяем левую границу, а уже потом
    // при необходимости сдвигаем правую до конца массива
    // чтобы не обходить лишние элементы в бинарном поиске
    int left = right / 2;
    right = (right < n) ? right : n - 1;

    return binSearch(arr, left, right);
}

int main() {
    int n;
    std::cin >> n;

    int *arr = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    std::cout << expSearch(arr, n) << std::endl;

    delete[] arr;
    return 0;
}
