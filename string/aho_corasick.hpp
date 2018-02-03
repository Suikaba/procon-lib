
struct alphabets { // example: lower letters
    static int const size = 26;
    static int convert(char c) {
        assert('a' <= c && c <= 'z');
        return c - 'a';
    }
    static int convert_inv(int i) {
        assert(0 <= i && i < size);
        return 'a' + i;
    }
};

template <typename Alphabets>
class aho_corasick {
    struct PMA {
        int const id;
        PMA* fail;
        std::vector<std::unique_ptr<PMA>> next;
        std::vector<int> accept;
        PMA(int i) : id(i), fail(), next(Alphabets::size) {}
    };

public:
    aho_corasick(std::vector<std::string> const& ts)
    : K(ts.size()), root(std::make_unique<PMA>(0)) {
        int idx = 1; // node id
        root->fail = root.get();
        for(int i = 0; i < K; ++i) {
            auto t = root.get();
            for(auto cc : ts[i]) {
                int c = Alphabets::convert(cc);
                if(!t->next[c]) {
                    t->next[c] = std::make_unique<PMA>(idx++);
                }
                t = t->next[c].get();
            }
            t->accept.push_back(i);
        }

        std::queue<PMA*> que;
        for(int c = 0; c < Alphabets::size; ++c) {
            if(root->next[c]) {
                root->next[c]->fail = root.get();
                que.push(root->next[c].get());
            }
        }
        while(!que.empty()) {
            auto t = que.front();
            que.pop();
            for(int c = 0; c < Alphabets::size; ++c) {
                if(t->next[c]) {
                    que.push(t->next[c].get());
                    auto nxt = transition(t->fail, Alphabets::convert_inv(c));
                    t->next[c]->fail = nxt;
                    for(auto ac : nxt->accept) {
                        t->next[c]->accept.push_back(ac);
                    }
                }
            }
        }
    }

    PMA* transition(PMA* node, char cc) {
        int c = Alphabets::convert(cc);
        auto now = node;
        while(!now->next[c] && now != root.get()) now = now->fail;
        now = now->next[c].get();
        if(now == nullptr) now = root.get();
        return now;
    }

    std::vector<std::vector<int>> match(std::string const& s) {
        std::vector<std::vector<int>> res(K);
        PMA* now = root.get();
        for(int i = 0; i < (int)s.size(); ++i) {
            now = transition(now, s[i]);
            for(auto k : now->accept) {
                res[k].push_back(i);
            }
        }
        return res;
    }

    PMA* get_root() const {
        return root.get();
    }

private:
    int const K;
    std::unique_ptr<PMA> root;
};