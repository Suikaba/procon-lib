using weight = int;
constexpr weight INF = 1e9;

struct edge {
    int from, to;
    weight cost;
};

using edges = std::vector<edge>;
using graph = std::vector<edges>;

void add_edge(graph& g, int from, int to, weight cost) {
    g[from].push_back(edge{from, to, cost});
}

weight minimum_spanning_rooted_arborescence(graph const& g, int root, weight sum = 0) {
    int const n = g.size();
    std::vector<int> rev(n, -1), cost(n, INF);
    for(int i = 0; i < n; ++i) {
        for(auto& e : g[i]) {
            if(e.cost < cost[e.to]) {
                cost[e.to] = e.cost;
                rev[e.to] = i;
            }
        }
    }
    for(int i = 0; i < n; ++i) {
        if(i != root && rev[i] == -1) { // not exists
            return INF;
        }
    }

    graph g2(n);
    for(int i = 0; i < n; ++i) {
        if(root == i) {
            continue;
        }
        g2[rev[i]].push_back(edge{rev[i], i, 0}); // ignore cost
        sum += cost[i];
    }
    std::vector<int> cmp(n);
    int const K = scc(g2, cmp);
    auto nxt = build_graph(g2, cmp, K);
    if(nxt.size() == n) {
        return sum;
    }

    graph ng(nxt.size());
    for(int i = 0; i < n; ++i) {
        for(auto& e : g[i]) {
            if(cmp[i] == cmp[e.to]) {
                continue;
            }
            ng[cmp[i]].push_back(edge{cmp[i], cmp[e.to], e.cost - cost[e.to]});
        }
    }
    return minimum_spanning_rooted_arborescence(ng, cmp[root], sum);
}

// not verified
weight maximum_branching(graph const& g) {
    graph g2(g.size() + 1);
    int const super = g.size();
    for(int i = 0; i < (int)g.size(); ++i) {
        for(auto& e : g[i]) {
            add_edge(g2, e.from, e.to, -e.cost);
        }
        add_edge(g2, super, i, 0);
    }
    return -minimum_spanning_rooted_arborescence(g2, super);
}

// not verified
weight minimum_spanning_arborescence(graph const& g) {
    weight K = 1;
    int const n = g.size();
    for(int i = 0; i < n; ++i) {
        for(auto& e : g[i]) {
            K += std::abs(e.cost);
        }
    }

    auto g2 = g;
    for(int i = 0; i < n; ++i) {
        for(auto& e : g2[i]) {
            e.cost = K - e.cost;
        }
    }
    return K * (n - 1) - maximum_branching(g2);
}
