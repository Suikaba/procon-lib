// Kahn's algorithm. topological sort.
std::vector<int> tsort(std::vector<std::vector<int>> const& g) {
    const int n = g.size();
    std::vector<int> in(n);
    for(int i = 0; i < n; ++i) {
        for(auto to : g[i]) {
            ++in[to];
        }
    }
    std::vector<int> S;
    for(int i = 0; i < n; ++i) {
        if(in[i] == 0) {
            S.push_back(i);
        }
    }
    std::vector<int> res;
    while(!S.empty()) {
        int v = S.back();
        S.pop_back();
        res.push_back(v);
        for(auto to : g[v]) {
            --in[to];
            if(in[to] == 0) {
                S.push_back(to);
            }
        }
    }
    bool ok = true;
    for(int i = 0; i < n; ++i) {
        ok &= in[i] == 0;
    }
    if(!ok) {
        res.clear();
    }
    return res;
}
