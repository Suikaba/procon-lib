struct edge {
    int from, to;
};

using edges = std::vector<edge>;
using graph = std::vector<edges>;

void add_edge(graph& g, int from, int to) {
    g[from].push_back(edge{from, to});
}

class strongly_connected_component {
public:
    strongly_connected_component(graph const& G) 
    : g(G), cmp(G.size()), K(0) {
        int const V = G.size();
        std::vector<std::vector<int>> rg(V);
        for(int v = 0; v < V; ++v) {
            for(auto& e : g[v]) {
                rg[e.to].push_back(v);
            }
        }
        std::vector<bool> used(V);
        for(int v = 0; v < V; ++v) {
            if(!used[v]) dfs(v, used);
        }
        std::fill(std::begin(used), std::end(used), false);
        int k = 0;
        for(int i = (int)vs.size() - 1; i >= 0; --i) {
            if(!used[vs[i]]) rdfs(vs[i], k++, rg, used);
        }
        K = k;
    }

    int belongs(int v) const {
        return cmp[v];
    }

    std::vector<std::vector<int>> build_graph() const {
        int const V = g.size();
        std::vector<std::set<int>> s(K);
        std::vector<std::vector<int>> res(K);
        for(int v = 0; v < V; ++v) {
            for(auto& e : g[v]) {
                s[cmp[v]].insert(cmp[e.to]);
            }
        }
        for(int i = 0; i < K; ++i) {
            for(auto j : s[i]) {
                if(i != j) res[i].push_back(j);
            }
        }
        return res;
    }

private:
    void dfs(int v, std::vector<bool>& used) {
        used[v] = true;
        for(auto& e : g[v]) {
            if(!used[e.to]) dfs(e.to, used);
        }
        vs.push_back(v);
    }
    void rdfs(int v, int k, std::vector<std::vector<int>> const& rg, std::vector<bool>& used) {
        used[v] = true;
        cmp[v] = k;
        for(auto to : rg[v]) {
            if(!used[to]) rdfs(to, k, rg, used);
        }
    }

private:
    graph const& g;
    std::vector<int> cmp, vs;
    int K;
};