template <typename T, unsigned int B = 32>
class trie {
public:
    struct node {
        std::unique_ptr<node> l, r;
        int cnt;
        node(std::unique_ptr<node> l_ = nullptr, std::unique_ptr<node> r_ = nullptr)
            : l(std::move(l_)), r(std::move(r_)), cnt(0)
        {}
    };

public:
    trie() : root(nullptr)
    {}

    void insert(T val) {
        // when you do not want multiset but set, insert below:
        // if(find(val)) return;
        insert(root, val, B - 1);
    }

    void erase(T val) {
        erase(root, val, B - 1);
    }
    
    bool find(T val) const {
        node* t = root.get();
        for(int i = B - 1; i >= 0; --i) {
            if(val & ((T)1 << i)) {
                if(!t->r) {
                    return false;
                }
                t = t->r.get();
            } else {
                if(!t->l) {
                    return false;
                }
                t = t->l.get();
            }
        }
        return true;
    }

    int size() const {
        return count(root);
    }

    T get(int k) const {
        assert(0 <= k && k < count(root));
        return get(root, k, 0, B - 1);
    }

private:
    int count(std::unique_ptr<node> const& t) const {
        return t ? t->cnt : 0;
    }
    T get(std::unique_ptr<node> const& t, int k, T tmp, int b) const {
        if(b < 0) {
            return tmp;
        }
        assert(t);
        int c = count(t->l);
        if(k < c) {
            return get(t->l, k, tmp, b - 1);
        } else {
            return get(t->r, k - c, tmp | ((T)1 << b), b - 1);
        }
    }
    void insert(std::unique_ptr<node>& t, T val, int b) {
        if(!t) {
            t = std::make_unique<node>();
        }
        t->cnt++;
        if(b >= 0) {
            if(val & ((T)1 << b)) {
                insert(t->r, val, b - 1);
            } else {
                insert(t->l, val, b - 1);
            }
        }
    }
    void erase(std::unique_ptr<node>& t, T val, int b) {
        assert(t);
        t->cnt--;
        if(b >= 0) {
            if(val & ((T)1 << b)) {
                erase(t->r, val, b - 1);
            } else {
                erase(t->l, val, b - 1);
            }
        }
        if(t->cnt == 0) {
            t.release();
        }
    }

private:
    std::unique_ptr<node> root;
};