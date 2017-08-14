
template <typename T>
class treap {
    struct node {
        T val;
        node* l_child, *r_child;
        int priority;
        unsigned int sz;
        T sum;
        T mini;

        node(T v, double p)
            : val(v), priority(p), sz(1), sum(v), mini(v)
        {
            l_child = r_child = nullptr;
        }
        ~node() {
            delete l_child;
            delete r_child;
        }
    };

    unsigned int size(node* t) const {
        return !t ? 0 : t->sz;
    }

    T sum(node* t) {
        return !t ? 0 : t->sum;
    }

    T min(node* t) {
        return !t ? std::numeric_limits<T>::max() : t->mini; 
    }

    node* update(node* t) {
        t->sz = size(t->l_child) + size(t->r_child) + 1;
        t->sum = sum(t->l_child) + sum(t->r_child) + t->val;
        t->mini = std::min(std::min(min(t->l_child), min(t->r_child)), t->val);
        return t;
    }

    node* merge(node* l, node* r) {
        if(!l || !r) {
            return !l ? r : l;
        }

        if(l->priority > r->priority) {
            l->r_child = merge(l->r_child, r);
            return update(l);
        } else {
            r->l_child = merge(l, r->l_child);
            return update(r);
        }
    }

    // [0, k), [k, n)
    std::pair<node*, node*> split(node* t, unsigned int k) {
        if(!t) {
            return std::make_pair(nullptr, nullptr);
        }

        if(k <= size(t->l_child)) {
            auto s = split(t->l_child, k);
            t->l_child = s.second;
            return std::make_pair(s.first, update(t));
        } else {
            auto s = split(t->r_child, k - size(t->l_child) - 1);
            t->r_child = s.first;
            return std::make_pair(update(t), s.second);
        }
    }

    node* insert(node* t, int k, T v, int p) {
        auto s = split(t, k);
        t = merge(s.first, new node(v, p));
        t = merge(t, s.second);
        return update(t);
    }

    node* erase(node* t, unsigned int k) {
        auto u = split(t, k + 1);
        auto v = split(u.first, k);
        t = merge(v.first, u.second);
        delete v.second;
        return update(t);
    }

    T range_minimum_query(int l, int r, node* t) {
        int sz = size(t);
        if(r <= 0 || sz <= l) {
            return std::numeric_limits<T>::max();
        }
        if(l <= 0 && sz <= r) {
            return min(t);
        }

        sz = size(t->l_child);
        T vl = range_minimum_query(l, r, t->l_child);
        T vr = range_minimum_query(l - sz - 1, r - sz - 1, t->r_child);
        T res = std::min(vl, vr);
        if(l <= sz && sz < r) {
            res = std::min(res, t->val);
        }
        return res;
    }

    T sum(int l, int r, node* t) {
        int sz = size(t);
        if(r <= 0 || sz <= l) {
            return 0;
        }
        if(l <= 0 && sz <= r) {
            return sum(t);
        }

        sz = size(t->l_child);
        T vl = sum(l, r, t->l_child);
        T vr = sum(l - sz - 1, r - sz - 1, t->r_child);
        T res = vl + vr;
        if(l <= sz && sz < r) {
            res += t->val;
        }
        return res;
    }


    // member variable
    node* root;

public:
    treap()
        : root(nullptr)
    {
        std::srand(time(nullptr));
    }
    treap(int n, T val) {
        for(int i = 0; i < n; ++i) {
            insert(i, val);
        }
    }
    ~treap() {
        delete root;
    }


    unsigned int size() const {
        return size(root);
    }

    void insert(unsigned int k, T val) {
        root = insert(root, k, val, std::rand());
    }

    void erase(int k) {
        root = erase(root, k);
    }

    void update(int k, T v) {
        erase(k);
        insert(k, v);
    }

    node* find(unsigned int k) {
        node* res = root;
        while(res) {
            unsigned int s = size(res->l_child);
            if(s > k) {
                res = res->l_child;
            } else if(s == k) {
                return res;
            } else {
                res = res->r_child;
                k -= s + 1;
            }
        }
        return res;
    }

    // [l, r)
    T range_minimum_query(int l, int r) {
        return range_minimum_query(l, r, root);
    }

    void shift(int l, int r) {
        auto s1 = split(root, l);             // [0, l), [l, ...)
        auto s2 = split(s1.second, r - l);    // [l, r), [r, ...)
        auto s3 = split(s2.first, r - l - 1); // [l, r - 1), r - 1

        root = merge(s1.first, s3.second);
        root = merge(root, s3.first);
        root = merge(root, s2.second);
    }

    T sum(int l, int r) {
        return sum(l, r, root);
    }
};

