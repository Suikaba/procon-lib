struct slide_min {
    using data = int;
    std::deque<std::pair<int, data>> deq;
    int c, w;
    slide_min(int w) : c(0), w(w) {}
    data query(data d) {
        while(!deq.empty() && deq.front().first <= c - w) deq.pop_front();
        while(!deq.empty() && deq.back().second >= d) deq.pop_back();
        deq.push_back(make_pair(c++, d));
        return deq.front().second;
    }
};
