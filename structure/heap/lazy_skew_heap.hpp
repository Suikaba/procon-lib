// 以下は全体加算を遅延するもの
// function が遅いかもしれない、定数倍厳しいときは一般化するのをやめよう
template <typename T, typename Lazy>
class lazy_skew_heap {
    struct node {
        T val;
        Lazy lazy_val;
        std::unique_ptr<node> l, r;

        node(T v) : val(v), lazy_val(0), l(nullptr), r(nullptr) {}
    };
    using node_ptr = std::unique_ptr<node>;

public:
    lazy_skew_heap(std::function<T(T, Lazy)> op_) : op(op_) {}

    void meld(lazy_skew_heap& sh) {
        root = meld(std::move(root), std::move(sh.root));
    }

    void push(T val) {
        root = meld(std::move(root), std::make_unique<node>(val));
    }

    void add(Lazy lazy_val) {
        if(!root) return;
        root->lazy_val += lazy_val;
    }

    T top() const {
        return op(root->val, root->lazy_val);
    }

    void pop() {
        push_lazy(root.get());
        root = meld(std::move(root->r), std::move(root->l));
    }

    bool empty() const {
        return !root;
    }

private:
    node_ptr meld(node_ptr a, node_ptr b) {
        using std::swap;
        if(!a) return std::move(b);
        if(!b) return std::move(a);
        if(op(a->val, a->lazy_val) > op(b->val, b->lazy_val)) swap(a, b);

        push_lazy(a.get());
        a->r = meld(std::move(a->r), std::move(b));
        swap(a->l, a->r);

        return std::move(a);
    }
    void push_lazy(node* n) {
        if(n->l) n->l->lazy_val += n->lazy_val;
        if(n->r) n->r->lazy_val += n->lazy_val;
        n->val = op(n->val, n->lazy_val);
        n->lazy_val = 0; // todo
    }

private:
    std::function<T(T, Lazy)> op;
    node_ptr root;
};
