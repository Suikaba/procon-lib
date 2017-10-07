
template<typename Monoid>
class segment_tree2d {
    using T = typename Monoid::type;

public:
    segment_tree2d(std::vector<std::vector<T>> const& init)
        : H(expand(init.size())),
          W(expand(init[0].size()))
    {
        int const H2 = H * 2, W2 = W * 2;
        dat.assign(H2 * W2, Monoid::id());
        for(int i = 0; i < (int)init.size(); ++i) {
            for(int j = 0; j < (int)init[0].size(); ++j) {
                dat[(i + H) * W2 + j + W] = init[i][j];
            }
        }
        for(int i = H2 - 1; i >= H; --i) {
            for(int j = W - 1; j > 0; --j) {
                dat[i * W2 + j] = Monoid::op(dat[i * W2 + 2 * j], dat[i * W2 + 2 * j + 1]);
            }
        }
        for(int i = H - 1; i > 0; --i) {
            for(int j = 1; j < W2; ++j) {
                dat[i * W2 + j] = Monoid::op(dat[2 * i * W2 + j], dat[(2 * i + 1) * W2 + j]);
            }
        }
    }

    segment_tree2d(int const H, int const W, T const& init = Monoid::id())
        : segment_tree2d(std::vector<std::vector<T>>(H, std::vector<T>(W, init)))
    {}

    void update(int i, int j, T val) {
        i += H;
        j += W;
        int tj = j;
        while(tj >>= 1) {
            dat[i * W * 2 + tj] = Monoid::op(dat[i * W * 2 + tj * 2], dat[i * W * 2 + tj * 2 + 1]);
        }
        while(i >>= 1) {
            dat[i * W * 2 + j] = Monoid::op(dat[i * 2 * W * 2 + j], dat[(i * 2 + 1) * W * 2 + j]);
            tj = j;
            while(tj >>= 1) {
                dat[i * W * 2 + tj] = Monoid::op(dat[i * W * 2 + tj * 2], dat[i * W * 2 + tj * 2 + 1]);
            }
        }
    }

    // [li, ri) * [lj, rj)
    T query(int li, int lj, int ri, int rj) const {
        return query_h(li, lj, ri, rj, 0, H, 1);
    }
    T query_h(int li, int lj, int ri, int rj, int si, int ti, int k) const {
        if(ri <= si || ti <= li) {
            return Monoid::id();
        }
        if(li <= si && ti <= ri) {
            return query_w(lj, rj, 0, W, k, 1);
        }
        int const mi = (si + ti) / 2;
        return Monoid::op(query_h(li, lj, ri, rj, si, mi, 2 * k), query_h(li, lj, ri, rj, mi, ti, 2 * k + 1));
    }
    T query_w(int lj, int rj, int sj, int tj, int i, int k) const {
        if(rj <= sj || tj <= lj) {
            return Monoid::id();
        }
        if(lj <= sj && tj <= rj) {
            return dat[i * (W * 2) + k];
        }
        int const mj = (sj + tj) / 2;
        return Monoid::op(query_w(lj, rj, sj, mj, i, 2 * k), query_w(lj, rj, mj, tj, i, 2 * k + 1));
    }

private:
    int expand(int n) const {
        assert(n >= 1);
        return n == 1 ? n : expand((n + 1) / 2) * 2;
    }

private:
    int const H, W;
    std::vector<T> dat;
};

struct rmq {
    using type = int;
    static type id() {
        return std::numeric_limits<type>::max();
    }
    static type op(type const& l, type const& r) {
        return std::min(l, r);
    }
};

