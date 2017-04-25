
template <typename T>
class segment_tree {
    using func_t = std::function<T(T, T)>;

public:
    segment_tree(int n_, T id_, func_t merge_)
        : n(size(n_)),
          id(id_),
          merge(merge_),
          dat(size(n_)*2, id_)
    {}
    segment_tree(vector<T> const& dat_, T id_, func_t merge_)
        : n(size(dat_.size())),
          id(id_),
          merge(merge_),
          dat(size(dat_.size())*2, id_)
    {
        for(int i=0; i<dat_.size(); ++i) {
            dat[i+n-1] = dat_[i];
        }
        for(int i=n-2; i>=0; --i) {
            dat[i] = merge(dat[i*2+1], dat[i*2+2]);
        }
    }

    void update(int k, T val) {
        k += n-1;
        dat[k] = val;
        while(k > 0) {
            k = (k-1)/2;
            dat[k] = merge(dat[k*2+1], dat[k*2+2]);
        }
    }
    void add(int k, T val) {
        k += n-1;
        dat[k] += val;
        while(k > 0) {
            k = (k-1)/2;
            dat[k] = merge(dat[k*2+1], dat[k*2+2]);
        }
    }
    // [l, r)
    T find(int l, int r) {
        return sub(l, r, 0, 0, n);
    }

private:
    int size(int n) {
        int res = 1;
        while(res < n) {
            res *= 2;
        }
        return res;
    }
    T sub(int l, int r, int node, int lb, int ub) {
        if(ub <= l || r <= lb) {
            return id;
        }
        if(l <= lb && ub <= r) {
            return dat[node];
        }
        T vl = sub(l, r, node*2+1, lb, (lb + ub)/2);
        T vr = sub(l, r, node*2+2, (lb+ub)/2, ub);
        return merge(vl, vr);
    }

private:
    const int n;
    const T id;
    func_t merge;
    vector<T> dat;
};
