
// Dijkstra algorithm.
// Solve single source shortest path problem.
// @param g: weighted graph
// @param s: source vertex index
// @return d (foreach vertex i, d[i] is shortest-distance from s)
// @complexity O(E log V)
// @requirement: none of edges in graph g have negative cost
template <typename Edge, typename Cost = typename Edge::cost_type>
std::vector<Cost> dijkstra(graph<Edge> const& g, int s, Cost inf = std::numeric_limits<Cost>::max() / 2) {
    std::vector<Cost> d(g.size(), inf);
    d[s] = Cost(0);
    using state = std::pair<Cost, int>;
    std::priority_queue<state, std::vector<state>, std::greater<>> que;
    que.emplace(Cost(0), s);
    while(!que.empty()) {
        const auto cur_d = que.top().first;
        const int v = que.top().second;
        que.pop();
        if(d[v] < cur_d) continue;
        for(auto const& e : g[v]) {
            if(d[e.to] <= d[v] + e.cost) continue;
            d[e.to] = d[v] + e.cost;
            que.emplace(d[e.to], e.to);
        }
    }
    return d;
}

