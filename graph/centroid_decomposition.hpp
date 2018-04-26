void centroid_decomposition(std::vector<std::vector<int>> const& g) {
    int res = 0;
    const int n = g.size();
    std::vector<int> sz(n);
    std::vector<bool> used(n);
    std::function<void(int, int)> calc_sz = [&](int v, int p) {
        sz[v] = 1;
        for(auto to : g[v]) {
            if(to == p || used[to]) continue;
            calc_sz(to, v);
            sz[v] += sz[to];
        }
    };
    std::function<int(int, int, int)> find_centroid_subtree = [&](int v, int p, const int n) {
        for(auto to : g[v]) {
            if(to == p || used[to]) continue;
            if(n / 2 < sz[to]) {
                return find_centroid_subtree(to, v, n);
            }
        }
        return v;
    };
    std::function<void(int)> dfs = [&](int v) {
        calc_sz(v, -1);
        const int c = find_centroid_subtree(v, -1, sz[v]);
        used[c] = true;

        //<here: compute each subtree alone (without the centroid)
        //>

        for(auto to : g[c]) {
            if(used[to]) continue;
            dfs(to);
        }

        //< here: compute each subtree with centroid
        //>

        used[c] = false;
    };
    dfs(0);
}
