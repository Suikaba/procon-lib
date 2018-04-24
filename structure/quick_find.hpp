class quick_find {
public:
    quick_find(int n) : par(n, -1), cmp(n) {
        for(int i = 0; i < n; ++i) {
            cmp[i].push_back(i);
        }
    }

    int root(int x) {
        return par[x] < 0 ? x : par[x] = root(par[x]);
    }

    bool unite(int x, int y) {
        x = root(x), y = root(y);
        if(x == y) return false;
        if(cmp[x].size() > cmp[y].size()) swap(x, y);
        for(auto v : cmp[y]) {
            cmp[x].push_back(v);
        }
        cmp[y].clear();
        par[y] = x;
        return true;
    }

    std::vector<int> component(int v) {
        return cmp[root(v)];
    }

    bool same(int x, int y) {
        return root(x) == root(y);
    }

    int size(int v) {
        return cmp[root(v)].size();
    }

private:
    std::vector<int> par;
    std::vector<std::vector<int>> cmp;
};