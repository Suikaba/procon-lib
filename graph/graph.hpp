
template <typename Edge>
class graph {
    using graph_t = std::vector<std::vector<Edge>>;
public:
    using reference = std::vector<Edge>&;
    using const_reference = std::vector<Edge> const&;
    using iterator = typename graph_t::iterator;
    using const_iterator = typename graph_t::const_iterator;
    using reverse_iterator = typename graph_t::reverse_iterator;

    graph() : g() {}
    graph(int n) : g(n) {}

    reference operator[](int x) { return g[x]; }
    const_reference operator[](int x) const { return g[x]; }

    iterator begin() { return std::begin(g); }
    const_iterator begin() const { return std::begin(g); }
    iterator end() { return std::end(g); }
    const_iterator end() const { return std::end(g); }
    reverse_iterator rbegin() { return std::rbegin(g); }
    reverse_iterator rend() { return std::rend(g); }

    int size() const { return g.size(); }

    void add_node(std::vector<Edge> es) {
        g.push_back(std::move(es));
    }

private:
    std::vector<std::vector<Edge>> g;
};

// ============================================================================
// directed graph
struct directed_edge {
    directed_edge(int t) : to(t) {}
    int to;
};

using directed_graph = graph<directed_edge>;

void add_edge(directed_graph& g, int from, int to) {
    g[from].emplace_back(to);
}

// ============================================================================
// bidirected graph
struct bidirected_edge {
    bidirected_edge(int f, int t) : from(f), to(t) {}
    int from, to;
};

using bidirected_graph = graph<bidirected_edge>;

void add_edge(bidirected_graph& g, int u, int v) {
    g[u].emplace_back(u, v);
    g[v].emplace_back(v, u);
}

// ============================================================================
// weighted graph (directed)
template <typename Cost>
struct weighted_edge {
    using cost_type = Cost;
    weighted_edge(int t, Cost c) : to(t), cost(c) {}
    int to;
    cost_type cost;
};

template <typename Cost>
using weighted_graph = graph<weighted_edge<Cost>>;

template <typename Cost>
void add_edge(weighted_graph<Cost>& g, int from, int to, Cost cost) {
    g[from].emplace_back(to, cost);
}

// ============================================================================
// capacity graph (for maximum flow)
template <typename Capacity>
struct capacity_edge {
    using capacity_type = Capacity;
    int to, rev;
    capacity_type cap;
    capacity_edge(int t, int r, capacity_type c) : to(t), rev(r), cap(c) {}
};

template <typename Capacity>
using capacity_graph = graph<capacity_edge<Capacity>>;

template <typename Capacity>
void add_edge(capacity_graph<Capacity>& g, int from, int to, Capacity cap) {
    g[from].emplace_back(to, static_cast<int>(g[to].size()), cap);
    g[to].emplace_back(from, static_cast<int>(g[from].size() - 1), Capacity{0});
}

// ============================================================================
// capacity weighted graph (for minimum cost flow)
template <typename Capacity, typename Cost>
struct capacity_weighted_edge {
    using capacity_type = Capacity;
    using cost_type = Cost;
    int to, rev;
    capacity_type cap;
    cost_type cost;
    capacity_weighted_edge(int t, int r, capacity_type cp, cost_type c)
        : to(t), rev(r), cap(cp), cost(c)
    {}
};

template <typename Capacity, typename Cost>
using capacity_weighted_graph = graph<capacity_weighted_edge<Capacity, Cost>>;

template <typename Capacity, typename Cost>
void add_edge(capacity_weighted_graph<Capacity, Cost>& g, int from, int to, Capacity cap, Cost cost) {
    g[from].emplace_back(to, static_cast<int>(g[to].size()), cap, cost);
    g[to].emplace_back(from, static_cast<int>(g[from].size() - 1), Capacity{0}, -cost);
}
