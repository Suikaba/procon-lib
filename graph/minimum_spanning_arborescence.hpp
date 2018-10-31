using weight = int;

struct edge {
    int from, to;
    weight cost;
};

using edges = std::vector<edge>;
using graph = std::vector<edges>;

class minimum_spanning_arborescence {
public:
    minimum_spanning_arborescence(int V, edges const& es_) : es(V) {
        for(auto const& e : es_) {
            es[e.to].push_back(e);
        }
    }

    weight solve(int r) {
        const int n = es.size();
        weight res = 0;
        union_find uf(n);              // 縮約状況を表す
        std::vector<int> used(n, -1);  // -1: まだ見てない, otherwise: 探索の始点の頂点番号
        used[r] = r; // 根は最初に確定させてしまう
        for(int s = 0; s < n; ++s) {
            std::vector<int> path; // 処理中の頂点集合をもつ
            for(int u = s; used[u] == -1;) {
                path.push_back(u);
                used[u] = s;
                if(es[u].empty()) return -1; // そもそも最小有向全域木が存在しない
                int use_idx = 0;
                for(int i = 0; i < (int)es[u].size(); ++i) {
                    if(es[u][use_idx].cost > es[u][i].cost) {
                        use_idx = i;
                    }
                }
                auto use_e = es[u][use_idx];
                es[u].erase(std::begin(es[u]) + use_idx); // 使った辺は削除

                // 本来、ここで自己辺への対処をする必要がある。
                // しかし、自己辺のコストを足した後、周辺の辺のコストを以下のように
                // 同じ分減らすので、実は結果に影響しない。
                // そして自己辺はすぐに閉路と判定され、その辺が取り除かれるため、
                // 事実上これで自己辺に対応したことになっている。
                // コードの見た目としてはかなり不自然なので注意
                res += use_e.cost;
                for(auto& e : es[u]) {
                    e.cost -= use_e.cost;
                }

                int v = uf.root(use_e.from);
                if(used[v] == s) { // 閉路を発見
                    int w = -1;
                    std::vector<edge> new_es;
                    do { // 閉路をまわる
                        w = path.back();
                        path.pop_back();
                        new_es.insert(std::end(new_es), std::begin(es[w]), std::end(es[w]));
                        es[w].clear();
                    } while(uf.unite(v, w));
                    es[uf.root(v)] = std::move(new_es);
                    used[uf.root(v)] = -1; // 次のループでもう一度探索させるため
                }
                u = uf.root(v);
            }
        }

        return res;
    }

private:
    std::vector<std::vector<edge>> es;
};