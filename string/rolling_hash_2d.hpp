// ハッシュ値を贅沢にペアで計算しているため，MODのコストが馬鹿にならない
// 2^64 でmodを取る感じにすると mod サボれるのでそっちののほうが良いかも．
class rolling_hash_2d {
    using ll = long long;
public:
    rolling_hash_2d(std::vector<std::vector<int>> const& v)
        : H(v.size()), W(v[0].size()),
          p(2, std::vector<std::vector<ll>>(2, std::vector<ll>(std::max(H, W) + 1, 1))),
          hs(2, std::vector<std::vector<ll>>(H + 1, std::vector<ll>(W + 1)))
    {
        for(int i = 0; i < 2; ++i) {
            for(int j = 0; j < 2; ++j) {
                for(int k = 0; k + 1 < max(W, H); ++k) {
                    p[i][j][k + 1] = p[i][j][k] * B[i] % M[j];
                }
            }
        }
        for(int i = 0; i < 2; ++i) {
            for(int j = 0; j < H; ++j) {
                for(int k = 0; k < W; ++k) {
                    assert(j + 1 < hs[i].size());
                    hs[i][j + 1][k + 1] = (hs[i][j + 1][k] * B[i] + v[j][k]) % M[i];
                }
            }
        }
        for(int i = 0; i < 2; ++i) {
            for(int j = 0; j < H; ++j) {
                for(int k = 0; k < W; ++k) {
                    assert(j + 1 < hs[i].size());
                    hs[i][j + 1][k + 1] = ((hs[i][j][k + 1] * B[!i]) + hs[i][j + 1][k + 1]) % M[i];
                }
            }
        }
    }

    // [y1, y2) x [x1, x2)
    std::pair<ll, ll> query(int y1, int x1, int y2, int x2) const {
        std::pair<ll, ll> res(0, 0);
        for(int i = 0; i < 2; ++i) {
            auto& r = (i == 0 ? res.first : res.second);
            r = (hs[i][y2][x2] - hs[i][y2][x1] * p[i][i][x2 - x1] % M[i] + M[i]) % M[i];
            r = (r - hs[i][y1][x2] * p[!i][i][y2 - y1] % M[i] + M[i]) % M[i];
            r = (r + (hs[i][y1][x1] * p[!i][i][y2 - y1] % M[i]) * p[i][i][x2 - x1] % M[i]) % M[i];
        }
        return res;
    }

private:
    static constexpr ll B[2] = {9973, 100000007};
    static constexpr ll M[2] = {1000000007, 1000000009};
    int const H, W;
    std::vector<std::vector<std::vector<ll>>> p;
    std::vector<std::vector<std::vector<ll>>> hs;
};

constexpr ll rolling_hash_2d::B[2];
constexpr ll rolling_hash_2d::M[2];
