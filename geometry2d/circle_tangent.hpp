std::vector<line> tangent_cp(circle c, point p) {
    std::vector<line> res;
    point v = c.p - p;
    ld d = std::abs(v), len = std::sqrt(std::norm(v) - c.r * c.r);
    if(std::isnan(len)) {
        return res;
    }
    point v1 = v * point(len / d, c.r / d);
    point v2 = v * point(len / d, -c.r / d);
    res.push_back(line(p, p + v1));
    if(len < eps) {
        return res;
    }
    res.push_back(line(p, p + v2));
    return res;
}

std::vector<line> tangent_cc(circle c1, circle c2) {
    std::vector<line> res;
    auto average = [] (point const& p1, point const& p2, ld ratio1, ld ratio2) {
        return (p1 * ratio1 + p2 * ratio2) / (ratio1 + ratio2);
    };
    if(std::abs(c1.p - c2.p) - (c1.r + c2.r) >= 0) {
        auto center = average(c1.p, c2.p, c2.r, c1.r);
        res = tangent_cp(c1, center);
    }
    if(c1.r - c2.r != 0) {
        const point out = average(c1.p, c2.p, -c2.r, c1.r);
        const std::vector<line> tmp = tangent_cp(c1, out);
        res.insert(std::end(res), std::begin(tmp), std::end(tmp));
    } else {
        point vec = (c2.p - c1.p) / std::abs(c2.p - c1.p);
        point q1 = c1.p + vec * point(0, 1) * c1.r;
        point q2 = c1.p + vec * point(0, -1) * c1.r;
        res.emplace_back(q1, q1 + vec);
        res.emplace_back(q2, q2 + vec);
    }
    return res;
}
