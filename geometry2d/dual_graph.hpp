struct edge {
    int from, to;
};

using edges = std::vector<edge>;
using graph = std::vector<edges>;

struct dual_graph_data {
    graph g; // vertex id is polygon index
    std::vector<std::vector<int>> polys; // point ps[poly[i][j]]
};
// ss: segments, ps: segment endpoints
// [required] any two segments must not cross
dual_graph_data dual_graph(std::vector<segment> const& ss, std::vector<point> const& ps) {
    const int n = ps.size();
    std::vector<std::vector<int>> poly;
    std::vector<std::vector<std::tuple<ld, int, bool>>> tup(n);
    std::vector<std::vector<std::vector<int>>> seg2p(n, std::vector<std::vector<int>>(n));
    for(auto& s : ss) {
        int a = -1, b = -1;
        for(int i = 0; i < n; ++i) {
            if(std::abs(s.a - ps[i]) < eps) {
                a = i;
            }
            if(std::abs(s.b - ps[i]) < eps) {
                b = i;
            }
        }
        assert(a >= 0 && b >= 0);
        tup[a].emplace_back(std::arg(s.b - s.a), b, false);
        tup[b].emplace_back(std::arg(s.a - s.b), a, false);
    }
    for(int i = 0; i < n; ++i) {
        std::sort(std::begin(tup[i]), std::end(tup[i]));
    }
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < (int)tup[i].size(); ++j) {
            ld angle;
            int pos = j, src = i, dst;
            bool flag;
            std::tie(angle, dst, flag) = tup[i][j];
            if(flag) continue;
            std::vector<int> ps2;
            while(!flag) {
                ps2.push_back(src);
                std::get<2>(tup[src][pos]) = true;
                seg2p[src][dst].push_back(poly.size());
                seg2p[dst][src].push_back(poly.size());
                angle += pi + eps;
                if(angle > pi) angle -= 2 * pi;
                auto it = std::lower_bound(std::begin(tup[dst]), std::end(tup[dst]), std::make_tuple(angle, 0, false));
                if(it == tup[dst].end()) it = tup[dst].begin();
                src = dst;
                std::tie(angle, dst, flag) = *it;
                pos = it - tup[src].begin();
            }
            poly.push_back(ps2);
        }
    }

    graph g(poly.size());
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < i; ++j) {
            if(seg2p[i][j].size() == 2u) {
                int src = seg2p[i][j][0], dst = seg2p[i][j][1];
                g[src].push_back(edge{src, dst});
                g[dst].push_back(edge{dst, src});
            }
        }
    }
    return dual_graph_data {g, poly};
}
