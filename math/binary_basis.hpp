
using ull = unsigned long long;

// 直交するように基底を選んでいく
void binary_basis_insert(std::vector<ull>& w, ull a) {
    for(auto v : w) {
        if(v & (-v) & a) {
            a ^= v;
        }
    }
    if(a == 0) { // すでに生成可能
        return;
    }
    for(auto& v : w) {
        if(a & (-a) & v) {
            v ^= a;
        }
    }
    w.push_back(a);
}

std::vector<ull> binary_basis(std::vector<ull> const& w) {
    std::vector<ull> res;
    for(auto v : w) {
        binary_basis_insert(res, v);
    }
    return res;
}

