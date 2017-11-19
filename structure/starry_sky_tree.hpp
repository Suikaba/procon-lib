
//template <typename T> T inf;
//template <> constexpr int inf<int> = 1e9;
//template <> constexpr ll inf<ll> = 1e18;

template <typename T>
class starry_sky_tree {
public:
    starry_sky_tree(int n_)
        : n(expand(n_)), data(n * 2, 0), lazy(n * 2, 0)
    {}

    void add(int l, int r, T val) {
        l += n;
        r += n;
        int const left = l, right = r;
        while(l != r) {
            if(l & 1) {
                lazy[l] += val;
                data[l++] += val;
            }
            if(r & 1) {
                lazy[--r] += val;
                data[r] += val;
            }
            l /= 2;
            r /= 2;
        }
        l = left, r = right - 1;
        while(l /= 2, r /= 2) {
            data[l] = std::min(data[l * 2], data[l * 2 + 1]) + lazy[l];
            data[r] = std::min(data[r * 2], data[r * 2 + 1]) + lazy[r];
        }
    }

    T min(int l, int r) const {
        if(l == r) {
            return inf<T>;
        }
        l += n;
        r += n;
        T res1 = inf<T>, res2 = inf<T>;
        while(l != r) {
            if(l & 1) {
                res1 = std::min(res1, data[l++]);
            }
            if(r & 1) {
                res2 = std::min(res2, data[--r]);
            }
            l /= 2;
            r /= 2;
            res1 += lazy[l - 1];
            res2 += lazy[r];
        }
        --l;
        while(l /= 2, r /= 2) {
            res1 += lazy[l];
            res2 += lazy[r];
        }
        return std::min(std::min(res1, res2), inf<T>);
    }

private:
    int expand(int n) const {
        assert(n != 0);
        return n == 1 ? n : expand((n + 1) / 2) * 2;
    }

private:
    int const n;
    std::vector<T> data, lazy;
};

