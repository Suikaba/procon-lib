long double dist_lp(line l, point p) {
    return std::abs(p - proj(l, p));
}

long double dist_ll(line l, line m) {
    return isis_ll(l, m) ? 0 : dist_lp(l, m.a);
}

long double dist_ls(line l, segment s) {
    return isis_ls(l, s) ? 0 : std::min(dist_lp(l, s.a), dist_lp(l, s.b));
}

long double dist_sp(segment s, point p) {
    point r = proj(line(s), p);
    return isis_sp(s, r) ? std::abs(r - p) : std::min(std::abs(s.a - p), std::abs(s.b - p));
}

long double dist_ss(segment s, segment t) {
    if(isis_ss(s, t)) return 0;
    long double d1 = std::min(dist_sp(s, t.a), dist_sp(s, t.b));
    long double d2 = std::min(dist_sp(t, s.a), dist_sp(t, s.b));
    return std::min(d1, d2);
}