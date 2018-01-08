class convex_hull_trick {
    using T = long long;

    bool check(int f1, int f2, T aa, T bb) {
        return (a[f2] - a[f1]) * (bb - b[f2]) >= (b[f2] - b[f1]) * (aa - a[f2]);
    }
    bool check(int f1, int f2, int f3) {
        return (a[f2] - a[f1]) * (b[f3] - b[f2]) >= (b[f2] - b[f1]) * (a[f3] - a[f2]);
    }

    T f(int idx, T x) {
        return a[idx] * x + b[idx];
    }

public:
    convex_hull_trick() = default;
    // a : sorted sequence, a[i] >= a[i + 1] >= a[i + 2] ...
    convex_hull_trick(std::vector<T> const& a_, std::vector<T> const& b_) {
        assert(a_.size() == b_.size());
        for(int i = 0; i < a_.size(); ++i) {
            add_f(a_[i], b_[i]);
        }
    }

    void add_f(T aa, T bb) {
        while(a.size() >= 2 && check(a.size() - 2, a.size() - 1, aa, bb)) {
            a.pop_back();
            b.pop_back();
        }
        a.push_back(aa);
        b.push_back(bb);
    }
    T min(T x) {
        assert(a.size() > 0);
        while(a.size() >= 2 && f(0, x) >= f(1, x)) {
            a.pop_front();
            b.pop_front();
        }
        return a[0] * x + b[0];
    }

private:
    std::deque<T> a, b; // functions
};
