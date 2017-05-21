
struct edge {
    int from, to;
};

using edges = std::vector<edge>;
using graph = std::vector<edges>;

void add_edge(graph& g, int from, int to) {
    g[from].push_back((edge){from, to});
    g[to].push_back((edge){to, from});
}


class lca {
public:
    lca(graph& g, int root)
        : V(g.size()),
          log_V(0),
          depth_(V, 0)
    {
        for(int v=V; v>0; v /= 2) {
            ++log_V;
        }
        parent.assign(log_V, std::vector<int>(V, 0));
        dfs(g, root, -1, 0);
        for(int k=0; k+1 < log_V; ++k) {
            for(int v=0; v<V; ++v) {
                if(parent[k][v] < 0) {
                    parent[k+1][v] = -1;
                } else {
                    parent[k+1][v] = parent[k][parent[k][v]];
                }
            }
        }
    }

    void dfs(graph const& g, int v, int p, int d) {
        parent[0][v] = p;
        depth_[v] = d;
        for(int i=0; i<g[v].size(); ++i) {
            if(g[v][i].to != p) {
                dfs(g, g[v][i].to, v, d+1);
            }
        }
    }

    int depth(int v) const {
        return depth_[v];
    }

    int query(int u, int v) {
        if(depth_[u] > depth_[v]) {
            std::swap(u, v);
        }
        for(int k=0; k<log_V; ++k) {
            if((depth_[v] - depth_[u]) >> k & 1) {
                v = parent[k][v];
            }
        }
        if(u == v) {
            return u;
        }
        for(int k=log_V - 1; k>=0; --k) {
            if(parent[k][u] != parent[k][v]) {
                u = parent[k][u];
                v = parent[k][v];
            }
        }
        return parent[0][u];
    }

private:
    int V, log_V;
    std::vector<vector<int>> parent;
    std::vector<int> depth_;
};
