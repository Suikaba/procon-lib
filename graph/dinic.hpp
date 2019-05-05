
template <typename Edge, typename Capacity = typename Edge::capacity_type>
Capacity augment(graph<Edge>& g, std::vector<int> const& level, std::vector<int>& iter, int v, int t, Capacity f) {
    if(v == t) return f;
    for(int& i = iter[v]; i < (int)g[v].size(); ++i) {
        auto& e = g[v][i];
        if(e.cap > 0 && level[v] < level[e.to]) {
            const auto d = augment(g, level, iter, e.to, t, std::min(f, e.cap));
            if(d > 0) {
                e.cap -= d;
                g[e.to][e.rev].cap += d;
                return d;
            }
        }
    }
    return 0;
}

template <typename Edge, typename Capacity = typename Edge::capacity_type>
Capacity max_flow(graph<Edge>& g, int s, int t) {
    const auto inf = std::numeric_limits<Capacity>::max() / 2;
    Capacity flow = 0;
    while(true) {
        std::vector<int> level(g.size(), -1);
        level[s] = 0;
        std::queue<int> que;
        que.push(s);
        while(!que.empty()) {
            const int v = que.front();
            que.pop();
            for(auto const& e : g[v]) {
                if(e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
        }

        if(level[t] < 0) return flow;
        std::vector<int> iter(g.size());
        Capacity f{0};
        while((f = augment(g, level, iter, s, t, inf)) > 0) {
            flow += f;
        }
    }
}

