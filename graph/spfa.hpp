
// Shortest Path Faster Algorithm.
// Solve single source shortest path problem.
// @param g: weighted graph
// @param s: start node
// @param inf: value big enough to represent infty (default max() / 2)
// @return d (foreach vertex i, d[i] is shortest distance from s).
//         If g has negative cycle, d is empty.
// @complexity O(VE)
template <typename Edge, typename Cost = typename Edge::cost_type>
std::vector<Cost> spfa(graph<Edge> const& g, int s, Cost inf = std::numeric_limits<Cost>::max() / 2) {
    std::vector<Cost> d(g.size(), inf);
    std::vector<int> in_q(g.size()), count(g.size());
    std::queue<int> que;
    d[s] = Cost(0);
    in_q[s] = true;
    count[s] = 1;
    que.push(s);
    while(!que.empty()) {
        const int v = que.front();
        que.pop();
        in_q[v] = false;
        for(const auto& e : g[v]) {
            if(d[e.to] <= d[v] + e.cost) continue;
            d[e.to] = d[v] + e.cost;
            if(in_q[e.to]) continue;
            que.emplace(e.to);
            in_q[e.to] = true;
            if(++count[e.to] >= static_cast<int>(g.size())) {
                return std::vector<Cost>{};
            }
        }
    }
    return d;
}

