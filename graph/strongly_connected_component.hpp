struct edge {
    int from, to;
};

using edges = std::vector<edge>;
using graph = std::vector<edges>;

void add_edge(graph& g, int from, int to) {
    g[from].push_back((edge){from, to});
}

int scc(graph& G, std::vector<int>& cmp) {
    int V = G.size();
    std::vector<std::vector<int>> g(V), rg(V);
    std::vector<bool> used(V, false);
    std::vector<int> vs;
    cmp.resize(V);
    for(int i = 0; i < V; ++i) {
        for(auto e : G[i]) {
            g[i].push_back(e.to);
            rg[e.to].push_back(i);
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
    for(int v = 0; v < V; ++v) {
        if(!used[v]) {
            dfs(v);
        }
    }
    std::fill(used.begin(), used.end(), false);
    int k = 0;
    for(int i = vs.size() - 1; i >= 0; --i) {
        if(!used[vs[i]]) {
            rdfs(vs[i], k++);
        }
    }
    return k;
}

std::vector<std::vector<int>> build_graph(graph const& g, std::vector<int> const& cmp, int K) {
    int V = g.size();
    std::vector<std::set<int>> s(K);
    std::vector<std::vector<int>> res(K);
    for(int i = 0; i < V; ++i) {
        for(auto e : g[i]) {
            s[cmp[i]].insert(cmp[e.to]);
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
