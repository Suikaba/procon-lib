template <typename T> // T := integer
class binary_trie {
    using cnt_t = int;
    static constexpr int max_b = std::numeric_limits<T>::digits;

public:
    binary_trie() : cnt(0) {}

    bool insert(T val) {
        //if(find(val)) return false; // when multiset, comment out this line
        auto cur_node = this;
        cur_node->cnt += 1;
        for(int i = max_b - 1; i >= 0; --i) {
            const int b = (val >> i) & 1;
            if(!cur_node->ch[b]) cur_node->ch[b] = std::make_unique<binary_trie>();
            cur_node = cur_node->ch[b].get();
            cur_node->cnt += 1;
        }
        return true;
    }

    bool erase(T val) { // node: not erased empty node
        if(!find(val)) return false;
        auto cur_node = this;
        cur_node->cnt -= 1;
        for(int i = max_b - 1; i >= 0; --i) {
            cur_node = cur_node->ch[(val >> i) & 1].get();
            cur_node->cnt -= 1;
        }
        return true;
    }

    bool find(T val) const {
        auto cur_node = this;
        for(int i = max_b - 1; i >= 0; --i) {
            const int b = (val >> i) & 1;
            if(!cur_node->ch[b]) return false;
            cur_node = cur_node->ch[b].get();
        }
        return true;
    }

    T kth_element(cnt_t k) const { // k is 1-indexed
        auto cur_node = this;
        T res = 0;
        for(int b = max_b - 1; b >= 0; --b) {
            if(count(cur_node->ch[0]) >= k) {
                cur_node = cur_node->ch[0].get();
            } else {
                k -= count(cur_node->ch[0]);
                cur_node = cur_node->ch[1].get();
                res |= ((T)1 << b);
            }
        }
        return res;
    }

private:
    cnt_t count(std::unique_ptr<binary_trie> const& n) const {
        return n ? n->cnt : 0;
    }

private:
    cnt_t cnt;
    std::array<std::unique_ptr<binary_trie>, 2> ch;
};
