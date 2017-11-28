constexpr double pi = std::acos(-1.0);

using data = std::complex<double>;

// v のサイズは必ず 2 のべき乗であること．
std::vector<data> fft_impl(std::vector<data> v, double theta) {
    int const n = v.size();
    for(int m = n; m >= 2; m >>= 1) {
        int mh = m >> 1;
        data theta_i(0, theta);
        for(int i = 0; i < mh; ++i) {
            data w = std::exp((data)i * theta_i);
            for(int j = i; j < n; j += m) {
                data x = v[j] - v[j + mh]; // (a_k - a_{k + N/2})
                v[j] = v[j] + v[j + mh];   // (a_k + a_{k + N/2})
                v[j + mh] = w * x;         // (a_k - a_{k + N/2}) * W_N^j
            }
        }
        theta *= 2;
    }
    int i = 0;
    for(int j = 1; j < n - 1; ++j) {
        for(int k = n >> 1; k > (i ^= k); k >>= 1);
        if(j < i) {
            swap(v[i], v[j]);
        }
    }
    return v;
}

std::vector<data> fft(std::vector<data> v) {
    return fft_impl(std::move(v), 2 * pi / v.size());
}

std::vector<data> inverse_fft(std::vector<data> v) {
    return fft_impl(std::move(v), -2 * pi / v.size());
}

std::vector<data> convolution(std::vector<data> x, std::vector<data> y) {
    int sz = 1;
    int t = x.size() + y.size() - 1;
    while(sz < t) {
        sz <<= 1;
    }
    x.resize(sz); y.resize(sz);
    x = fft(std::move(x)); y = fft(std::move(y));
    for(int i = 0; i < sz; ++i) {
        x[i] *= y[i];
    }
    x = inverse_fft(std::move(x));
    for(auto& xx : x) {
        xx /= sz;
    }
    return x;
}
