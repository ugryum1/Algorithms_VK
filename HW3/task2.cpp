/*
Задача 2. Количество различных путей

Дан невзвешенный неориентированный граф.
В графе может быть несколько кратчайших путей между какими-то вершинами.
Найдите количество различных кратчайших путей между заданными вершинами.

Требования: сложность O(V+E), граф реализован в виде класса.

Формат ввода.
v: кол-во вершин (макс. 50000),
n: кол-во ребер (макс. 200000),
n пар реберных вершин,
пара вершин u, w для запроса.

Формат вывода.
Количество кратчайших путей от u к w.

in      out
-----------
4       2
5
0 1
0 2
1 2
1 3
2 3
0 3

*/

#include <iostream>
#include <vector>
#include <cassert>
#include <queue>
#include <climits>

struct IGraph {
    virtual ~IGraph() {}

	// Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

struct ListGraph: public IGraph {
public:
    ListGraph(int size);
    ListGraph(const IGraph &graph);
    ~ListGraph();

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> adjacencyLists;
};

ListGraph::ListGraph(int size) : adjacencyLists(size) {}

ListGraph::ListGraph(const IGraph &graph) {
    adjacencyLists.resize(graph.VerticesCount());
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        adjacencyLists[i] = graph.GetNextVertices(i);
    }
}

ListGraph::~ListGraph() {}

void ListGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < adjacencyLists.size());
    assert(0 <= to && to < adjacencyLists.size());
    adjacencyLists[from].push_back(to);
    if (from != to) {
        adjacencyLists[to].push_back(from);
    }
}

int ListGraph::VerticesCount() const {
    return static_cast<int>(adjacencyLists.size());
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < adjacencyLists.size());
    return adjacencyLists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    assert(0 <= vertex && vertex < adjacencyLists.size());
    return adjacencyLists[vertex];
}

void bfs(const ListGraph &graph, int u, int n, std::vector<int> &d, std::vector<int> &k) {
    std::queue<int> q;
    q.push(u);

    while(!q.empty()) {
        int v = q.front();
        q.pop();

        for (int w : graph.GetNextVertices(v)) {
            if (d[v] + 1 < d[w]) {
                d[w] = d[v] + 1;
                k[w] = k[v];
                q.push(w);
            } else if (d[v] + 1 == d[w]) {
                k[w] += k[v];
            }
        }
    }
}

int shortestPathsNumber(const ListGraph &graph, int u, int w) {
    int n = graph.VerticesCount();
    assert(u < n && w < n);

    std::vector<int> d; // вектор кратчайших путей
    d.assign(n, INT_MAX);
    d[u] = 0;

    std::vector<int> k; // вектор чисел кратчайших путей
    k.assign(n, 0);
    k[u] = 1;

    bfs(graph, u, n, d, k);

    return k[w];
}

int main() {
    int v, n;
    std::cin >> v;
    std::cin >> n;

    ListGraph graph(v);

    for (int i = 0; i < n; ++i) {
        int v1, v2;
        std::cin >> v1 >> v2;
        graph.AddEdge(v1, v2);
    }

    int u, w;
    std::cin >> u >> w;

    std::cout << shortestPathsNumber(graph, u, w) << std::endl;

    return 0;
}
