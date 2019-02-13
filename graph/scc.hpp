
// Strongly Connected Components.
// @param g: directed graph
// @return cmp, cmp[i] is connected component index of vertex i
// @complexity O(V + E)
template <typename Edge>
std::vector<int> scc(graph<Edge> const& g) {
    const int n = g.size();
    int idx = 0, k = 0, s = 0;
    std::vector<int> ord(n, -1), low(n), on_s(n), cmp(n), stk(n);
    std::function<void(int)> dfs = [&] (int v) {
        ord[v] = low[v] = idx++;
        stk[s++] = v;
        on_s[v] = true;
        for(const auto& e : g[v]) {
            if(ord[e.to] == -1) {
                dfs(e.to);
                low[v] = std::min(low[v], low[e.to]);
            } else if(on_s[e.to]) {
                low[v] = std::min(low[v], ord[e.to]);
            }
        }
        if(low[v] == ord[v]) {
            while(true) {
                const int w = stk[--s];
                on_s[w] = false;
                cmp[w] = k;
                if(w == v) break;
            }
            ++k;
        }
    };
    for(int v = 0; v < n; ++v) {
        if(ord[v] == -1) dfs(v);
    }

    return cmp;
}

