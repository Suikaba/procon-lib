class max_clique {
public:
    max_clique(int v)
        : V(v), ans(0), dp(max_V),
          el(max_V, std::vector<int>(max_V / 30 + 1)),
          s(max_V, std::vector<int>(max_V / 30 + 1))
    {}

    void add_edge(int u, int v) {
        if(u > v) {
            swap(u, v);
        }
        if(u == v) {
            return;
        }
        el[u][v / 32] |= (1 << (v % 32));
    }

    int solve() {
        for(int i = V - 1; i >= 0; --i) {
            dfs(i, 1);
            dp[i] = ans;
        }
        return ans;
    }

private:
    bool dfs(int v, int k) {
        int c = 0, d = 0;
        for(int i = 0; i < (V + 31) / 32; ++i) {
            s[k][i] = el[v][i];
            if(k != 1) {
                s[k][i] &= s[k - 1][i];
            }
            c += __builtin_popcount(s[k][i]);
        }
        if(c == 0) {
            if(k > ans) {
                ans = k;
                sol.clear();
                sol.push_back(v);
                return true;
            }
            return false;
        }
        for(int i = 0; i < (V + 31) / 32; ++i) {
            for(int a = s[k][i]; a != 0; ++d) {
                if(k + (c - d) <= ans) {
                    return false;
                }
                int lb = a & (-a), lg = 0;
                a ^= lb;
                while(lb != 1) {
                    lb = (unsigned int)(lb) >> 1;
                    lg++;
                }
                int u = i * 32 + lg;
                if(k + dp[u] <= ans) {
                    return false;
                }
                if(dfs(u, k + 1)) {
                    sol.push_back(v);
                    return true;
                }
            }
        }
        return false;
    }

private:
    static int const max_V = 210;
    int const V;
    int ans;
    std::vector<int> dp;
    std::vector<int> sol;
    std::vector<std::vector<int>> el, s;
};
