using graph = std::vector<std::vector<int>>;

// require: input graph is bipartite
class bipartite_matching {
public:
    bipartite_matching(graph const& g)
        : g_(g), match_(g.size(), -1), used_(g.size(), false)
    {}

    int solve() {
        const int n = g_.size();
        int res = 0;
        bool update = true;
        while(update) {
            update = false;
            for(int v = 0; v < n; ++v) {
                if(match_[v] == -1 && dfs(v)) {
                    update = true;
                    ++res;
                }
            }
            if(update) std::fill(used_.begin(), used_.end(), false);
        }
        return res;
    }

    int match(int v) const {
        return match_[v];
    }

private:
    bool dfs(int v) {
        if(used_[v]) return false;
        used_[v] = true;
        for(auto u : g_[v]) {
            if(match_[u] < 0 || dfs(match_[u])) {
                match_[u] = v;
                match_[v] = u;
                return true;
            }
        }
        return false;
    }

private:
    graph g_;
    std::vector<int> match_;
    std::vector<bool> used_;
};
