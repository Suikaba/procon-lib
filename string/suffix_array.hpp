
class suffix_array {
public:
    suffix_array(std::string const& s_)
        : length(s_.length()),
          s(s_),
          sa(s.length()+1),
          rank(s.length()+1)
    {
        for(int i = 0; i <= length; ++i) {
            sa[i] = i;
            rank[i] = i < length ? s[i] : -1;
        }
        int k = 0;
        auto compare_sa = [this, &k](int i, int j) {
            if(rank[i] != rank[j]) {
                return rank[i] < rank[j];
            } else {
                int ri = i + k <= length ? rank[i + k] : -1;
                int rj = j + k <= length ? rank[j + k] : -1;
                return ri < rj;
            }
        };
        for(k = 1; k <= length; k *= 2) {
            sort(sa.begin(), sa.end(), compare_sa);

            std::vector<int> tmp(length+1);
            tmp[sa[0]] = 0;
            for(int i = 1; i <= length; ++i) {
                tmp[sa[i]] = tmp[sa[i - 1]] + (compare_sa(sa[i - 1], sa[i]) ? 1 : 0);
            }
            for(int i=0; i <= length; ++i) {
                rank[i] = tmp[i];
            }
        }
    }

    std::vector<int> get() const {
        return sa;
    }

    bool contain(std::string const& t) {
        int l = 0, r = length;
        while(r - l > 1) {
            int m = (l + r) / 2;
            if(s.compare(sa[m], t.length(), t) < 0) {
                l = m;
            } else {
                r = m;
            }
        }
        return s.compare(sa[r], t.length(), t) == 0;
    }

    int lower_bound(std::string const& t) {
        int l = 0, r = length + 1;
        while(r - l > 1) {
            int m = (l + r) / 2;
            if(s.compare(sa[m], std::string::npos, t) < 0) {
                l = m;
            } else {
                r = m;
            }
        }
        return r;
    }
    int upper_bound(std::string const& t) {
        int l = 0, r = length + 1;
        while(r - l > 1) {
            int m = (l + r) / 2;
            if(s.compare(sa[m], t.size(), t) <= 0) {
                l = m;
            } else {
                r = m;
            }
        }
        return r;
    }

private:
    int length;
    std::string s;
    std::vector<int> sa;
    std::vector<int> rank;
};

