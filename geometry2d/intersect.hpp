bool isis_ll(line l, line m) {
    return cross(l.b - l.a, m.b - m.a) < eps;
}

bool isis_ls(line l, segment s) {
    return (cross(l.b - l.a, s.a - l.a) * cross(l.b - l.a, s.b - l.a) < eps);
}

bool isis_lp(line l, point p) {
    return (std::abs(cross(l.b - p, l.a - p)) < eps);
}

bool isis_sp(segment s, point p) {
    return (std::abs(s.a - p) + std::abs(s.b - p) - std::abs(s.b - s.a) < eps);
}

bool isis_ss(segment s, segment t) {
    return ccw(s.a, s.b, t.a) * ccw(s.a, s.b, t.b) <= 0
        && ccw(t.a, t.b, s.a) * ccw(t.a, t.b, s.b) <= 0;
}

point is_ll(line s, line t) {
    point sv = s.b - s.a, tv = t.b - t.a;
    assert(cross(sv, tv) != 0);
    return s.a + sv * cross(tv, t.a - s.a) / cross(tv, sv);
}

point is_ss(segment s1, segment s2) {
    assert(isis_ss(s1, s2));
    point sv = s1.b - s1.a, tv = s2.b - s2.a;
    if(cross(sv, tv) == 0) {
        if(std::abs(s1.a - s2.a) < eps || std::abs(s1.a - s2.b) < eps) {
            return s1.a;
        } else {
            return s1.b;
        }
    } else {
        return is_ll(line(s1), line(s2));
    }
}

point proj(line l, point p) {
    long double t = dot(p - l.a, l.a - l.b) / std::norm(l.a - l.b);
    return l.a + t * (l.a - l.b);
}
