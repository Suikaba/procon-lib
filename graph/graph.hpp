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
    iterator end() { return std::end(g); }
    reverse_iterator rbegin() { return std::rbegin(g); }
    reverse_iterator rend() { return std::rend(g); }

    int size() const { return g.size(); }

    void add_node(std::vector<Edge> es) {
        g.push_back(std::move(es));
    }

    void add_edge(int node_idx, Edge e) {
        g[node_idx].push_back(std::move(e));
    }

private:
    std::vector<std::vector<Edge>> g;
};


class directed_edge {
public:
    directed_edge(int t) : to(t) {}

    int to;
};

class directed_graph : public graph<directed_edge> {
public:
    directed_graph(int n) : graph(n) {}

    using graph::add_edge;
    void add_edge(int from, int to) {
        add_edge(from, directed_edge{to});
    }
};


class bidirected_edge {
public:
    bidirected_edge(int f, int t) : from(f), to(t) {}

    int from, to;
};

class bidirected_graph : public graph<bidirected_edge> {
public:
    bidirected_graph(int n) : graph(n) {}

    using graph::add_edge;
    void add_edge(int u, int v) {
        add_edge(u, bidirected_edge{u, v});
        add_edge(v, bidirected_edge{v, u});
    }
};


template <typename Cost>
class weighted_edge {
public:
    using cost_type = Cost;

    weighted_edge(int t, Cost c) : to(t), cost(c) {}

    int to;
    cost_type cost;
};

template <typename Cost>
class weighted_graph : public graph<weighted_edge<Cost>> {
    using base_t = graph<weighted_edge<Cost>>;
public:
    weighted_graph(int n) : base_t(n) {}

    using base_t::add_edge;
    void add_edge(int from, int to, Cost cost) {
        add_edge(from, weighted_edge<Cost>{to, cost});
    }
};
