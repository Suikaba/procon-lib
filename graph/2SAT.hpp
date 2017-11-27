class two_SAT {
public:
    two_SAT(int n)
        : V(2 * n), N(n), g(2 * n), rg(2 * n), used(2 * n), cmp(2 * n)
    {}

    // example: x_2 | !x_5 -> add_closure(2, 1, 5, 0)
    void add_closure(int i, bool bi, int j, bool bj) {
        add_edge(i + N * bi, j + N * !bj);
        add_edge(j + N * bj, i + N * !bi);
    }
    void add_edge(int from, int to) {
        g[from].push_back(to);
        rg[to].push_back(from);
    }

    std::vector<bool> solve() {
        std::fill(used.begin(), used.end(), false);
        vs.clear();
        for(int v = 0; v < V; ++v) {
            if(!used[v]) {
                dfs(v);
            }
        }
        std::fill(used.begin(), used.end(), false);
        std::reverse(vs.begin(), vs.end());
        int k = 0;
        for(int i : vs) {
            if(!used[i]) {
                rdfs(i, k++);
            }
        }
        vector<bool> res;
        for(int i = 0; i < N; ++i) {
            if(cmp[i] == cmp[i + N]) {
                return res;
            }
        }
        for(int i = 0; i < N; ++i) {
            res.push_back(cmp[i] > cmp[N + i]);
        }
        return res;
    }

private:
    void dfs(int v) {
        used[v] = true;
        for(int i : g[v]) {
            if(!used[i]) {
                dfs(i);
            }
        }
        vs.push_back(v);
    }
    void rdfs(int v, int k) {
        used[v] = true;
        cmp[v] = k;
        for(int i : rg[v]) {
            if(!used[i]) {
                rdfs(i, k);
            }
        }
    }

private:
    int V, N;
    std::vector<vector<int>> g, rg;
    std::vector<int> vs;
    std::vector<bool> used;
    std::vector<int> cmp;
};
