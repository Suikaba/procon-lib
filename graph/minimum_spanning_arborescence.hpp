
using weight = int;
constexpr weight INF = 1e9;

struct edge {
    int from, to;
    weight cost;
    
    bool operator<(edge const& e) const {
        return cost < e.cost;
    }
};

using edges = std::vector<edge>;
using graph = std::vector<edges>;

void add_edge(graph& g, int from, int to, weight cost) {
    g[from].push_back(edge{from, to, cost});
}

int scc(std::vector<std::vector<int>>& G, std::vector<int>& cmp) {
    int V = G.size();
    std::vector<std::vector<int>> g(V), rg(V);
    std::vector<bool> used(V, false);
    std::vector<int> vs;
    cmp.resize(V);
    for(int i = 0; i < V; ++i) {
        for(auto to : G[i]) {
            g[i].push_back(to);
            rg[to].push_back(i);
        }
    }
    std::function<void(int)> dfs = [&g, &vs, &used, &dfs](int v) {
        used[v] = true;
        for(auto i : g[v]) {
            if(!used[i]) {
                dfs(i);
            }
        }
        vs.push_back(v);
    };
    std::function<void(int, int)> rdfs = [&rg, &cmp, &used, &rdfs](int v, int k) {
        used[v] = true;
        cmp[v] = k;
        for(int i : rg[v]) {
            if(!used[i]) {
                rdfs(i, k);
            }
        }
    };
    for(int v=0; v<V; ++v) {
        if(!used[v]) {
            dfs(v);
        }
    }
    std::fill(used.begin(), used.end(), false);
    int k = 0;
    for(int i=vs.size()-1; i>=0; --i) {
        if(!used[vs[i]]) {
            rdfs(vs[i], k++);
        }
    }
    return k;
}

std::vector<std::vector<int>> build_graph(std::vector<std::vector<int>> const& g, std::vector<int> const& cmp, int K) {
    int V = g.size();
    std::vector<std::set<int>> s(K);
    std::vector<std::vector<int>> res(K);
    for(int i = 0; i < V; ++i) {
        for(auto to : g[i]) {
            s[cmp[i]].insert(cmp[to]);
        }
    }
    for(int i = 0; i < K; ++i) {
        for(auto j : s[i]) {
            if(i != j) {
                res[i].push_back(j);
            }
        }
    }
    return res;
}


weight minimum_spanning_arborescence(graph const& g, int root, weight sum = 0) {
    int const n = g.size();

    std::vector<int> rev(n, -1), cost(n, INF);
    for(int i = 0; i < n; ++i) {
        for(auto& e : g[i]) {
            if(e.cost < cost[e.to]) {
                cost[e.to] = e.cost;
                rev[e.to] = i;
            }
        }
    }
    for(int i = 0; i < n; ++i) {
        if(i != root && rev[i] == -1) { // not exists
            return INF;
        }
    }

    std::vector<std::vector<int>> g2(n);
    for(int i = 0; i < n; ++i) {
        if(root == i) {
            continue;
        }
        g2[rev[i]].push_back(i);
        sum += cost[i];
    }
    std::vector<int> cmp(n);
    int const K = scc(g2, cmp);
    auto nxt = build_graph(g2, cmp, K);
    if(nxt.size() == n) {
        return sum;
    }

    graph ng(nxt.size());
    for(int i = 0; i < n; ++i) {
        for(auto& e : g[i]) {
            if(cmp[i] == cmp[e.to]) {
                continue;
            }
            ng[cmp[i]].push_back(edge{cmp[i], cmp[e.to], e.cost - cost[e.to]});
        }
    }

    return minimum_spanning_arborescence(ng, cmp[root], sum);
}

