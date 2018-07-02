
using ld = long double;
using point = std::complex<ld>;
using polygon = std::vector<point>;

constexpr ld eps = 1e-10;
constexpr ld pi = std::acos(-1.0);

bool comp(point a, point b) {
    return std::real(a) < std::real(b) || (std::real(a) <= std::real(b) && std::imag(a) < std::imag(b));
}

std::vector<point> unique(std::vector<point> ps) {
    std::sort(std::begin(ps), std::end(ps), comp);
    std::vector<point> res;
    for(auto& p : ps) {
        if(res.empty() || abs(res.back() - p) > eps) {
            res.push_back(p);
        }
    }
    return res;
}

ld dot(point const& a, point const& b) {
    return std::real(std::conj(a) * b);
}
ld cross(point const& a, point const& b) {
    return std::imag(std::conj(a) * b);
}

int ccw(point a, point b, point c) {
    b -= a; c -= a;
    if(cross(b, c) > eps) return 1;            // a -> b -> c : counterclockwise
    if(cross(b, c) < -eps) return -1;          // a -> b -> c : clockwise
    if(dot(b, c) < 0) return 2;                // c -> a -> b : line
    if(std::norm(b) < std::norm(c)) return -2; // a -> b -> c : line
    return 0;                                  // a -> c -> b : line
}

struct segment {
    segment() : a(0, 0), b(0, 0) {}
    segment(point a, point b) : a(a), b(b) {}
    point a, b;
};

struct line {
    line() : a(0, 0), b(0, 0) {}
    line(point a, point b) : a(a), b(b) {}
    line(segment s) : a(s.a), b(s.b) {}
    point a, b;
};
