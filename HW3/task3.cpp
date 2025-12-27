/*
Задача 3. «Города»
Требуется отыскать самый выгодный маршрут между городами.

Требования: время работы O((N+M)logN),
где N-количество городов, M-известных дорог между ними.
Граф должен быть реализован в виде класса.

Формат входных данных.
Первая строка содержит число N – количество городов.
Вторая строка содержит число M - количество дорог.
Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
Последняя строка содержит маршрут (откуда и куда нужно доехать).

Формат выходных данных.
Вывести длину самого выгодного маршрута.

in          out
----------------
6           9
9
0 3 1
0 4 2
1 2 7
1 3 2
1 4 3
1 5 3
2 5 3
3 4 4
3 5 6
0 2
*/

#include <iostream>
#include <vector>
#include <cassert>
#include <queue>
#include <climits>

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to, int weight) = 0;

	virtual int VerticesCount() const = 0;

    virtual std::vector<std::pair<int,int>> GetNextVertices(int vertex) const = 0;
};

struct ListGraph: public IGraph {
public:
    ListGraph(int size);
    ListGraph(const IGraph &graph);
    ~ListGraph();

    void AddEdge(int from, int to, int weight) override;

    int VerticesCount() const override;

    std::vector<std::pair<int,int>> GetNextVertices(int vertex) const override;

private:
    std::vector<std::vector<std::pair<int,int>>> adjacencyLists;
};

ListGraph::ListGraph(int size) : adjacencyLists(size) {}

ListGraph::ListGraph(const IGraph &graph) {
    adjacencyLists.resize(graph.VerticesCount());
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        adjacencyLists[i] = graph.GetNextVertices(i);
    }
}

ListGraph::~ListGraph() {}

void ListGraph::AddEdge(int from, int to, int weight) {
    assert(0 <= from && from < adjacencyLists.size());
    assert(0 <= to && to < adjacencyLists.size());
    adjacencyLists[from].push_back({to, weight});
    if (from != to) {
        adjacencyLists[to].push_back({from, weight});
    }
}

int ListGraph::VerticesCount() const {
    return static_cast<int>(adjacencyLists.size());
}

std::vector<std::pair<int,int>> ListGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < adjacencyLists.size());
    return adjacencyLists[vertex];
}

// Алгоритм Дейкстры
int Dijkstra(const IGraph& graph, int start, int finish) {
    int n = graph.VerticesCount();

    // d[v] - минимальное расстояние от start до v
    std::vector<int> d(n, INT_MAX);

    // pi[v] - предыдущая вершина на кратчайшем пути
    std::vector<int> pi(n, -1);

    // Расстояние до стартовой вершины равно 0
    d[start] = 0;

    // Очередь с приоритетом, хранит пары (текущее расстояние, вершина)
    // greater, чтобы получить min-heap
    std::priority_queue<
        std::pair<int,int>,
        std::vector<std::pair<int,int>>,
        std::greater<std::pair<int,int>>
    > q;

    q.push({0, start});

    while (!q.empty()) {
        // Вершина с минимальным расстоянием
        int u = q.top().second;
        int dist = q.top().first;
        q.pop();

        if (dist > d[u]) {
            continue;
        }

        // Просматриваем все рёбра из вершины u
        for (auto [v, w] : graph.GetNextVertices(u)) {
            if (d[v] > d[u] + w) {
                d[v] = d[u] + w;
                pi[v] = u;
                q.push({d[v], v});
            }
        }
    }

    return d[finish];
}

int main() {
    int n, m;
    std::cin >> n >> m;

    ListGraph graph(n);

    for (int i = 0; i < m; ++i) {
        int from, to, weight;
        std::cin >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
    }

    int start, finish;
    std::cin >> start >> finish;

    std::cout << Dijkstra(graph, start, finish) << std::endl;

    return 0;
}
