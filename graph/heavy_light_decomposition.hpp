class heavy_light_decomposition {
public:
    heavy_light_decomposition(int n_)
        : n(n_), g(n), par(n), head(n), in(n), out(n)
    {}
    heavy_light_decomposition(std::vector<std::vector<int>> const& g_)
        : n(g_.size()), g(g_), par(n), head(n), in(n), out(n)
    {}
    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }
    void build(int rt = 0) {
        dfs1(rt);
        head[rt] = rt;
        dfs2(rt);
    }

    int get_id(int v) const { return in[v]; }

    void path_query(int u, int v, std::function<void(int, int)> f) { // vertex
        while(true) {
            if(in[u] > in[v]) std::swap(u, v);
            f(std::max(in[head[v]], in[u]), in[v] + 1);
            if(head[u] == head[v]) return;
            v = par[head[v]];
        }
    }
    void edge_path_query(int u, int v, std::function<void(int, int)> f) {
        while(true) {
            if(in[u] > in[v]) std::swap(u, v);
            if(head[u] != head[v]) {
                f(std::max(in[head[v]], in[u]), in[v] + 1);
                v = par[head[v]];
            } else {
                if(u != v) f(in[u] + 1, in[v] + 1);
                break;
            }
        }
    }
    void subtree_query(int v, std::function<void(int, int)> f) {
        f(in[v], out[v]);
    }
    int get_lca(int u, int v) const {
        while(true) {
            if(in[u] > in[v]) std::swap(u, v);
            if(head[u] == head[v]) return u;
            v = par[head[v]];
        }
    }

private:
    void dfs1(int rt) {
        std::vector<int> sz(n, 1), iter(n);
        std::vector<std::pair<int, int>> st = {{rt, -1}};
        st.reserve(n);
        while(!st.empty()) {
            const int v = st.back().first;
            if(iter[v] < (int)g[v].size()) {
                if(g[v][iter[v]] != st.back().second) {
                    st.emplace_back(g[v][iter[v]], v);
                }
                ++iter[v];
                continue;
            }
            par[v] = st.back().second;
            if(par[v] != -1) {
                const int pidx = std::find(std::begin(g[v]), std::end(g[v]), par[v]) - std::begin(g[v]);
                std::swap(g[v].back(), g[v][pidx]);
                g[v].pop_back();
            }
            for(auto& u : g[v]) {
                sz[v] += sz[u];
                if(sz[u] > sz[g[v].front()]) std::swap(u, g[v].front());
            }
            st.pop_back();
        }
    }
    void dfs2(int rt) {
        int it = 0;
        std::vector<int> st = {rt}, iter(n);
        st.reserve(n);
        while(!st.empty()) {
            const int v = st.back();
            if(!iter[v]) in[v] = it++; // first visit
            if(iter[v] < (int)g[v].size()) {
                int u = g[v][iter[v]];
                head[u] = iter[v] ? u : head[v];
                st.push_back(u);
                ++iter[v];
                continue;
            }
            out[v] = it;
            st.pop_back();
        }
    }

private:
    const int n;
    std::vector<std::vector<int>> g;
    std::vector<int> par, head, in, out;
};
