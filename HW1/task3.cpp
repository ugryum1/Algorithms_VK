/*
Во всех задачах из следующего списка следует написать структуру данных,
обрабатывающую команды push* и pop*.

Формат входных данных.
В первой строке количество команд n. n ≤ 1000000.
Каждая команда задаётся как 2 целых числа: a b.
a = 1 - push front
a = 2 - pop front
a = 3 - push back
a = 4 - pop back
Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
Если дана команда pop*, то число b - ожидаемое значение.
Если команда pop вызвана для пустой структуры данных, то ожидается “-1”.

Формат выходных данных.
Требуется напечатать YES - если все ожидаемые значения совпали.
Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.

3_1. Реализовать очередь с динамическим зацикленным буфером (на основе динамического массива).
Требования: Очередь должна быть реализована в виде класса.

in      out
-----------
3       Yes
3 44
3 50
2 44
-----------
2       YES
2 -1
3 10
-----------
2       NO
3 44
2 66

*/

#include <iostream>

const int defaultSize = 5;

class Queue {
    public:
        Queue(int size = defaultSize) : size(size), headIndex(-1), tailIndex(-1) {
            queue = new int[size];
        }

        Queue(const Queue &other) = delete;
        Queue& operator=(const Queue &other) = delete;

        bool isEmpty() {
            return headIndex == -1;
        }

        // расширение очереди в 2 раза
        void extend() {
            // создаём новую очередь
            int *new_queue = new int[size * 2];

            // переносим все элементы
            for (int i = 0; i < size; ++i) {
                new_queue[i] = queue[i];
            }

            // перезаписываем указатель и увеличиваем размер очереди
            delete[] queue;
            queue = new_queue;
            size *= 2;
        }

        // выравнивание - очередь будет от головы до хвоста слева направо
        void align() {
            // создаём новую очередь
            int *new_queue = new int[size];

            int index = 0;

            // переносим все элементы в нужном порядке
            for (int i = headIndex; i < size; ++i) {
                new_queue[index++] = queue[i];
            }
            for (int i = 0; i < headIndex; ++i) {
                new_queue[index++] = queue[i];
            }

            // перезаписываем указатель
            delete[] queue;
            queue = new_queue;

            // обновляем индексы головы и хвоста
            headIndex = 0;
            tailIndex = size - 1;
        }

        int popFront() {
            if (isEmpty()) {
                return -1;
            }

            int value = queue[headIndex];

            // если в очреди 1 элемент - голова - это также хвост
            // в этом случае делаем очередь пустой
            if (headIndex == tailIndex) {
                headIndex = tailIndex = -1;
            } else {
                // если справа от головы есть места в массиве
                if (headIndex + 1 < size) {
                    ++headIndex;
                // иначе голова становится началом массива
                } else {
                    headIndex = 0;
                }
            }

            return value;
        }

        void pushBack(int value) {
            if (isEmpty()) {
                queue[0] = value;
                headIndex = tailIndex = 0;
            } else {
                // если голова левее хвоста (очередь не разрывается)
                if (headIndex <= tailIndex) {
                    // если помещается в конец - добавляем
                    if (tailIndex + 1 < size) {
                        queue[++tailIndex] = value;
                    } else {
                        // если голова - не нулевой элемент массива,
                        // переносим хвост в начало
                        if (headIndex) {
                            queue[0] = value;
                            tailIndex = 0;
                        // иначе расширяем массив и добавляем элемент
                        } else {
                            extend();
                            queue[++tailIndex] = value;
                        }
                    }
                // голова правее хвоста
                } else {
                    // если помещается в конец - добавляем
                    if (tailIndex + 1 < headIndex) {
                        queue[++tailIndex] = value;
                    // иначе выравниваем очередь, расширяем и добавляем элемент
                    } else {
                        align();
                        extend();
                        queue[++tailIndex] = value;
                    }
                }
            }
        }

        ~Queue() {
            delete[] queue;
        }
    private:
        int size;
        int *queue;
        int headIndex;
        int tailIndex;
};

void run(std::istream &input, std::ostream &output) {
    int n, command, value;
    input >> n;

    Queue queue;

    for (int i = 0; i < n; ++i) {
        input >> command >> value;

        switch (command) {
            case 2:
                if (queue.popFront() != value) {
                    std::cout << "NO" << std::endl;
                    return;
                }
                break;
            case 3:
                queue.pushBack(value);
                break;
            default:
                break;
        }
    }

    output << "YES" << std::endl;
}

int main() {
    run(std::cin, std::cout);

    return 0;
}
