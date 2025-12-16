#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"

int main(int argc, const char * argv[]) {
    ListGraph listGraph(7);
    listGraph.AddEdge(0, 1);
    listGraph.AddEdge(0, 5);
    listGraph.AddEdge(1, 2);
    listGraph.AddEdge(1, 3);
    listGraph.AddEdge(1, 5);
    listGraph.AddEdge(1, 6);
    listGraph.AddEdge(3, 2);
    listGraph.AddEdge(3, 4);
    listGraph.AddEdge(3, 6);
    listGraph.AddEdge(5, 4);
    listGraph.AddEdge(5, 6);
    listGraph.AddEdge(6, 4);

    std::cout << "ListGraph" << std::endl;
    std::cout << "Количество вершин: \t\t" << listGraph.VerticesCount() << std::endl;

    std::cout << "BFS: \t\t\t\t";
    mainBFS(listGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    std::cout << "DFS: \t\t\t\t";
    mainDFS(listGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    std::cout << "Топологическая сортировка: \t";
    for (int vertex: topologicalSort(listGraph)) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl << std::endl;


    MatrixGraph matrixGraph(listGraph);

    std::cout << "MatrixGraph" << std::endl;
    std::cout << "Количество вершин: \t\t" << matrixGraph.VerticesCount() << std::endl;

    std::cout << "BFS: \t\t\t\t";
    mainBFS(matrixGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    std::cout << "DFS: \t\t\t\t";
    mainDFS(matrixGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    std::cout << "Топологическая сортировка: \t";
    for (int vertex: topologicalSort(matrixGraph)) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl << std::endl;


    ArcGraph arcGraph(matrixGraph);

    std::cout << "ArcGraph" << std::endl;
    std::cout << "Количество вершин: \t\t" << arcGraph.VerticesCount() << std::endl;

    std::cout << "BFS: \t\t\t\t";
    mainBFS(arcGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    std::cout << "DFS: \t\t\t\t";
    mainDFS(arcGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    std::cout << "Топологическая сортировка: \t";
    for (int vertex: topologicalSort(arcGraph)) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl << std::endl;


    SetGraph setGraph(arcGraph);

    std::cout << "SetGraph" << std::endl;
    std::cout << "Количество вершин: \t\t" << setGraph.VerticesCount() << std::endl;

    std::cout << "BFS: \t\t\t\t";
    mainBFS(setGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    std::cout << "DFS: \t\t\t\t";
    mainDFS(setGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    std::cout << "Топологическая сортировка: \t";
    for (int vertex: topologicalSort(setGraph)) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;

    // Нужно продемонстрировать работу конструктора копирования, проинициализировав
    // графы разных классов друг от друга. Далее, показать, что вершины и ребра
    // успешно скопированы: число вершин осталось прежним, произвести BFS и DFS.
    // MatrixGraph matrixGraph(listGraph);
    // ArcGraph arcGraph(matrixGraph);
    // SetGraph setGraph(arcGraph);
    return 0;
}
