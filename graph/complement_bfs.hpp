
constexpr int INF = 1e9;

using pii = std::pair<int, int>;

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
    while(!que.empty()) {
        int v = que.front().first;
        int d = que.front().second;
        que.pop();
        std::vector<bool> adj(n);
        for(auto to : g[v]) {
            adj[to] = true;
        }

        for(auto it = std::begin(unused); it != std::end(unused); ) {
            if(!adj[*it]) { // 補グラフ上で隣接しているか？
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

