using graph = std::vector<std::vector<int>>;

bool dfs(graph const& g, int v, std::vector<int>& match, std::vector<bool>& used) {
    used[v] = true;
    for(auto u : g[v]) {
        int w = match[u];
        if(w < 0 || !used[w] && dfs(g, w, match, used)) {
            match[v] = u;
            match[u] = v;
            return true;
        }
    }
    return false;
}

int bipartite_matching(graph const& g) {
    const int V = g.size();
    std::vector<int> match(V, -1);
    std::vector<bool> used(V);
    int res = 0;
    for(int v = 0; v < V; ++v) {
        if(match[v] < 0) {
            std::fill(used.begin(), used.end(), false);
            if(dfs(g, v, match, used)) {
                ++res;
            }
        }
    }
    return res;
}
