polygon convex_hull(std::vector<point> ps) {
    std::sort(ps.begin(), ps.end(), comp);
    const int n = ps.size();
    int k = 0;
    polygon qs(2 * n);
    for(int i = 0; i < n; ++i) {
        while(k > 1 && ccw(qs[k - 2], qs[k - 1], ps[i]) <= 0) {
            k--;
        }
        qs[k++] = ps[i];
    }
    for(int i = n - 2, t = k; i >= 0; --i) {
        while(k > t && ccw(qs[k - 2], qs[k - 1], ps[i]) <= 0) {
            k--;
        }
        qs[k++] = ps[i];
    }
    qs.resize(k - 1);
    return qs;
}

// left side
polygon convex_cut(polygon const& p, line l) {
    const int N = p.size();
    polygon res;
    for(int i = 0; i < N; ++i) {
        auto a = p[i], b = p[(i + 1) % N];
        if(ccw(l.a, l.b, a) != -1) {
            res.push_back(a);
        }
        if(ccw(l.a, l.b, a) * ccw(l.a, l.b, b) < 0) {
            if(cross(a - b, l.a - l.b) == 0) continue; // cut line が辺に覆いかぶさる
            res.push_back(is_ll(line(a, b), l));
        }
    }
    return res;
}
