
using weight = int;

struct edge {
    int to, cap;
    weight cost;
    int rev;
};

using edges = std::vector<edge>;
using graph = std::vector<edges>;

void add_edge(graph& g, int from, int to, int cap, weight cost) {
    g[from].push_back(edge{to, cap, cost, (int)g[to].size()});
    g[to].push_back(edge{from, 0, -cost, (int)g[from].size()-1});
}

weight min_cost_flow(graph& g, int s, int t, weight f) {
    using P = std::pair<weight, int>;
    weight res = 0;
    std::vector<weight> h(g.size(), 0);
    std::vector<weight> dist(g.size());
    std::vector<int> prevv(g.size()), preve(g.size());
    while(f > 0) {
        std::priority_queue<P, std::vector<P>, std::greater<P>> que;
        std::fill(dist.begin(), dist.end(), INF);
        dist[s] = 0;
        que.push(P{dist[s], s});
        while(!que.empty()) {
            P p = que.top(); que.pop();
            int v = p.second;
            if(dist[v] < p.first) {
                continue;
            }
            for(int i = 0; i < (int)g[v].size(); ++i) {
                edge& e = g[v][i];
                if(e.cap > 0 && dist[e.to] > dist[v] + e.cost + h[v] - h[e.to]) {
                    dist[e.to] = dist[v] + e.cost + h[v] - h[e.to];
                    prevv[e.to] = v;
                    preve[e.to] = i;
                    que.push(P{dist[e.to], e.to});
                }
            }
        }
        if(dist[t] == INF) {
            return -1;
        }
        for(int v = 0; v < (int)g.size(); ++v) {
            h[v] += dist[v];
        }

        weight d = f;
        for(int v = t; v != s; v = prevv[v]) {
            d = min(d, g[prevv[v]][preve[v]].cap);
        }
        f -= d;
        res += d * h[t];
        for(int v = t; v != s; v = prevv[v]) {
            edge& e = g[prevv[v]][preve[v]];
            e.cap -= d;
            g[v][e.rev].cap += d;
        }
    }
    return res;
}


