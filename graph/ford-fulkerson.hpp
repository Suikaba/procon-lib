
// ford-fulkerson
struct edge {
    int to, cap, rev;
};

using edges = std::vector<edge>;
using graph = std::vector<edges>;

void add_edge(graph& g, int from, int to, int cap) {
    g[from].push_back(edge{to, cap, static_cast<int>(g[to].size())});
    g[to].push_back(edge{from, 0, static_cast<int>(g[from].size()-1)});
}

int dfs(graph& g, std::vector<bool>& used, int v, int t, int f) {
    if(v == t) {
        return f;
    }
    used[v] = true;
    for(int i=0; i<g[v].size(); ++i) {
        edge& e = g[v][i];
        if(!used[e.to] && e.cap > 0) {
            int d = dfs(g, used, e.to, t, std::min(f, e.cap));
            if(d > 0) {
                e.cap -= d;
                g[e.to][e.rev].cap += d;
                return d;
            }
        }
    }
    return 0;
}

int max_flow(graph& g, int s, int t) {
    int flow = 0;
    int INF = 1e9;
    std::vector<bool> used(g.size(), false);
    while(true) {
        std::fill(used.begin(), used.end(), false);
        int f = dfs(g, used, s, t, INF);
        if(f == 0) {
            return flow;
        }
        flow += f;
    }

}


