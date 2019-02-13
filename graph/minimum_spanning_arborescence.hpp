
// for lazy add.
// If you need more efficiency, use node pool.
template <typename T>
class lazy_skew_heap {
    struct node {
        std::pair<T, int> val; // (val, node_idx)
        T lazy;
        std::unique_ptr<node> l, r;
        node(std::pair<T, int> v) : val(v), lazy(0) {}
    };
    using node_ptr = std::unique_ptr<node>;

public:
    void meld(lazy_skew_heap& other) {
        root = meld(std::move(root), std::move(other.root));
    }

    void push(std::pair<T, int> val) {
        root = meld(std::move(root), std::make_unique<node>(val));
    }

    void add(T val) {
        if(!root) return;
        root->lazy += val;
    }

    std::pair<T, int> top() const {
        return std::make_pair(root->val.first + root->lazy, root->val.second);
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
        if(!a) return b;
        if(!b) return a;
        if(a->val.first + a->lazy > b->val.first + b->lazy) swap(a, b);
        push_lazy(a.get());
        a->r = meld(std::move(a->r), std::move(b));
        swap(a->l, a->r);
        return a;
    }

    void push_lazy(node* n) {
        if(n->l) n->l->lazy += n->lazy;
        if(n->r) n->r->lazy += n->lazy;
        n->val.first += n->lazy;
        n->lazy = 0;
    }

private:
    node_ptr root;
};


class union_find {
public:
    union_find(int n) : par(n, -1) {}

    int root(int x) {
        return par[x] < 0 ? x : par[x] = root(par[x]);
    }

    bool unite(int x, int y) {
        x = root(x), y = root(y);
        if(x == y) return false;
        if(par[x] > par[y]) std::swap(x, y);
        par[x] += par[y];
        par[y] = x;
        return true;
    }

    bool same(int x, int y) {
        return root(x) == root(y);
    }

    int size(int x) {
        return -par[root(x)];
    }

private:
    std::vector<int> par;
};


// Calculate Minimum Spanning Arborescence cost.
// @param g: weighted (directed) graph
// @param root: root
// @return MSA cost (when msa do no exist, return -1)
// @complexity O(E log V)
template <typename Edge, typename Cost = typename Edge::cost_type>
Cost msa(graph<Edge> const& g, const int root) {
    const int n = g.size();
    std::vector<lazy_skew_heap<Cost>> heaps(n);
    for(int v = 0; v < n; ++v) {
        for(auto const& e : g[v]) {
            heaps[e.to].push(std::make_pair(e.cost, v));
        }
    }

    Cost res = 0;
    union_find uf(n); // represents contraction of graph
    std::vector<int> used(n, -1); // -1: not checked, otherwise: vertex where search started
    used[root] = root;
    for(int s = 0; s < n; ++s) {
        std::vector<int> path; // vertex set on process
        for(int u = s; used[u] == -1;) {
            path.push_back(u);
            used[u] = s;

            if(heaps[u].empty()) return -1; // MSA not exists

            auto p = heaps[u].top();
            heaps[u].pop(); // erase used edge
            // note: Even for self-loop edge, below 2 line is ok
            res += p.first;
            heaps[u].add(-p.first);

            const int v = uf.root(p.second);
            if(used[v] == s) { // found cycle
                int w = -1;
                lazy_skew_heap<Cost> nheap;
                do { // move on cycle
                    w = path.back();
                    path.pop_back();
                    nheap.meld(heaps[w]);
                } while(uf.unite(v, w));
                heaps[uf.root(v)] = std::move(nheap);
                used[uf.root(v)] = -1; // uf.root(v) need one more search
            }
            u = uf.root(v);
        }
    }
    return res;
}

