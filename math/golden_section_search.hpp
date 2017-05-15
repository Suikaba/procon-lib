//#include <cmath>

constexpr double gratio = (std::sqrt(5)+1)/2.0;

// 下に凸な関数Fの区間[lb, ub]における最小となる点を求める．
// cnt は 200 ぐらいなら十分な精度になってた．
template <typename F>
double golden_section_search(F f, double lb, double ub, int cnt) {
    double x1 = (ub-lb)/(gratio+1) + lb,
           x2 = (ub-lb)*gratio/(gratio+1) + lb;
    double v1 = f(x1), v2 = f(x2);
    for(int i=0; i<cnt; ++i) {
        if(v1 < v2) {
            ub = x2;
            x2 = x1;
            v2 = v1;
            x1 = (ub-lb)/(gratio+1) + lb;
            v1 = f(x1);
        } else {
            lb = x1;
            x1 = x2;
            v1 = v2;
            x2 = (ub-lb)*gratio/(gratio+1) + lb;
            v2 = f(x2);
        }
    }
    return (lb+ub)/2;
}
