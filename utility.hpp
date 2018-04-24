int popcount(unsigned long long a) {
    a = (a & 0x5555555555555555) + (a >> 1 & 0x5555555555555555);
    a = (a & 0x3333333333333333) + (a >> 2 & 0x3333333333333333);
    a = (a & 0x0f0f0f0f0f0f0f0f) + (a >> 4 & 0x0f0f0f0f0f0f0f0f);
    a = (a & 0x00ff00ff00ff00ff) + (a >> 8 & 0x00ff00ff00ff00ff);
    a = (a & 0x0000ffff0000ffff) + (a >> 16 & 0x0000ffff0000ffff);
    return (a & 0xffffffff) + (a >> 32);
}

template<typename T>
std::vector<T> table(int n, T v) { return std::vector<T>(n, v); }
 
template <class... Args>
auto table(int n, Args... args) {
  auto val = table(args...);
  return std::vector<decltype(val)>(n, std::move(val));
}