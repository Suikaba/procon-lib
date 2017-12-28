std::vector<int> kmp(std::string const& s) {
    int const n = s.size();
    std::vector<int> res(n + 1);
    res[0] = -1;
    int j = -1;
    for(int i = 0; i < n; ++i) {
        while(j >= 0 && s[i] != s[j]) j = res[j];
        res[i + 1] = ++j;
    }
    return res;
}
