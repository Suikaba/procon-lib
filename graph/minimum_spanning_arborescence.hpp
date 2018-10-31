using weight = int;

struct edge {
    int from, to;
    weight cost;
};

using edges = std::vector<edge>;
using graph = std::vector<edges>;

class minimum_spanning_arborescence {
public:
    minimum_spanning_arborescence(int V, edges const& es_) : es(V) {
        for(auto const& e : es_) {
            es[e.to].push_back(e);
        }
    }

    weight solve(int r) {
        const int n = es.size();
        weight res = 0;
        union_find uf(n);              // �k��󋵂�\��
        std::vector<int> used(n, -1);  // -1: �܂����ĂȂ�, otherwise: �T���̎n�_�̒��_�ԍ�
        used[r] = r; // ���͍ŏ��Ɋm�肳���Ă��܂�
        for(int s = 0; s < n; ++s) {
            std::vector<int> path; // �������̒��_�W��������
            for(int u = s; used[u] == -1;) {
                path.push_back(u);
                used[u] = s;
                if(es[u].empty()) return -1; // ���������ŏ��L���S��؂����݂��Ȃ�
                int use_idx = 0;
                for(int i = 0; i < (int)es[u].size(); ++i) {
                    if(es[u][use_idx].cost > es[u][i].cost) {
                        use_idx = i;
                    }
                }
                auto use_e = es[u][use_idx];
                es[u].erase(std::begin(es[u]) + use_idx); // �g�����ӂ͍폜

                // �{���A�����Ŏ��ȕӂւ̑Ώ�������K�v������B
                // �������A���ȕӂ̃R�X�g�𑫂�����A���ӂ̕ӂ̃R�X�g���ȉ��̂悤��
                // ���������炷�̂ŁA���͌��ʂɉe�����Ȃ��B
                // �����Ď��ȕӂ͂����ɕH�Ɣ��肳��A���̕ӂ���菜����邽�߁A
                // �����ケ��Ŏ��ȕӂɑΉ��������ƂɂȂ��Ă���B
                // �R�[�h�̌����ڂƂ��Ă͂��Ȃ�s���R�Ȃ̂Œ���
                res += use_e.cost;
                for(auto& e : es[u]) {
                    e.cost -= use_e.cost;
                }

                int v = uf.root(use_e.from);
                if(used[v] == s) { // �H�𔭌�
                    int w = -1;
                    std::vector<edge> new_es;
                    do { // �H���܂��
                        w = path.back();
                        path.pop_back();
                        new_es.insert(std::end(new_es), std::begin(es[w]), std::end(es[w]));
                        es[w].clear();
                    } while(uf.unite(v, w));
                    es[uf.root(v)] = std::move(new_es);
                    used[uf.root(v)] = -1; // ���̃��[�v�ł�����x�T�������邽��
                }
                u = uf.root(v);
            }
        }

        return res;
    }

private:
    std::vector<std::vector<edge>> es;
};