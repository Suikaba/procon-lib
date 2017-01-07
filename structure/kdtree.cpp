//#include <memory>
//#include <vector>
//#include <random>
//#include <array>
//#include <limits>
//#include <algorithm>

template<typename T, int Dim = 2>
struct data {
    int index;
    std::array<T, Dim> v; // coordinate
    data() : index(-1), v() {}
};

template<typename T, int Dim = 2>
class axis_sorter {
public:
    // k == dim
    axis_sorter(int _k)
        : k(_k)
    {}
    bool operator()(data<T, Dim> const& a, data<T, Dim> const& b) {
        return a.v[k] < b.v[k];
    }

private:
    int k;
};

template <typename T, int Dim>
T dist(data<T, Dim> const& d1, data<T, Dim> const& d2) {
    T res = 0;
    for(int i=0; i<Dim; ++i) {
        res += (d1.v[i] - d2.v[i]) * (d1.v[i] - d2.v[i]);
    }
    return res;
}

template<typename T, int Dim = 2>
class kdtree {
public:
    kdtree(T const& t)
        : val(t),
          left(nullptr),
          right(nullptr)
    {}
    // [l...r)
    kdtree(std::vector<data<T, Dim>>& ds, int l, int r, int depth_ = 0)
        : axis(depth_ % Dim),
          depth(depth_),
          left(nullptr),
          right(nullptr)
    {
        if(l >= r) {
            return;
        }
        //std::random_device rnd;
        //std::mt19937 mt(rnd());
        //std::uniform_int_distribution<> randlr(l, r-1);
        //const int samp_size = std::min(r-l, 10);
        //std::vector<std::pair<int, data<T, Dim>>> sample(samp_size);
        //for(int i=0; i<samp_size; ++i) {
        //    int idx = randlr(mt);
        //    sample[i] = std::make_pair(idx, ds[idx]);
        //}
        //axis_sorter<T, Dim> sorter(axis);
        //std::sort(sample.begin(), sample.end(), [&sorter](auto const& p1, auto const& p2) {
        //    return sorter(p1.second, p2.second);
        //});
        //int nth = sample[samp_size/2].first;
        int nth = (l + r) / 2;
        std::nth_element(ds.begin() + l, ds.begin()+nth, ds.begin()+r, axis_sorter<T, Dim>(axis));
        val = ds[nth];
        left = make_kdtree(ds, l, nth, depth+1);
        right = make_kdtree(ds, nth+1, r, depth+1);
    }

    std::vector<data<T, Dim>> query(data<T, Dim> const& amin, data<T, Dim> const& amax) {
        std::vector<data<T, Dim>> res;
        bool aok = true;
        for(int i=0; i<Dim; ++i) {
            if(amin.v[i] <= val.v[i] && val.v[i] <= amax.v[i]) {
                continue;
            }
            aok = false;
            break;
        }
        if(aok) {
            res.push_back(val);
        }
        axis_sorter<T, Dim> as(axis);
        if(as(val, amax) || val.v[axis] == amax.v[axis]) {
            if(right) {
                std::vector<data<T, Dim>> tmp(right->query(amin, amax));
                res.insert(res.end(), tmp.begin(), tmp.end());
            }
        }
        if(as(amin, val) || val.v[axis] == amin.v[axis]) {
            if(left) {
                std::vector<T> tmp(left->query(amin, amax));
                res.insert(res.end(), tmp.begin(), tmp.end());
            }
        }
        return res;
    }

    T nearest_neighbor(data<T, Dim> const& p) {
        T res = dist(val, p);
        if(res == 0) { // point(p) equals point(val)
            return std::numeric_limits<T>::max();
        }
        axis_sorter<T, Dim> sorter(axis);
        if(sorter(p, val) || val.v[axis] == p.v[axis]) {
            if(left) {
                res = std::min(res, left->nearest_neighbor(p));
            }
            T dis = p.v[axis] - val.v[axis];
            // todo: eps
            if(dis * dis >= res) {
                return res;
            } else {
                if(right) {
                    res = std::min(res, right->nearest_neighbor(p));
                }
            }
        } else {
            if(right) {
                res = std::min(res, right->nearest_neighbor(p));
            }
            T dis = val.v[axis] - p.v[axis];
            // todo: eps
            if(dis * dis >= res) {
                return res;
            } else {
                if(left) {
                    res = std::min(res, left->nearest_neighbor(p));
                }
            }
        }
        return res;
    }

private:
    data<T, Dim> val;
    int axis;
    int depth;
    std::shared_ptr<kdtree<T, Dim>> left, right;
};

template <typename T, int Dim = 2>
std::shared_ptr<kdtree<T, Dim>> make_kdtree(std::vector<data<T, Dim>>& ps, int l, int r, int depth = 0) {
    if(l >= r) {
        return nullptr;
    } else {
        return std::make_shared<kdtree<T, Dim>>(ps, l, r, depth);
    }
}

