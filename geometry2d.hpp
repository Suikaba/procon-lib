
namespace geometry2d {


using point = std::complex<long double>;

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
    segment(point a_, point b_)
        : a(a_), b(b_)
    {}

    point a, b;
};

class line {
public:
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


} // namespace geometry2d

using namespace geometry2d;
