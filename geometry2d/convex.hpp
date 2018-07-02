
polygon convex_hull(std::vector<point> ps) {
    std::sort(std::begin(ps), std::end(ps),
              [](point const& p1, point const& p2) {
                  return std::real(p1) < std::real(p2)
                                         || (std::real(p1) <= std::real(p2) && std::imag(p1) < std::imag(p2));
              });
    const int n = ps.size();
    int k = 0;
    polygon qs(2 * n);
    for(int i = 0; i < n; ++i) {
        while(k > 1 && ccw(qs[k - 2], qs[k - 1], ps[i]) <= 0) k--;
        qs[k++] = ps[i];
    }
    for(int i = n - 2, t = k; i >= 0; --i) {
        while(k > t && ccw(qs[k - 2], qs[k - 1], ps[i]) <= 0) k--;
        qs[k++] = ps[i];
    }
    qs.resize(k - 1);
    return qs;
}
