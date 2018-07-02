using weight = int;

struct edge {
    int to;
    weight cost;
};

using edges = std::vector<edge>;
using graph = std::vector<edges>;

// O(VE)
// when g has neg cycle, result is empty
std::vector<weight> spfa(graph const& g, int s) {
    constexpr weight inf = std::numeric_limits<weight>::max() / 2;
    const int n = g.size();
    std::vector<weight> d(n, inf);
    std::vector<int> in_q(n), count(n);
    std::queue<int> que;
    d[s] = 0;
    que.emplace(s);
    in_q[s] = true;
    ++count[s];
    bool neg_cycle = false;
    while(!que.empty()) {
        int v = que.front();
        que.pop();
        in_q[v] = false;
        for(auto& e : g[v]) {
            if(d[v] != inf && d[e.to] > d[v] + e.cost) {
                d[e.to] = d[v] + e.cost;
                if(!in_q[e.to]) {
                    que.emplace(e.to);
                    in_q[e.to] = true;
                    ++count[e.to];
                    if(count[e.to] >= n) {
                        neg_cycle = true;
                        return std::vector<weight>();
                    }
                }
            }
        }
    }
    return d;
}
