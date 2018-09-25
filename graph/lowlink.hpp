class lowlink { // multiple edges ok
public:
    lowlink(graph const& g)
        : ord(g.size()), low(g.size())
    {
        const int n = g.size();
        std::vector<bool> visited(n);
        int cnt = 0;
        for(int i = 0; i < n; ++i) {
            if(!visited[i]) {
                dfs(g, i, -1, cnt, visited);
            }
        }
    }

    std::vector<int> get_articulation_points() const { return articulation_points; }
    std::vector<edge> get_bridges() const            { return bridges; }

    bool is_bridge(int u, int v) const {
        if(ord[u] > ord[v]) std::swap(u, v);
        return ord[u] < low[v];
    }

private:
    void dfs(graph const& g, int v, int prev, int& cnt, std::vector<bool>& visited) {
        visited[v] = true;
        low[v] = ord[v] = cnt++;
        bool is_articulation = false;
        int cnt2 = 0, pcnt = 0;

        for(auto& e : g[v]) {
            if((e.to != prev || (e.to == prev && pcnt == 1)) && visited[e.to]) {
                low[v] = min(low[v], ord[e.to]);
            } else if(!visited[e.to]) {
                cnt2++;
                dfs(g, e.to, v, cnt, visited);
                low[v] = min(low[v], low[e.to]);
                if(prev != -1 && ord[v] <= low[e.to]) {
                    is_articulation = true;
                }
                if(is_bridge(v, e.to)) {
                    bridges.push_back(edge{min(v, e.to), max(v, e.to)});
                }
            }
            pcnt += e.to == prev;
        }
        is_articulation |= (prev == -1 && cnt2 > 1);
        if(is_articulation) articulation_points.push_back(v);
    }

private:
    std::vector<int> articulation_points;
    std::vector<edge> bridges;
    std::vector<int> ord, low;
};
