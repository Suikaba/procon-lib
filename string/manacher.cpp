std::vector<int> manacher(std::string const& s) {
    int const n = s.size();
    std::vector<int> res(n);
    int i = 0, j = 0;
    while(i < n) {
        while(i - j >= 0 && i + j < n && s[i - j] == s[i + j]) ++j;
        res[i] = j;
        int k = 1;
        while(i - k >= 0 && i + k < n && k + res[i - k] < j) {
            res[i + k] = res[i - k];
            ++k;
        }
        i += k; j -= k;
    }
    return res;
}
