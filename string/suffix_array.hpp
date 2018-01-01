class suffix_array {
public:
    suffix_array(std::string const& t)
        : len(t.length()), s(t), sa(len + 1), rank(len + 1)
    {
        for(int i = 0; i <= len; ++i) {
            sa[i] = i;
            rank[i] = i < len ? s[i] : -1;
        }
        int k = 0;
        auto compare_sa = [this, &k](int i, int j) {
            if(rank[i] != rank[j]) {
                return rank[i] < rank[j];
            } else {
                int ri = i + k <= len ? rank[i + k] : -1;
                int rj = j + k <= len ? rank[j + k] : -1;
                return ri < rj;
            }
        };
        for(k = 1; k <= len; k *= 2) {
            std::sort(std::begin(sa), std::end(sa), compare_sa);
            std::vector<int> tmp(len + 1);
            tmp[sa[0]] = 0;
            for(int i = 1; i <= len; ++i) {
                tmp[sa[i]] = tmp[sa[i - 1]] + compare_sa(sa[i - 1], sa[i]);
            }
            for(int i = 0; i <= len; ++i) {
                rank[i] = tmp[i];
            }
        }
    }

    int operator[](std::size_t i) const {
        assert(0 <= i && i <= len);
        return sa[i];
    }

    std::string get_string() const {
        return s;
    }

    // r: index of sa if found
    // -1: not found
    int contain(std::string const& t) {
        int l = 0, r = len;
        while(r - l > 1) {
            int m = (l + r) / 2;
            if(s.compare(sa[m], t.length(), t) < 0) {
                l = m;
            } else {
                r = m;
            }
        }
        if(s.compare(sa[r], t.length(), t) == 0) {
            return r;
        } else {
            return -1;
        }
    }

private:
    int len;
    std::string s;
    std::vector<int> sa, rank;
};

std::vector<int> construct_lcp(suffix_array const& sa) {
    auto s = sa.get_string();
    int const n = s.size();
    std::vector<int> rank(n + 1);
    for(int i = 0; i <= n; ++i) {
        rank[sa[i]] = i;
    }
    int h = 0;
    std::vector<int> lcp(n + 1);
    for(int i = 0; i < n; ++i) {
        int j = sa[rank[i] - 1];
        if(h > 0) {
            h--;
        }
        for(; j + h < n && i + h < n; ++h) {
            if(s[j + h] != s[i + h]) {
                break;
            }
        }
        lcp[rank[i] - 1] = h;
    }
    return lcp;
}
