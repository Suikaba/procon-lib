
// usage: 
// ./a.out &> data.js

class visualizer {
    // see: visualize.html
    const int canvas_width = 800;
    const int canvas_height = 800;

    const int center_x = 0;
    const int center_y = 0;

    const ld zoom = 2;

public:
    void change_color(int r, int g, int b) {
        fprintf(stderr, "cv.strokeStyle = 'rgb(%d, %d, %d)';\n", r, g, b);
    }

    void draw_point(point p) {
        fprintf(stderr, "circle(%d, %d, %d)\n", cordx(p), cordy(p), 2);
    }

    void draw_segment(segment s) {
        fprintf(stderr, "line(%d, %d, %d, %d)\n", cordx(s.a), cordy(s.a), cordx(s.b), cordy(s.b));
    }

    void draw_line(line l) {
        auto v = l.a - l.b;
        segment s(l.a - v * point(1e4, 0), l.b + v * point(1e4, 0));
        draw_segment(s);
    }

    void draw_polygon(polygon const& p) {
        const int N = p.size();
        for(int i=0; i<N; ++i) {
            draw_segment(segment(p[i], p[(i+1)%N]));
        }
    }

    void draw_circle(circle const& c) {
        fprintf(stderr, "circle(%d, %d, %d)\n", cordx(c.p), cordy(c.p), (int)c.r);
    }

private:
    int cordx(point p) {
        return canvas_width / 2 + zoom * (p.real() - center_x);
    }

    int cordy(point p) {
        return canvas_height / 2 + zoom * (p.imag() - center_y);
    }
};
