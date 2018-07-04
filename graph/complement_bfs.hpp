// O(E + V)
std::vector<int> complement_bfs(std::vector<std::vector<int>> const& g, int s) {
    const int n = g.size();
    std::vector<int> dist(n, INF);
    dist[s] = 0;
    std::queue<pii> que;
    que.emplace(s, 0);
    std::list<int> unused;
    for(int i = 0; i < n; ++i) {
        if(i != s) {
            unused.push_back(i);
        }
    }

    std::vector<int> adj(n, -1);
    while(!que.empty()) {
        int v = que.front().first;
        int d = que.front().second;
        que.pop();
        for(auto to : g[v]) {
            adj[to] = v;
        }

        for(auto it = std::begin(unused); it != std::end(unused); ) {
            if(adj[*it] != v) { // 補グラフ上で隣接しているか？
                que.emplace(*it, d + 1);
                dist[*it] = d + 1;
                it = unused.erase(it);
            } else {
                ++it;
            }
        }
    }

    return dist;
}
