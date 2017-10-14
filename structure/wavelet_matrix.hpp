#include <bits/stdc++.h>

using u32 = unsigned int;
using u64 = unsigned long long;

int popcount(unsigned long long a) {
    a = (a & 0x5555555555555555) + (a >> 1 & 0x5555555555555555);
    a = (a & 0x3333333333333333) + (a >> 2 & 0x3333333333333333);
    a = (a & 0x0f0f0f0f0f0f0f0f) + (a >> 4 & 0x0f0f0f0f0f0f0f0f);
    a = (a & 0x00ff00ff00ff00ff) + (a >> 8 & 0x00ff00ff00ff00ff);
    a = (a & 0x0000ffff0000ffff) + (a >> 16 & 0x0000ffff0000ffff);
    return (a & 0xffffffff) + (a >> 32);
}

// select : O(logN)
class fully_indexable_dictionary {
public:
    static const int interval = 32;
    static const int not_found = -1;

    fully_indexable_dictionary(int len) : length(len) {
        block_size = (length + interval - 1) / interval + 1;
        blocks.resize(block_size);
    }

    void set(int i) {
        blocks[i / interval] |= 1U << (i & (interval - 1));
    }

    void build() {
        rank_table.assign(block_size + 1, 0);
        for(int i = 1; i < block_size; ++i) {
            rank_table[i] = rank_table[i - 1] + popcount(blocks[i - 1]);
        }
    }

    // return B[pos]
    bool access(int pos) const {
        return blocks[pos / interval] >> (pos & 31) & 1;
    }

    // count 1 on B[0..pos)
    int rank(int pos) const {
        int block_idx = pos / interval;
        u32 mask = (1U << (pos & 31)) - 1;
        return rank_table[block_idx] + popcount(blocks[block_idx] & mask);
    }
    int rank(bool b, int pos) const {
        return (b ? rank(pos) : pos - rank(pos));
    }
    int rank(bool b, int l, int r) const {
        return rank(b, r) - rank(b, l);
    }

    // k + 1 番目の b の位置
    // blocks 上で二分探索してから rank で二分探索したほうが早い
    // 実装が楽なので今は遅いほうだけど，そのうち書き直す．
    int select(bool b, int x) const {
        if(x < 0 || rank(b, length) <= x) {
            return not_found;
        }

        int lb = 0, ub = length;
        while(ub - lb > 1) {
            int m = (lb + ub) / 2;
            if(rank(b, m) <= x) {
                lb = m;
            } else {
                ub = m;
            }
        }

        return lb;
    }
    int select(bool b, int x, int left) const {
        return select(b, x + rank(b, left));
    }

private:
    int length;
    int block_size;
    std::vector<u32> blocks;
    std::vector<int> rank_table;
};


template <typename T>
class wavelet_matrix {
public:
    static const int not_found = -1;
    static const int max_bit_size = 64;

    wavelet_matrix(std::vector<T> const& data) {
        length = data.size();
        max_val = *std::max_element(std::begin(data), std::end(data));
        for(bit_size = 0; 1LL << bit_size <= max_val; ++bit_size);
        dicts.assign(bit_size, length);
        mids.assign(bit_size, 0);
        std::vector<T> buf1(data), buf2(length);
        for(int b = 0; b < bit_size; ++b) {
            int pos = 0;
            for(int i = 0; i < length; ++i) {
                if((buf1[i] >> (bit_size - b - 1) & 1) == 0) {
                    buf2[pos++] = buf1[i];
                }
            }
            mids[b] = pos;
            for(int i = 0; i < length; ++i) {
                if((buf1[i] >> (bit_size - b - 1) & 1) == 1) {
                    dicts[b].set(i);
                    buf2[pos++] = buf1[i];
                }
            }
            dicts[b].build();
            buf1.swap(buf2);
        }
    }

    T access(int pos) const {
        T res = 0;
        for(int b = 0; b < bit_size; ++b) {
            bool d = dicts[b].access(pos);
            res = res << 1 | d;
            pos = dicts[b].rank(d, pos);
            if(d) {
                pos += mids[b];
            }
        }
        return res;
    }

    // [l..r)
    int rank(T val, int l, int r) const {
        if(val > max_val) {
            return 0;
        }

        for(int b = 0; b < bit_size; ++b) {
            bool d = val >> (bit_size - b - 1) & 1;
            l = dicts[b].rank(d, l);
            r = dicts[b].rank(d, r);
            if(d) {
                l += mids[b];
                r += mids[b];
            }
        }
        return r - l;
    }
    // [0..r)
    int rank(T val, int r) const {
        return rank(val, 0, r);
    }

    // ol -> the number of numbers strictly less than val on [l..r)
    // og -> the number of numbers strictly greater than val on [l..r)
    int rank_all(T val, int l, int r, int& ol, int& og) const {
        if(val > max_val) {
            ol = r - l;
            og = 0;
            return 0;
        }
        ol = og = 0;

        for(int b = 0; b < bit_size; ++b) {
            bool d = val >> (bit_size - b - 1) & 1;
            int lcnt = dicts[b].rank(d, l);
            int rcnt = dicts[b].rank(d, r);
            (d ? ol : og) += (r - l) - (rcnt - lcnt);
            l = lcnt;
            r = rcnt;
            if(d) {
                l += mids[b];
                r += mids[b];
            }
        }
        return r - l;
    }

    int rank_lt(T val, int l, int r) const {
        int ol, og;
        rank_all(val, l, r, ol, og);
        return ol;
    }

    // return the number of numbers in [b..u) on [l..r)
    int range_freq(int l, int r, T b, T u) const {
        return rank_lt(u, l, r) - rank_lt(b, l, r);
    }

    // find the (x + 1)th position of val
    int select(T val, int x) const {
        if(val > max_val) {
            return not_found;
        }

        static int lefts[max_bit_size], rights[max_bit_size];
        int l = 0, r = length;
        for(int b = 0; b < bit_size; ++b) {
            lefts[b] = l;
            rights[b] = r;
            bool d = val >> (bit_size - b - 1) & 1;
            l = dicts[b].rank(d, l);
            r = dicts[b].rank(d, r);
            if(d) {
                l += mids[b];
                r += mids[b];
            }
        }

        for(int b = bit_size - 1; b >= 0; --b) {
            x = dicts[b].select(val >> (bit_size - b - 1) & 1, x, lefts[b]);
            if(x == fully_indexable_dictionary::not_found || x >= rights[b]) {
                return not_found;
            }
            x -= lefts[b];
        }
        return x;
    }
    int select(T val, int x, int left) const {
        return select(val, x + rank(val, left));
    }

    // out_v -> the (k + 1)th biggest number on [l..r)
    // out_k -> which one (of numbers which equal out_v)
    void quantile(int l, int r, int kth, int& out_v, int& out_k) const {
        if(r - l <= kth || kth < 0) {
            out_v = -1;
            out_k = not_found;
            return;
        }

        T val = 0;
        for(int b = 0; b < bit_size; ++b) {
            int cnt = dicts[b].rank(1, l, r);
            bool d = kth < cnt;
            val = val << 1 | d;
            if(!d) {
                kth -= cnt;
            }
            l = dicts[b].rank(d, l);
            r = dicts[b].rank(d, r);
            if(d) {
                l += mids[b];
                r += mids[b];
            }
        }
        out_v = val;
        out_k = kth;
    }
    T quantile(int l, int r, int kth) const {
        T tmp_v;
        int tmp_k;
        quantile(l, r, kth, tmp_v, tmp_k);
        return tmp_v;
    }

    struct idx_val {
        int idx;
        T val;
    };
    idx_val quantile_idx_val(int l, int r, int k) const {
        T tmp_v;
        int tmp_k;
        quantile(l, r, k, tmp_v, tmp_k);
        return idx_val{select(tmp_v, tmp_k, l), tmp_v};
    }

    T max(int l, int r) const {
        return quantile(l, r, 0);
    }
    T min(int l, int r) const {
        return quantile(l, r, r - l - 1);
    }

    // O(bit_size * log(length))
    void max_dfs(int b, int l, int r, int& k, T val, std::vector<T>& res) const {
        if(l >= r || k == 0) {
            return;
        }
        if(b == bit_size) {
            while(l++ < r && k > 0) {
                res.push_back(val);
                k--;
            }
            return;
        }
        int ll = dicts[b].rank(1, l);
        int rr = dicts[b].rank(1, r);
        max_dfs(b + 1, ll + mids[b], rr + mids[b], k, 1ULL << (bit_size - b - 1) | val, res);
        max_dfs(b + 1, l - ll, r - rr, k, val, res);
        // note: when requires min, changes to below
        //max_dfs(b + 1, l - ll, r - rr, k, val, res);
        //max_dfs(b + 1, ll + mids[b], rr + mids[b], k, 1ULL << (bit_size - b - 1) | val, res);
    }
    // top k numbers on [l..r)
    std::vector<T> topk(int l, int r, int k) const {
        if(r - l < k) {
            k = r - l;
        }
        if(k < 0) {
            return std::vector<T>();
        }
        std::vector<T> res;
        max_dfs(0, l, r, k, 0, res);
        return res;
    }

    struct freq_list_data {
        T val;
        int count;
        freq_list_data(T v, int cnt) : val(v), count(cnt) {}
    };
    // O(K * logM). K := res.size()
    // return {([num], [count]), ...}.
    // Each [num] satisfies bottom <= [num] < up and in [l..r)
    // [count] is number of occurences of [num].
    void freq_list_dfs(int b, int l, int r, T val, T bottom, T up, std::vector<freq_list_data>& res) const {
        if(val >= up || l >= r) {
            return;
        }
        if(b == bit_size) {
            if(bottom <= val) {
                res.emplace_back(val, r - l);
            }
        }
        T nv = val | (1ULL << (bit_size - b - 1));
        T nnv = nv | ((1ULL << (bit_size - b - 1)) - 1);
        if(nnv < bottom) {
            return;
        }
        int ll = dicts[b].rank(1, l);
        int rr = dicts[b].rank(1, r);
        freq_list_dfs(b + 1, l - ll, r - rr, val, bottom, up, res);
        freq_list_dfs(b + 1, ll + mids[b], rr + mids[b], nv, bottom, up, res);
    }
    std::vector<freq_list_data> freq_list(int l, int r, T bottom, T up) const {
        std::vector<freq_list_data> res;
        freq_list_dfs(0, l, r, 0, bottom, up, res);
        return res;
    }

    struct rect_data {
        int idx;
        T val;
        rect_data(int i, T v) : idx(i), val(v) {}
    };
    // O(K * logM). K := res.size().
    // return {{[idx], [val]}...}
    // Each [val] satisfies bottom <= val < up and in [l..r)
    // [idx] is the index of the [val].
    std::vector<rect_data> get_rect(int l, int r, T bottom, T up) const {
        auto freq = freq_list(l, r, bottom, up);
        std::vector<rect_data> res;
        for(auto& fd : freq) {
            for(int i = 0; i < fd.count; ++i) {
                res.emplace_back(select(fd.val, i, l), fd.val);
            }
        }
        return res;
    }

private:
    int length;
    int bit_size;
    T max_val;
    std::vector<fully_indexable_dictionary> dicts;
    std::vector<int> mids;
};

