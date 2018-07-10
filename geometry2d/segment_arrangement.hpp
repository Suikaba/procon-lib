struct edge {
    int from, to;
    ld cost;
};

using edges = std::vector<edge>;
using graph = std::vector<edges>;

void add_edge(graph& g, int from, int to, ld cost) {
    g[from].push_back(edge{from, to, cost});
    g[to].push_back(edge{to, from, cost});
}

graph segment_arrangement(std::vector<segment> const& s, std::vector<point> const& p) {
    const int m = s.size();
    const int n = p.size();
    graph g(n);
    for(int i = 0; i < m; ++i) {
        std::vector<std::pair<ld, int>> v;
        for(int j = 0; j < n; ++j) {
            if(isis_sp(s[i], p[j])) {
                v.emplace_back(std::abs(s[i].a - p[j]), j);
            }
        }
        std::sort(std::begin(v), std::end(v));
        for(int j = 0; j < (int)v.size() - 1; ++j) {
            int src = v[j].second, dest = v[j + 1].second;
            add_edge(g, src, dest, std::abs(p[src] - p[dest]));
        }
    }
    return g;
}
