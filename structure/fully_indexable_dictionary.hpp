
using u32 = unsigned int;

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
    static const int interval = 32;
    static const int not_found = -1;

public:
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

    // k + 1 番目の b の位置
    // blocks 上で二分探索してから rank で二分探索したほうが早い
    // 実装が楽なので今は遅いほうだけど，そのうち書き直す．
    int select(bool b, int x) {
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
    int select(bool b, int x, int left) {
        return select(b, x + rank(b, left));
    }

private:
    int length;
    int block_size;
    std::vector<u32> blocks;
    std::vector<int> rank_table;
};

