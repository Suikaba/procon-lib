// isis = is intersected ...

namespace geometry3d {

using point = std::array<long double, 3>;

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
    sphere(point p_, long double r_)
        : p(p_), r(r_)
    {}
    point p;
    long double r;
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

point operator*(point const& a, long double b) {
    point ret = a;
    for(int i=0; i<3; ++i) {
        ret[i] *= b;
    }
    return ret;
}
point operator*(long double a, point const& b) {
    return b*a;
}

bool eq(long double a, long double b) {
    return (std::abs(a - b) < eps);
}

point make_point(long double a, long double b, long double c) {
    return point{{a, b, c}};
}

long double dot(point a, point b) {
    return std::inner_product(std::begin(a), std::end(a), std::begin(b), 0);    
}

point cross(point a, point b) {
    return make_point(a[1]*b[2] - a[2]*b[1], a[2]*b[0] - a[0]*b[2], a[0]*b[1] - a[1]*b[0]);
}

long double norm(point a) { // |a|^2
    return dot(a, a);
}

long double dist(point a) { // |a|
    return std::sqrt(dot(a, a));
}


// verified
point proj(line l, point p) {
    point p1, p2, ret; // p1 = p - l.a, p2 = l.a - l.b, ret = l.a + t*(l.a - l.b)
    for(int i=0; i<3; ++i) {
        p1[i] = p[i] - l.a[i];
        p2[i] = l.a[i] - l.b[i];
    }
    long double t = dot(p1, p2) / norm(p2);
    for(int i=0; i<3; ++i) {
        ret[i] = l.a[i] + t*p2[i];
    }
    return ret;
}


} // namespace geometry3d

using namespace geometry3d;

