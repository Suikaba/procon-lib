 
using weight = int;

struct edge {
    int from, to;
    weight cost;
};

using edges = std::vector<edge>;
using graph = std::vector<edges>;

void add_edge(graph& g, int from, int to, weight cost) {
    g[from].push_back(edge{from, to, cost});
}

std::vector<weight> bfs01(graph const& g, int s) {
    std::vector<weight> d(g.size(), inf<weight>);
    d[s] = 0;
    std::deque<std::pair<weight, int>> que;
    que.emplace_back(d[s], s);
    while(!que.empty()) {
        weight cur_d = que.front().first;
        int v = que.front().second;
        que.pop_front();
        if(d[v] < cur_d) {
            continue;
        }
        for(auto& e : g[v]) {
            if(d[e.to] > d[v] + e.cost) {
                d[e.to] = d[v] + e.cost;
                if(e.cost == 1) {
                    que.emplace_back(d[e.to], e.to);
                } else {
                    que.emplace_front(d[e.to], e.to);
                }
            }
        }
    }
    return d;
}

