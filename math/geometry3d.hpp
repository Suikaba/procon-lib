#include <bits/stdc++.h>
using namespace std;

using ld = long double;
constexpr ld eps = 1e-8;


namespace geometry3d {

using std::abs; // important
using point = std::array<ld, 3>;


std::ostream& operator<<(std::ostream& os, point const& p) {
    os << '(' << p[0] << ", " << p[1] << ", " << p[2] << ')';
    return os;
}


class segment {
public:
    segment(point p1, point p2)
        : a(p1), b(p2)
    {}

    point a, b;
};
class line {
public:
    line(point p1, point p2)
        : a(p1), b(p2)
    {}
    line(segment s)
        : a(s.a), b(s.b)
    {}

    point a, b;
};

class sphere {
public:
    sphere()
        : p({0, 0, 0}), r(0)
    {}
    sphere(point p_, ld r_)
        : p(p_), r(r_)
    {}
    point p;
    ld r;
};


point operator-(point const& a, point const& b) {
    point ret;
    for(int i=0; i<3; ++i) {
        ret[i] = a[i] - b[i];
    }
    return ret;
}

point operator+(point const& a, point const& b) {
    point ret;
    for(int i=0; i<3; ++i) {
        ret[i] = a[i] + b[i];
    }
    return ret;
}

point operator*(point const& a, ld b) {
    point ret = a;
    for(int i=0; i<3; ++i) {
        ret[i] *= b;
    }
    return ret;
}
point operator*(ld a, point const& b) {
    return b*a;
}

bool eq(ld a, ld b) {
    return (std::abs(a - b) < eps);
}

point make_point(ld a, ld b, ld c) {
    return point{{a, b, c}};
}

ld dot(point a, point b) {
    return std::inner_product(std::begin(a), std::end(a), std::begin(b), 0.0);    
}

point cross(point a, point b) {
    return make_point(a[1]*b[2] - a[2]*b[1], a[2]*b[0] - a[0]*b[2], a[0]*b[1] - a[1]*b[0]);
}

ld norm(point a) { // |a|^2
    return dot(a, a);
}

ld abs(point a) { // |a|
    return std::sqrt(dot(a, a));
}


// verified
point proj(line l, point p) {
    ld t = dot(p - l.a, l.a - l.b) / norm(l.a - l.b);
    return l.a + t*(l.a - l.b);
}


bool isis_sps(sphere const& sp, segment const& s) {
    auto a = s.a - sp.p;
    auto b = s.b - sp.p;
    auto p = proj(line(s), sp.p) - sp.p;
    if(abs(a) + eps < sp.r && abs(b) + eps < sp.r && abs(p) + eps < sp.r) { // contain
        return false;
    }
    if(abs(p) > sp.r + eps) {
        return false;
    }
    if(abs(a) + eps < sp.r || abs(b) + eps < sp.r) {
        return true;
    }
    if(abs(abs(a - p) + abs(b - p) - abs(b - a)) < eps) {
        return true;
    }
    return false;
}

} // namespace geometry3d

using namespace geometry3d;

