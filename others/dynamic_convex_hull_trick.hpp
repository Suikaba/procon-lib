//template <typename T> constexpr T inf;
//template <> constexpr int inf<int> = 1e9;
//template <> constexpr ll inf<ll> = 1e18;
//template <> constexpr double inf<double> = 1e18;

template <typename T>
class dynamic_convex_hull_trick {
    // ax + b
    struct line {
        T a, b;
        T val;
        double x;
        bool is_query;
        line(T a_, T b_)
        : a(a_), b(b_), x(-inf<double>), is_query(false)
        {}

        T eval(T x) const {
            return a * x + b;
        }
        bool operator<(line const& rhs) const {
            if(rhs.is_query) return x < rhs.val;
            else             return a < rhs.a;
        }
    };
    using iter = typename std::set<line>::iterator;

    bool parallel(line const& l1, line const& l2) {
        return l1.a == l2.a;
    }
    double intersect(line const& l1, line const& l2) {
        return parallel(l1, l2) ? inf<double> : 1.0 * (l2.b - l1.b) / (l1.a - l2.a);
    }

    bool has_prev(iter it) {
        return it != std::begin(lines);
    }
    bool has_next(iter it) {
        return it != std::end(lines) && std::next(it) != std::end(lines);
    }

    bool bad(line const& l1, line const& l2, line const& l3) {
        return intersect(l1, l3) <= intersect(l1, l2);
    }
    bool bad(iter it) {
        return has_prev(it) && has_next(it) && bad(*std::prev(it), *it, *std::next(it));
    }

    iter update(iter it) {
        if(!has_prev(it)) return it;
        line tmp = *it;
        tmp.x = intersect(*it, *std::prev(it));
        it = lines.erase(it);
        return lines.insert(it, tmp);
    }

public:
    // add line: ax + b
    void add_f(T a, T b) {
        line l(a, b);
        auto it = lines.lower_bound(l);
        if(it != lines.end() && parallel(l, *it)) {
            if(it->b < b) it = lines.erase(it);
            else          return;
        }
        it = lines.insert(it, l);
        if(bad(it)) {
            lines.erase(it);
            return;
        }
        while(has_prev(it) && bad(std::prev(it))) lines.erase(std::prev(it));
        while(has_next(it) && bad(std::next(it))) lines.erase(std::next(it));

        it = update(it);
        if(has_prev(it)) update(std::prev(it));
        if(has_next(it)) update(std::next(it));
    }

    // maximum query
    T max(T x) const {
        if(lines.empty()) return -inf<T>;
        line q(0, 0);
        q.val = x, q.is_query = true;
        auto it = --lines.lower_bound(q);
        return it->eval(x);
    }

private:
    std::set<line> lines;
};