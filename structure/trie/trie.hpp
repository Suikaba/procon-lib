// !! Not verified

struct alphabet_t { // default: 'a' - 'z'
    static constexpr int size = 26;
    static constexpr int char_to_index(char c) {
        assert('a' <= c && c <= 'z');
        return c - 'a';
    }
    static constexpr char index_to_char(int idx) {
        assert(0 <= idx && idx < size);
        return 'a' + idx;
    }
};

template <typename Alpha>
class trie {
    static constexpr int alpha_sz = Alpha::size;
    using cnt_t = int;

public:
    trie() : cnt(0), cnt_subt(0), terminate(false) {}

    void insert(std::string const& s) {
        if(find(s)) return; // when multiset, comment out this line
        auto cur_node = this;
        cur_node->cnt_subt += 1;
        for(int p = 0; p < static_cast<int>(s.size()); ++p) {
            const auto c = Alpha::char_to_index(s[p]);
            if(!next[c]) next[c] = std::make_unique<trie<Alpha>>();
            cur_node = next[c].get();
            cur_node->cnt_subt += 1;
        }
        cur_node->terminate = true;
        cur_node->cnt += 1;
    }

    bool find(std::string const& s) const {
        auto cur_node = this;
        for(int p = 0; p < static_cast<int>(s.size()); ++p) {
            const auto c = Alpha::char_to_index(s[p]);
            if(!next[c]) return false;
            cur_node = next[c].get();
        }
        return cur_node->terminate;
    }

    std::string kth_element(cnt_t k) const { // 1-indexed
        assert(cnt_subt >= k);
        k -= cnt;
        while(k > 0) {
            for(int i = 0; i < alpha_sz; ++i) {
                if(next[i] && next[i]->cnt_subt >= k) {
                    return Alpha::index_to_char(i) + next[i]->kth_element(k);
                }
                k -= next[i] ? next[i]->cnt_subt : 0;
            }
        }
        return "";
    }

private:
    std::array<std::unique_ptr<trie<Alpha>>, alpha_sz> next;
    cnt_t cnt, cnt_subt;
    bool terminate;
};

