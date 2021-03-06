
template <typename Edge>
typename Edge::cost_type min_cost_flow(graph<Edge>& g, int s, int t, typename Edge::capacity_type f) {
    using cost_type = typename Edge::cost_type;
    using P = std::pair<cost_type, int>;
    const auto inf = std::numeric_limits<cost_type>::max() / 2;
    cost_type res = 0;
    std::vector<cost_type> h(g.size()), dist(g.size());
    std::vector<int> prevv(g.size()), preve(g.size());
    while(f > 0) {
        std::priority_queue<P, std::vector<P>, std::greater<>> que;
        std::fill(std::begin(dist), std::end(dist), inf);
        dist[s] = 0;
        que.emplace(dist[s], s);
        while(!que.empty()) {
            const auto cur_d = que.top().first;
            const int v = que.top().second;
            que.pop();
            if(dist[v] < cur_d) continue;
            for(int i = 0; i < (int)g[v].size(); ++i) {
                auto& e = g[v][i];
                if(e.cap > 0 && dist[e.to] > dist[v] + e.cost + h[v] - h[e.to]) {
                    dist[e.to] = dist[v] + e.cost + h[v] - h[e.to];
                    prevv[e.to] = v;
                    preve[e.to] = i;
                    que.emplace(dist[e.to], e.to);
                }
            }
        }
        if(dist[t] == inf) return -1;
        for(int v = 0; v < (int)g.size(); ++v) {
            h[v] += dist[v];
        }

        auto d = f;
        for(int v = t; v != s; v = prevv[v]) {
            d = std::min(d, g[prevv[v]][preve[v]].cap);
        }
        f -= d;
        res += d * h[t];
        for(int v = t; v != s; v = prevv[v]) {
            auto& e = g[prevv[v]][preve[v]];
            e.cap -= d;
            g[v][e.rev].cap += d;
        }
    }
    return res;
}

