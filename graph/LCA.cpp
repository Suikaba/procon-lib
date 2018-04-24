struct edge {
    int from, to;
};

using edges = std::vector<edge>;
using graph = std::vector<edges>;

void add_edge(graph& g, int from, int to) {
    g[from].push_back(edge{from, to});
    g[to].push_back(edge{to, from});
}

class lowest_common_ancestor {
public:
    lowest_common_ancestor(graph const& g, int root)
        : n(g.size()), log_n(std::ceil(std::log2(g.size())) + 1),
          parent(log_n, std::vector<int>(n, -1)), depth_(n)
    {
        dfs(g, root, -1, 0);
        for(int k = 0; k + 1 < log_n; ++k) {
            for(int v = 0; v < n; ++v) {
                if(parent[k][v] < 0) parent[k + 1][v] = -1;
                else                 parent[k + 1][v] = parent[k][parent[k][v]];
            }
        }
    }

    int depth(int v) const {
        return depth_[v];
    }

    int dist(int u, int v) const {
        return depth(u) + depth(v) - 2 * depth(query(u, v));
    }

    int query(int u, int v) const {
        if(depth_[u] > depth_[v]) {
            std::swap(u, v);
        }
        for(int k = 0; k < log_n; ++k) {
            if((depth_[v] - depth_[u]) >> k & 1) {
                v = parent[k][v];
            }
        }
        if(u == v) {
            return u;
        }
        for(int k = log_n - 1; k >= 0; --k) {
            if(parent[k][u] != parent[k][v]) {
                u = parent[k][u];
                v = parent[k][v];
            }
        }
        return parent[0][u];
    }

private:
    void dfs(graph const& g, int v, int p, int d) {
        parent[0][v] = p;
        depth_[v] = d;
        for(auto& e : g[v]) {
            if(e.to != p) {
                dfs(g, e.to, v, d + 1);
            }
        }
    }

private:
    int const n, log_n;
    std::vector<std::vector<int>> parent;
    std::vector<int> depth_;
};