// require: lowlink
class biconnected_component {
public:
    biconnected_component(graph const& g_)
        : g(g_), helper(g_), belongs_(g_.size(), -1)
    {
        for(auto&& b : helper.get_bridges()) {
            add_component(b.from), add_component(b.to);
        }
        add_component(0);
    }

    graph build() { // if not connected, result is forest
        graph res(cmps.size());
        auto bridges = helper.get_bridges();
        for(auto& b : bridges) {
            add_edge(res, belongs(b.from), belongs(b.to));
        }
        return res;
    }

    int belongs(int u) const { return belongs_[u]; }

private:
    void fill_component(int c, int u) {
        cmps[c].emplace_back(u);
        belongs_[u] = c;
        for(auto& e : g[u]) {
            if(belongs_[e.to] >= 0 || helper.is_bridge(u, e.to)) continue;
            fill_component(c, e.to);
        }
    }
    void add_component(int u) {
        if(belongs_[u] >= 0) return;
        cmps.emplace_back();
        fill_component(cmps.size() - 1, u);
    }

private:
    graph g;
    lowlink helper;
    std::vector<int> belongs_;
    std::vector<std::vector<int>> cmps;
};
