
class lowest_common_ancestor {
public:
    template <typename Edge>
    lowest_common_ancestor(graph<Edge> const& g, int root)
        : sz(g.size()), log_sz(0), depth(sz, 0)
    {
        for(int v = sz; v > 0; v >>= 1) ++log_sz;
        parent.assign(log_sz, std::vector<int>(sz));
        dfs(g, root, -1, 0);
        for(int k = 0; k < log_sz - 1; ++k) {
            for(int v = 0; v < sz; ++v) {
                if(parent[k][v] < 0) {
                    parent[k + 1][v] = -1;
                } else {
                    parent[k + 1][v] = parent[k][parent[k][v]];
                }
            }
        }
    }

    int query(int u, int v) const {
        if(depth[u] > depth[v]) std::swap(u, v);
        for(int k = 0; k < log_sz; ++k) {
            if(((depth[v] - depth[u]) >> k) & 1) {
                v = parent[k][v];
            }
        }
        if(u == v) return u;
        for(int k = log_sz - 1; k >= 0; --k) {
            if(parent[k][u] != parent[k][v]) {
                u = parent[k][u];
                v = parent[k][v];
            }
        }
        return parent[0][u];
    }

private:
    template <typename Edge>
    void dfs(graph<Edge> const& g, int v, int p, int d) {
        parent[0][v] = p;
        depth[v] = d;
        for(auto const& e : g[v]) {
            if(e.to == p) continue;
            dfs(g, e.to, v, d + 1);
        }
    }

private:
    const int sz;
    int log_sz;
    std::vector<std::vector<int>> parent;
    std::vector<int> depth;
};

