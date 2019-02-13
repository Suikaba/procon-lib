
template <typename Edge, typename Capacity = typename Edge::capacity_type>
Capacity augument(graph<Edge>& g, std::vector<bool>& used, int v, int t, Capacity f) {
    if(v == t) return f;
    used[v] = true;
    for(auto& e : g[v]) {
        if(!used[e.to] && e.cap > 0) {
            auto d = augument(g, used, e.to, t, std::min(f, e.cap));
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
    Capacity flow = 0;
    std::vector<bool> used(g.size(), false);
    while(true) {
        std::fill(std::begin(used), std::end(used), false);
        const auto f = augument(g, used, s, t, std::numeric_limits<Capacity>::max() / 2);
        if(f == 0) return flow;
        flow += f;
    }
}

