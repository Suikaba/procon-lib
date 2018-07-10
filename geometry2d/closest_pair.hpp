bool compare_y(point const& a, point const& b) {
    return std::imag(a) < std::imag(b);
}

ld closest_pair_impl(point* a, int n) {
    if (n <= 1) return 1e20;
    int m = n / 2;
    ld x = std::real(a[m]);
    ld d = std::min(closest_pair_impl(a, m), closest_pair_impl(a + m, n - m));
    std::inplace_merge(a, a + m, a + n, compare_y);
    std::vector<point> b;
    for (int i = 0; i < n; i++) {
        if (std::abs(std::real(a[i]) - x) >= d) continue;
        for (int j = 0; j < (int)b.size(); j++) {
            ld dx = std::real(a[i]) - std::real(b[b.size() - j - 1]);
            ld dy = std::imag(a[i]) - std::imag(b[b.size() - j - 1]);
            if (dy >= d) break;
            d = std::min(d, sqrtl(dx * dx + dy * dy));
        }
        b.push_back(a[i]);
    }
    return d;
}
ld closest_pair(std::vector<point> ps) {
    std::sort(std::begin(ps), std::end(ps), comp);
    return closest_pair_impl(ps.data(), ps.size());
}
