// each i, find max len s.t. S[0:len-1] = S[i:i+len-1]
std::vector<int> z_algo(std::string const& s) {
    const int n = s.size();
    std::vector<int> a(n);
    a[0] = n;
    for(int i = 1, j = 0; i < n;) {
        while(i + j < n && s[j] == s[i + j]) ++j;
        a[i] = j;
        if(j == 0) {
            ++i;
            continue;
        }
        int k = 1;
        while(i + k < n && k + a[k] < j) {
            a[i + k] = a[k];
            k++;
        }
        i += k; j -= k;
    }
    return a;
}
