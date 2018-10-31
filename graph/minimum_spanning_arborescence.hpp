using weight = int;

struct edge {
    int from, to;
    weight cost;
};

using edges = std::vector<edge>;
using graph = std::vector<edges>;

class minimum_spanning_arborescence {
    using node_t = std::pair<weight, int>;
public:
    minimum_spanning_arborescence(int V, edges const& es) {
        heaps.reserve(V);
        for(int i = 0; i < V; ++i) {
            heaps.emplace_back([] (node_t a, int v) { return node_t{a.first + v, a.second}; });
        }
        for(auto const& e : es) {
            heaps[e.to].push(node_t{e.cost, e.from});
        }
    }

    weight solve(int r) {
        const int n = heaps.size();
        weight res = 0;
        union_find uf(n);              // 縮約状況を表す
        std::vector<int> used(n, -1);  // -1: まだ見てない, otherwise: 探索の始点の頂点番号
        used[r] = r; // 根は最初に確定させてしまう
        for(int s = 0; s < n; ++s) {
            std::vector<int> path; // 処理中の頂点集合をもつ
            for(int u = s; used[u] == -1;) {
                path.push_back(u);
                used[u] = s;

                if(heaps[u].empty()) return -1; // そもそも最小有向全域木が存在しない

                auto p = heaps[u].top();
                heaps[u].pop(); // 使った辺は削除

                // 本来、ここで自己辺への対処をする必要がある。
                // しかし、自己辺のコストを足した後、周辺の辺のコストを以下のように
                // 同じ分減らすので、実は結果に影響しない。
                // そして自己辺はすぐに閉路と判定され、その辺が取り除かれるため、
                // 事実上これで自己辺に対応したことになっている。
                // コードの見た目としてはかなり不自然なので注意
                res += p.first;
                heaps[u].add(-p.first);

                int v = uf.root(p.second);
                if(used[v] == s) { // 閉路を発見
                    int w = -1;
                    lazy_skew_heap<node_t, int> nheap([] (node_t a, int lv) { return node_t{a.first + lv, a.second}; });
                    do { // 閉路をまわる
                        w = path.back();
                        path.pop_back();
                        nheap.meld(heaps[w]);
                    } while(uf.unite(v, w));
                    heaps[uf.root(v)] = std::move(nheap);
                    used[uf.root(v)] = -1; // 次のループでもう一度探索させるため
                }
                u = uf.root(v);
            }
        }

        return res;
    }

private:
    std::vector<lazy_skew_heap<node_t, int>> heaps;
};
