struct alphabets { // example: lower letters
    static int const size = 26;
    static int convert(char c) {
		assert('a' <= c && c <= 'z');
		return c - 'a';
    }
    static char convert_inv(int i) {
		assert(0 <= i && i < size);
		return 'a' + i;
    }
};

template <typename Alphabets>
class aho_corasick {
    static const int invalid_idx = -1;

    struct PMA {
        int fail;
        std::vector<int> next, accept;
    
        PMA() : fail(invalid_idx), next(Alphabets::size, invalid_idx) {}
    };

public:
    aho_corasick(std::vector<std::string> const& ts)
    : K(ts.size()) {
        const int root_idx = 0;
        nodes.push_back(std::make_unique<PMA>()); // root node
        nodes[root_idx]->fail = root_idx; // root idx
        for(int i = 0; i < K; ++i) {
            int now = root_idx;
            for(auto cc : ts[i]) {
                int c = Alphabets::convert(cc);
                if(nodes[now]->next[c] == invalid_idx) {
                    nodes[now]->next[c] = static_cast<int>(nodes.size());
                    nodes.push_back(std::make_unique<PMA>());
                }
                now = nodes[now]->next[c];
            }
            nodes[now]->accept.push_back(i);
        }

        std::queue<int> que;
        for(int c = 0; c < Alphabets::size; ++c) {
            if(nodes[root_idx]->next[c] != invalid_idx) {
                nodes[nodes[root_idx]->next[c]]->fail = root_idx;
                que.push(nodes[root_idx]->next[c]);
            }
        }
        while(!que.empty()) {
            int now = que.front();
            que.pop();
            for(int c = 0; c < Alphabets::size; ++c) {
                if(nodes[now]->next[c] != invalid_idx) {
                    que.push(nodes[now]->next[c]);
                    int nxt = transition(nodes[now]->fail, Alphabets::convert_inv(c));
                    nodes[nodes[now]->next[c]]->fail = nxt;
                    for(auto ac : nodes[nxt]->accept) {
                        nodes[nodes[now]->next[c]]->accept.push_back(ac);
                    }
                }
            }
        }
    }

    int transition(int node_idx, char cc) {
        assert(0 <= node_idx && node_idx < static_cast<int>(nodes.size()));
        int c = Alphabets::convert(cc);
        int now = node_idx;
        while(nodes[now]->next[c] == invalid_idx && now != 0) {
            now = nodes[now]->fail;
        }
        now = nodes[now]->next[c];
        if(now == invalid_idx) now = 0;
        return now;
    }

    bool is_accept(int node_idx) const {
        assert(0 <= node_idx && node_idx < static_cast<int>(nodes.size()));
        return nodes[node_idx]->accept.size() != 0;
    }


    std::vector<int> accept_list(int node_idx) const {
        assert(0 <= node_idx && node_idx < static_cast<int>(nodes.size()));
        return nodes[node_idx]->accept;
    }

    std::vector<std::vector<int>> match(std::string const& s) {
        std::vector<std::vector<int>> res(K);
        int now = 0;
        for(int i = 0; i < static_cast<int>(s.size()); ++i) {
            now = transition(now, s[i]);
            for(auto k : nodes[now]->accept) {
                res[k].push_back(i);
            }
        }
        return res;
    }

    int node_size() const {
        return static_cast<int>(nodes.size());
    }

private:
    const int K;
    std::vector<std::unique_ptr<PMA>> nodes;
};