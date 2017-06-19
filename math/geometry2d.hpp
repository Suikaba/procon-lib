
using ld = long double;

constexpr ld eps = 1e-8;


namespace geometry2d {


using point = std::complex<long double>;
using polygon = std::vector<point>;


bool eq(long double a, long double b) {
    return (std::abs(a-b) < eps);
}

long double dot(point a, point b) {
    return std::real(std::conj(a) * b);
}

long double cross(point a, point b) {
    return std::imag(std::conj(a) * b);
}


class segment {
public:
    segment()
        : a(point(0, 0)),
          b(point(0, 0))
    {}
    segment(point a_, point b_)
        : a(a_), b(b_)
    {}

    point a, b;
};

class line {
public:
    line()
        : a(point(0, 0)), b(point(0, 0))
    {}
    line(point a_, point b_)
        : a(a_), b(b_)
    {}
    line(segment s)
        : a(s.a), b(s.b)
    {}

    point a, b;
};

class circle {
public:
    circle()
        : p(point(0, 0)), r(0)
    {}
    circle(point p_, long double r_)
        : p(p_), r(r_)
    {}

    point p;
    long double r;
};


int ccw(point a, point b, point c) {
    b -= a; c -= a;
    if(cross(b, c) > eps)           return 1;  // a -> b -> c : counterclockwise
    if(cross(b, c) < -eps)          return -1; // a -> b -> c : clockwise
    if(dot(b, c) < 0)               return 2;  // c -> a -> b : line
    if(std::norm(b) < std::norm(c)) return -2; // a -> b -> c : line
    return 0;                                  // a -> c -> b : line
}


bool isis_ll(line l, line m) {
    return !eq(cross(l.b - l.a, m.b - m.a), 0);
}

bool isis_ls(line l, segment s) {
    return (cross(l.b - l.a, s.a - l.a) * cross(l.b - l.a, s.b - s.a) < eps);
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


point proj(line l, point p) {
    long double t = dot(p - l.a, l.a - l.b) / std::norm(l.a - l.b);
    return l.a + t * (l.a - l.b);
}

point is_ll(line s, line t) {
    point sv = s.b - s.a, tv = t.b - t.a;
    assert(cross(sv, tv) != 0);
    return s.a + sv * cross(tv, t.a - s.a) / cross(tv, sv);
}

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


std::vector<point> is_cc(circle const& c1, circle const& c2) {
    std::vector<point> res;
    long double d = std::abs(c1.p - c2.p);
    long double rc = (d * d + c1.r * c1.r - c2.r * c2.r) / (2 * d);
    long double dfr = c1.r * c1.r - rc * rc;
    if(std::abs(dfr) < eps) {
        dfr = 0.0;
    } else if(dfr < 0.0) {
        return res;
    }

    long double rs = std::sqrt(dfr);
    point diff = (c2.p - c1.p) / d;
    res.push_back(c1.p + diff * point(rc, rs));
    if(dfr != 0.0) {
        res.push_back(c1.p + diff * point(rc, -rs));
    }
    return res;
}

// 0 -> on, 1 -> in, 2 -> out
int is_in_polygon(polygon const& poly, point p) {
    int N = poly.size();
    ld sum = 0;
    for(int i=0; i<N; ++i) {
        point p1 = poly[i], p2 = poly[(i+1)%N];
        if(isis_sp(segment(p1, p2), p)) {
            return 0;
        }
        sum += arg((p2 - p) / (p1 - p));
    }
    return eq(sum, 0) ? 2 : 1;
}

point reflection(line const& l, point p) {
    auto pro = proj(l, p);
    return p + (pro - p) + (pro - p);
}

ld area(polygon const& p) {
    const int N = p.size();
    ld res = 0;
    for(int i=0; i<N; ++i) {
        res += cross(p[i], p[(i+1)%N]);
    }
    return res / 2;
}

// left side
polygon convex_cut(polygon const& p, line l) {
    const int N = p.size();
    polygon res;
    for(int i=0; i<N; ++i) {
        auto a = p[i], b = p[(i+1)%N];
        if(ccw(l.a, l.b, a) != -1) {
            res.push_back(a);
        }
        if(ccw(l.a, l.b, a) * ccw(l.a, l.b, b) < 0) {
            res.push_back(is_ll(line(a, b), l));
        }
    }
    return res;
}

line separate(point const& p1, point const& p2) {
    assert(p1 != p2);
    auto m = (p1 + p2) * point(0.5, 0);
    line res;
    res.a = m + (m - p1) * point(0, 1);
    res.b = m + (m - p1) * point(0, -1);
    return res;
}


} // namespace geometry2d

using namespace geometry2d;
