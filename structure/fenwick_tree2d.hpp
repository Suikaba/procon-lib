
template <typename T>
class fenwick_tree2d {
public:
    fenwick_tree2d(int h, int w)
        : H(h), W(w), dat(H, std::vector<T>(W))
    {}

    void add(int i, int j, T value) {
        for(; i < H; i |= i + 1) {
            int jj = j;
            for(; jj < W; jj |= jj + 1) {
                dat[i][jj] += value;
            }
        }
    }

    // [0..i] x [0..j]
    T sum(int i, int j) const {
        T res = 0;
        for(; i >= 0; i = (i & (i + 1)) - 1) {
            int jj = j;
            for(; jj >= 0; jj = (jj & (jj + 1)) - 1) {
                res += dat[i][jj];
            }
        }
        return res;
    }

    // [i1...i2) x [j1...j2)
    T sum(int i1, int j1, int i2, int j2) const {
        return sum(i2 - 1, j2 - 1) - sum(i1 - 1, j2 - 1) - sum(i2 - 1, j1 - 1) + sum(i1 - 1, j1 - 1);
    }

private:
    int const H, W;
    std::vector<std::vector<T>> dat;
};

