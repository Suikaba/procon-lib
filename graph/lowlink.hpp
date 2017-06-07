
struct edge {
    int from, to;
};

using edges = std::vector<edge>;
using graph = std::vector<edges>;

class lowlink {
public:
    lowlink(graph const& g, int root)
        : ord(g.size()),
          low(g.size())
    {
        const int N = g.size();
        std::vector<bool> visited(N);
        int cnt = 0;
        for(int i=0; i<N; ++i) {
            if(!visited[i]) {
                dfs(i, -1, cnt, g, visited);
            }
        }
    }

    std::vector<int> get_articulation_points() const {
        return articulation_points;
    }

    std::vector<edge> get_bridges() const {
        return bridges;
    }

private:
    void dfs(int v, int prev, int& cnt, graph const& g, std::vector<bool>& visited) {
        visited[v] = true;
        ord[v] = cnt++;
        low[v] = ord[v];
        bool is_articulation = false;
        int cnt2 = 0;

        for(auto& e : g[v]) {
            if(e.to != prev && visited[e.to]) {
                low[v] = min(low[v], ord[e.to]);
            } else if(!visited[e.to]) {
                cnt2++;
                dfs(e.to, v, cnt, g, visited);
                low[v] = min(low[v], low[e.to]);
                if(prev != -1 && ord[v] <= low[e.to]) {
                    is_articulation = true;
                }
                if(ord[v] < low[e.to]) {
                    bridges.push_back(edge{min(v, e.to), max(v, e.to)});
                }
            }
        }

        if(prev == -1 && cnt2 > 1) {
            is_articulation = true;
        }
        if(is_articulation) {
            articulation_points.push_back(v);
        }
    }

private:
    std::vector<int> articulation_points;
    std::vector<edge> bridges;
    std::vector<int> ord;
    std::vector<int> low;
};

