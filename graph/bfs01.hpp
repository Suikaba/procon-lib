
// 01-BFS
// @param g: weighted graph, all edge's cost is 0 or 1
// @param s: start vertex
// @return d; shortest distance sequence. foreach vertex i, d[i] is shortest distance from s.
// @requirement: all edge's cost must be 0 or 1.
// @complexity O(E + V)
template <typename Edge, typename Cost = typename Edge::cost_type>
std::vector<Cost> bfs01(graph<Edge> const& g, int s, Cost inf = std::numeric_limits<Cost>::max() / 2) {
    std::vector<Cost> d(g.size(), inf);
    d[s] = Cost{0};
    std::deque<std::pair<Cost, int>> que;
    que.emplace_back(Cost{0}, s);
    while(!que.empty()) {
        const auto cur_d = que.front().first;
        const int v = que.front().second;
        que.pop();
        if(d[v] < cur_d) continue;
        for(auto const& e : g[v]) {
            if(d[e.to] <= d[v] + e.cost) continue;
            d[e.to] = d[v] + e.cost;
            if(e.cost == Cost{0}) {
                que.emplace_front(d[e.to], e.to);
            } else if(e.cost == Cost{1}) {
                que.emplace_back(d[e.to], e.to);
            } else {
                assert(false); // cost must be 0 or 1
            }
        }
    }
    return d;
}

