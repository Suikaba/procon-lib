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


template <typename Capacity>
class capacity_edge {
public:
    using capacity_type = Capacity;
    int to, rev;
    capacity_type cap;
    capacity_edge(int t, int r, capacity_type c) : to(t), rev(r), cap(c) {}
};

template <typename Capacity>
class capacity_graph : public graph<capacity_edge<Capacity>> {
    using base_type = graph<capacity_edge<Capacity>>;
public:
    capacity_graph(int n) : base_type(n) {}
    using base_type::add_edge;
    void add_edge(int from, int to, Capacity cap) {
        add_edge(from, capacity_edge<Capacity>{to, this->edge_size(to), cap});
        add_edge(to, capacity_edge<Capacity>{from, this->edge_size(from) - 1, Capacity{0}});
    }
};

