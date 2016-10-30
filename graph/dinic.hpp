
using weight = int;

struct edge {
    int to;
    weight cap;
    int rev;
};

using edges = std::vector<edge>;
using graph = std::vector<edges>;

void add_edge(graph& g, int from, int to, weight cap) {
    g[from].push_back(edge{to, cap, static_cast<int>(g[to].size())});
    g[to].push_back(edge{from, 0, static_cast<int>(g[from].size()-1)});
}

void bfs(graph& g, std::vector<int>& level, int s) {
    for(int i=0; i<level.size(); ++i) {
        level[i] = -1;
    }
    level[s] = 0;
    std::queue<weight> que;
    que.push(s);
    while(!que.empty()) {
        int v = que.front(); que.pop();
        for(int i=0; i<g[v].size(); ++i) {
            edge& e = g[v][i];
            if(e.cap > 0 && level[e.to] < 0) {
                level[e.to] = level[v] + 1;
                que.push(e.to);
            }
        }
    }
}

weight dfs(graph& g, std::vector<int>& level, std::vector<int>& iter, int v, int t, weight f) {
    if(v == t) {
        return f;
    }
    for(int& i=iter[v]; i<g[v].size(); ++i) {
        edge& e = g[v][i];
        if(e.cap > 0 && level[v] < level[e.to]) {
            int d = dfs(g, level, iter, e.to, t, std::min(f, e.cap));
            if(d > 0) {
                e.cap -= d;
                g[e.to][e.rev].cap += d;
                return d;
            }
        }
    }
    return 0;
}

// verified
weight max_flow(graph& g, int s, int t) {
    weight flow = 0;
    std::vector<int> level(g.size(), -1);
    std::vector<int> iter(g.size(), 0);
    int INF = 1e9;
    while(true) {
        bfs(g, level, s);
        if(level[t] < 0) {
            return flow;
        }
        for(int i=0; i<iter.size(); ++i) {
            iter[i] = 0;
        }
        weight f;
        while((f = dfs(g, level, iter, s, t, INF)) > 0) {
            flow += f;
        }
    }
}

