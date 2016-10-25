
using weight = int;

constexpr weight INF = 1e9;

struct edge {
    int from, to;
    weight cost;

    bool operator<(edge const& rhs) const {
        return cost > rhs.cost;
    }
};

using edges = std::vector<edge>;
using graph = std::vector<edges>;


void add_edge(graph& g, int from, int to, weight cost) {
    g[from].push_back(edge{from, to, cost});
}

// bellman-ford(verified)
bool bellman_ford(edges const& es, std::vector<weight>& d, int s) {
    std::fill(d.begin(), d.end(), INF);
    d[s] = 0;
    for(int i=0; i<d.size(); ++i) {
        bool update = false;
        for(edge e : es) {
            if(d[e.from] != INF && d[e.to] > d[e.from] + e.cost) {
                d[e.to] = d[e.from] + e.cost;
                if(i == d.size() - 1) {
                    return false;
                }
                update = true;
            }
        }
        if(!update) {
            return true;
        }
    }
    return true;
}

// dijkstra(verified)
std::vector<weight> dijkstra(graph& g, int s) {
    using P = std::pair<weight, int>;
    std::vector<weight> d(g.size(), INF);
    std::priority_queue<P, std::vector<P>, std::greater<P>> que;
    fill(d.begin(), d.end(), INF);
    d[s] = 0;
    que.push(P{0, s});

    while(!que.empty()) {
        P p = que.top(); que.pop();
        int v = p.second;
        if(d[v] < p.first) {
            continue;
        }
        for(int i=0; i<g[v].size(); ++i) {
            edge e = g[v][i];
            if(d[e.to] > d[v] + e.cost) {
                d[e.to] = d[v] + e.cost;
                que.push(P{d[e.to], e.to});
            }
        }
    }
    return d;
}

// dijkstra (build path)
std::vector<weight> dijkstra(graph& g, int s, std::vector<int>& prev) {
    std::vector<weight> d(g.size(), INF);
    d[s] = 0;
    prev.assign(g.size(), -1);
    std::priority_queue<edge> que;
    // edge {from(prev), to, cost(total)}
    for(que.push(edge{-1, s, 0}); !que.empty();) {
        edge e = que.top(); que.pop();
        if(prev[e.to] != -1)  {
            continue;
        }
        prev[e.to] = e.from;
        for(edge& f : g[e.to]) {
            if(d[f.to] > e.cost + f.cost) {
                d[f.to] = e.cost + f.cost;
                que.push(edge{f.from, f.to, e.cost + f.cost});
            }
        }
    }
    return d;
}
std::vector<int> get_path(std::vector<int> const& prev, int t) {
    std::vector<int> path;
    for(; t!=-1; t=prev[t]) {
        path.push_back(t);
    }
    std::reverse(path.begin(), path.end());
    return path;
}


// warshall-floyd(verified)
void warshall_floyd(std::vector<std::vector<weight>>& g) {
    int V = g.size();
    for(int k=0; k<V; ++k) {
        for(int i=0; i<V; ++i) {
            for(int j=0; j<V; ++j) {
                g[i][j] = std::min(g[i][j], g[i][k] + g[k][j]);
            }
        }
    }
}

