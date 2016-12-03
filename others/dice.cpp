#include <vector>

enum {
    TOP = 0,
    FRONT,
    RIGHT,
    LEFT,
    BACK,
    BOTTOM
};

class dice {
public:
    dice(int top, int front) 
        : v_(6)
    {
        int right[7][7] = {};
        // change 
        right[1][2] = 3; right[1][3] = 5; right[1][4] = 2; right[1][5] = 4;
        right[2][1] = 4; right[2][3] = 1; right[2][4] = 6; right[2][6] = 3;
        right[3][1] = 2; right[3][2] = 6; right[3][5] = 1; right[3][6] = 5;
        right[4][1] = 5; right[4][2] = 1; right[4][5] = 6; right[4][6] = 2;
        right[5][1] = 3; right[5][3] = 6; right[5][4] = 1; right[5][6] = 4;
        right[6][2] = 4; right[6][3] = 2; right[6][4] = 5; right[6][5] = 3;
        v_[TOP] = top;
        v_[FRONT] = front;
        v_[BOTTOM] = 7 - top;
        v_[BACK] = 7 - front;
        v_[RIGHT] = right[top][front];
        v_[LEFT] = 7 - v_[RIGHT];
    }
    dice(vector<int> const& v)
    : v_(v)
    {}

    dice roll(int dir) {
        vector<int> res(6);
        for(int i=0; i<6; ++i) {
            res[i] = v_[rot[dir][i]];
        }
        return dice(res);
    }

    int get(int idx) {
        if(idx < 0 || 6 <= idx) {
            return -1;
        }
        return v_[idx];
    }

private:
    std::vector<int> v_;
    int rot[6][6] = {{4, 0, 2, 3, 5, 1},  // south 
                     {3, 1, 0, 5, 4, 2},  // east
                     {2, 1, 5, 0, 4, 3},  // west
                     {1, 5, 2, 3, 0, 4},  // north 
                     {0, 2, 4, 1, 3, 5},  // right(not move)
                     {0, 3, 1, 4, 2, 5}}; // left(not move)
};

