
template <typename T>
class skew_heap {
    struct heap {
        T val;
        std::unique_ptr<heap> l, r;

        heap(T v) : val(v), l(nullptr), r(nullptr) {}
    };

public:
    skew_heap() : root(nullptr) {}

    void meld(skew_heap& sh) {
        root = meld(move(root), move(sh.root));
    }

    void push(T val) {
        root = meld(move(root), std::unique_ptr<heap>(new heap(val)));
    }

    T top() const {
        return root->val;
    }

    void pop() {
        root = meld(move(root->r), move(root->l));
    }

    bool empty() const {
        return !root;
    }

private:
    std::unique_ptr<heap> meld(std::unique_ptr<heap> a, std::unique_ptr<heap> b) {
        if(!a) {
            return move(b);
        }
        if(!b) {
            return move(a);
        }
        if(a->val > b->val) {
            swap(a, b);
        }
        a->r = meld(move(a->r), move(b));
        swap(a->l, a->r);

        return move(a);
    }
private:
    std::unique_ptr<heap> root;
};

