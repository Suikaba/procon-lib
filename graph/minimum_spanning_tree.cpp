
using weight = int;

struct edge {
    int from, to;
    weight cost;
    bool operator<(edge const& rhs) const {
        return cost < rhs.cost;
    }
};

using edges = std::vector<edge>;
using graph = std::vector<edges>;

edges kruskal (int V, edges& es) {
    std::sort(es.begin(), es.end());
    union_find uf(V);
    edges res;
    for(int i=0; i<es.size(); ++i) {
        edge e = es[i];
        if(!uf.same(e.from, e.to)) {
            uf.unite(e.from, e.to);
            res.push_back(e);
        }
    }
    return res;
}

edges prim(graph& g) {
    edges res;
    std::vector<bool> used(g.size(), 0);
    priority_queue<edge> que;
    que.push((edge{-1, 0, 0}));
    while(!que.empty()) {
        edge e = que.top(); que.pop();
        if(used[e.to]) continue;
        if(e.from >= 0) {
            res.push_back(e);
        }
        used[e.to] = true;
        for(auto ee : g[e.to]) {
            if(!used[ee.to]) {
                que.push(ee);
            }
        }
    }
    sort(res.rbegin(), res.rend());
    return res;
}
