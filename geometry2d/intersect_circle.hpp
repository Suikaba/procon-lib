std::vector<point> is_cl(circle const& c, line const& l) {
    const ld d = dist_lp(l, c.p);
    std::vector<point> res;
    if(d > c.r) return res;
    auto p = proj(l, c.p);
    if(std::fabs(d - c.r) < eps) {
        res.push_back(p);
        return res;
    }
    const ld len = std::sqrt(c.r * c.r - d * d);
    auto vec = (l.b - l.a) / std::abs(l.b - l.a);
    res.push_back(p + vec * len);
    res.push_back(p + vec * len * point(-1, 0));
    return res;
}

std::vector<point> is_cc(circle const& c1, circle const& c2) {
    std::vector<point> res;
    ld d = std::abs(c1.p - c2.p);
    ld rc = (d * d + c1.r * c1.r - c2.r * c2.r) / (2 * d);
    ld dfr = c1.r * c1.r - rc * rc;
    if(std::abs(dfr) < eps) {
        dfr = 0.0;
    } else if(dfr < 0.0) {
        return res;
    }

    ld rs = std::sqrt(dfr);
    point diff = (c2.p - c1.p) / d;
    res.push_back(c1.p + diff * point(rc, rs));
    if(dfr != 0.0) {
        res.push_back(c1.p + diff * point(rc, -rs));
    }
    return res;
}
