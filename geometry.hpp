// for C++11
#include <complex>
#include <array>
#include <cmath>
#include <algorithm>

// todo
// separate 2d and 3d

// isis = is intersected ...

// Types
using ld = long double;

template <std::size_t N>
using point = std::array<ld, N>;
using point2d = point<2>;
using point3d = point<3>;

template <typename Point = point2d>
class line {
public:
    line(Point p1, Point p2)
        : a(p1), b(p2)
    {}
    Point a, b;
};

class circle {
public:
    circle(point2d p_, ld r_)
        : p(p_), r(r_)
    {}
    point2d p;
    ld r;
};

class sphere {
public:
    sphere(point3d p_, ld r_)
        : p(p_), r(r_)
    {}
    point3d p;
    ld r;
};


// operator
template <std::size_t N>
point<N> operator-(point<N> const& a, point<N> const& b) {
    point<N> ret;
    for(int i=0; i<N; ++i) {
        ret[i] = a[i] - b[i];
    }
    return ret;
}

template <std::size_t N>
point<N> operator+(point<N> const& a, point<N> const& b) {
    point<N> ret;
    for(int i=0; i<N; ++i) {
        ret[i] = a[i] + b[i];
    }
    return ret;
}

template <std::size_t N>
point<N> operator*(point<N> const& a, ld b) {
    point<N> ret = a;
    for(int i=0; i<N; ++i) {
        ret[i] *= b;
    }
    return ret;
}
template <std::size_t N>
point<N> operator*(ld a, point<N> const& b) {
    return b*a;
}


// constant
constexpr ld EPS = 1e-8;
constexpr ld PI = std::acos(-1.0);


// functions
bool eq(ld a, ld b) { // todo: rename
    return (std::abs(a - b) < EPS);
}

point2d make_point(ld a, ld b) {
    return point2d{{a, b}};
}
point3d make_point(ld a, ld b, ld c) {
    return point3d{{a, b, c}};
}

template <std::size_t N>
ld dot(point<N> a, point<N> b) { // inner_product
    return std::inner_product(std::begin(a), std::end(a), std::begin(b), 0);    
}

ld cross(point2d a, point2d b) {
    return a[0]*b[1] - a[1]*b[0];
}
point3d cross(point3d a, point3d b) {
    return make_point(a[1]*b[2] - a[2]*b[1], a[2]*b[0] - a[0]*b[2], a[0]*b[1] - a[1]*b[0]);
}

template <std::size_t N>
ld norm(point<N> a) { // |a|^2
    return dot(a, a);
}

template <std::size_t N>
ld dist(point<N> a) { // |a|
    return std::sqrt(dot(a, a));
}

bool isis_ll(line<> l, line<> m) { // line and line(2d)
    return !eq(cross(l.b - l.a, m.b - m.a), 0);
}

bool isis_ls(line<> l, line<> s) { // line and half line(2d)
    return (cross(l.b - l.a, s.a - l.a) * cross(l.b - l.a, s.b - s.a) < EPS);
}

bool isis_lp(line<> l, point2d p) { // 2d
    return (std::abs(cross(l.b - p, l.a - p)) < EPS);
}

template <std::size_t N>
bool isis_sp(line<point<N>> s, point<N> p) {
    return (dist(s.a - p) + dist(s.b - p) - dist(s.b - s.a) < EPS);
}

// verified
template <std::size_t N>
point<N> proj(line<point<N>> l, point<N> p) {
    point<N> p1, p2, ret; // p1 = p - l.a, p2 = l.a - l.b, ret = l.a + t*(l.a - l.b)
    for(int i=0; i<N; ++i) {
        p1[i] = p[i] - l.a[i];
        p2[i] = l.a[i] - l.b[i];
    }
    ld t = dot(p1, p2) / norm(p2);
    for(int i=0; i<N; ++i) {
        ret[i] = l.a[i] + t*p2[i];
    }
    return ret;
}

template <std::size_t N>
ld dist_lp(line<point<N>> l, point<N> p) {
    return dist(p - proj(l, p));
}

ld dist_ll(line<> l, line<> m) {
    return isis_ll(l, m) ? 0 : dist_lp(l, m.a);
}

ld dist_ls(line<> l, line<> s) {
    return isis_ls(l, s) ? 0 : min(dist_lp(l, s.a), dist_lp(l, s.b));
}

template <std::size_t N>
ld dist_sp(line<point<N>> s, point<N> p) {
    point<N> r = proj(s, p);
    return isis_sp(s, r) ? dist(r - p) : min(dist(s.a - p), dist(s.b - p));
}

int ccw(point2d a, point2d b, point2d c) {
    for(int i=0; i<2; ++i) {
        b[i] -= a[i];
        c[i] -= a[i];
    }
    if(cross(b, c) > EPS) return 1;   // a -> b -> c is counter clockwise
    if(cross(b, c) < -EPS) return -1; // a -> b -> c is clockwise
    if(dot(b, c) < 0) return 2;       // c -> a -> b is line
    if(norm(b) < norm(c)) return -2;  // a -> b -> c is line
    return 0;                         // a -> c -> b is line
}





