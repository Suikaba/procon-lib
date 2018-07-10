ld area(polygon const& p) {
    const int N = p.size();
    ld res = 0;
    for(int i=0; i<N; ++i) {
        res += cross(p[i], p[(i+1)%N]);
    }
    return res / 2;
}

// 0 -> on, 1 -> in, 2 -> out
int is_in_polygon(polygon const& poly, point p) {
    const int n = poly.size();
    ld sum = 0;
    for(int i = 0; i < n; ++i) {
        point p1 = poly[i], p2 = poly[(i + 1) % n];
        if(isis_sp(segment(p1, p2), p)) {
            return 0;
        }
        sum += arg((p2 - p) / (p1 - p));
    }
    return std::abs(sum) < pi / 2 ? 2 : 1; // @todo: should check < 0.5 ?
}
