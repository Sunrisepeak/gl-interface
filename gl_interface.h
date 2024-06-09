// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2024 - present  Sunrisepeak, speakshen@163.com
//
// ProjectLinks: https://github.com/Sunrisepeak/gl-interface
//

#ifndef GL_INTERFACE_H_GLI
#define GL_INTERFACE_H_GLI

#ifdef GLI_BUILD_LIB
#define GLI_API
#else
#define GLI_API static inline
#endif

#define GLI_API_V

#define TrueGLI 1
#define FalseGLI 0

#ifndef bool
#define BoolGLI int
#else
#define BoolGLI bool
#endif



#ifdef __cplusplus
#define GLI_IF_CPP(expr) expr
extern "C" {
#else
#define GLI_IF_CPP(expr)
#endif

enum DrawMode {
    GLI_POINTS = 0,
    GLI_LINES = 1,
    GLI_LINE_STRIP = 2,
    GLI_LINE_LOOP = 3,
    GLI_TRIANGLES = 4,
    GLI_TRIANGLE_STRIP = 5,
};

enum ColorMode {
    GLI_COL_ONE = 1,
    GLI_COL_MULTI = 2,
};

/*
        y
       ^
       |
       |
       +------> x
      /
     / 
    L  
    z    
*/
typedef struct PointGLI {
    float x, y, z;
} PointGLI;

typedef PointGLI CamDirectionGLI;

typedef struct ColorGLI {
    float r, g, b, a;
} ColorGLI;

static const struct {
    const PointGLI ORIGIN;
    // 1d
    const PointGLI RIGHT;
    const PointGLI LEFT;
    const PointGLI UP;
    const PointGLI DOWN;
    const PointGLI FRONT;
    const PointGLI BACK;
    // 2d
    const PointGLI RU;
    const PointGLI LU;
    const PointGLI LD;
    const PointGLI RD;
    const PointGLI UF;
    const PointGLI DF;
    const PointGLI DB;
    const PointGLI UB;

    const PointGLI RF;
    const PointGLI RB;
    const PointGLI LF;
    const PointGLI LB;
    // 3d
    const PointGLI RUF;
    const PointGLI RDF;
    const PointGLI RDB;
    const PointGLI RUB;
    const PointGLI LUF;
    const PointGLI LDF;
    const PointGLI LDB;
    const PointGLI LUB;

} GLI_POSITION = {
    .ORIGIN = { 0, 0, 0 },

    // 1d
    .RIGHT = { 1.0f, 0, 0 },
    .LEFT = { -1.0f, 0, 0 },
    .UP = { 0, 1.0f, 0 },
    .DOWN = { 0, -1.0f, 0 },
    .FRONT = { 0, 0, 1.0f },
    .BACK = { 0, 0, -1.0f },

    // 2d
    .RU = { 1.0f, 1.0f, 0 },
    .LU = { -1.0f, 1.0f, 0 },
    .LD = { -1.0f, -1.0f, 0 },
    .RD = { 1.0f, -1.0f, 0 },
    .UF = { 0, 1.0f, 1.0f },
    .DF = { 0, -1.0f, 1.0f },
    .DB = { 0, -1.0f, -1.0f },
    .UB = { 0, 1.0f, -1.0f },

    .RF = { 1.0f, 0, 1.0f },
    .RB = { 1.0f, 0, -1.0f },
    .LF = { -1.0f, 0, 1.0f },
    .LB = { -1.0f, 0, -1.0f },

    // 3d
    .RUF = { 1.0f, 1.0f, 1.0f },
    .RDF = { 1.0f, -1.0f, 1.0f },
    .RDB = { 1.0f, -1.0f, -1.0f },
    .RUB = { 1.0f, 1.0f, -1.0f },
    .LUF = { -1.0f, 1.0f, 1.0f },
    .LDF = { -1.0f, -1.0f, 1.0f },
    .LDB = { -1.0f, -1.0f, -1.0f },
    .LUB = { -1.0f, 1.0f, -1.0f }
};

static const struct {
    ColorGLI WHITE;
    ColorGLI BLACK;
    ColorGLI RED;
    ColorGLI GREEN;
    ColorGLI BLUE;
    ColorGLI YELLOW;
    ColorGLI CYAN;
    ColorGLI MAGENTE;
    ColorGLI GRAY;
    ColorGLI ORANGE;
    ColorGLI PURPLE;
    ColorGLI BROWN;
} GLI_COLORS = {
    .WHITE = { 1.0f, 1.0f, 1.0f, 1.0f },
    .BLACK = { 0.0f, 0.0f, 0.0f, 1.0f },
    { 1.0f, 0.0f, 0.0f, 1.0f },
    { 0.0f, 1.0f, 0.0f, 1.0f },
    { 0.0f, 0.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 0.0f, 1.0f },
    { 0.0f, 1.0f, 1.0f, 1.0f },
    { 1.0f, 0.0f, 1.0f, 1.0f },
    { 0.5f, 0.5f, 0.5f, 1.0f },
    { 1.0f, 0.65f, 0.0f, 1.0f },
    { 0.5f, 0.0f, 0.5f, 1.0f },
    { 0.65f, 0.16f, 0.16f, 1.0f }
};

struct GraphicsDataGLI {
    struct {
        enum DrawMode draw;
        enum ColorMode color;
    } mode;
    unsigned int vertexNums;
    float *vertexs;
    float *colors;
    float thickness;
    BoolGLI filled;
};

// Core API
GLI_API_V void gli_backend_init(void *extend);
GLI_API_V void gli_viewport(int x, int y, int w, int h);
GLI_API_V void gli_clear();
GLI_API_V void gli_draw(struct GraphicsDataGLI *gData);
GLI_API_V void gli_backend_deinit();

// Option - 3D Support
GLI_API_V void gli_camera_pos(float x, float y, float z);
GLI_API_V void gli_camera_target(float x, float y, float z);
GLI_API_V void gli_camera_direction(CamDirectionGLI direction);
GLI_API_V void gli_camera_fov(float fov);
GLI_API_V void gli_camera_rotation(float angle);
GLI_API_V void gli_camera_update();


/////// - graphics api
GLI_API void gli_point(PointGLI p, ColorGLI col, float size GLI_IF_CPP(= 1.0));
// line
GLI_API void gli_line(PointGLI p1, PointGLI p2, ColorGLI col, float thickness GLI_IF_CPP(= 1.0));
GLI_API void gli_line_base(PointGLI p1, ColorGLI col1, PointGLI p2, ColorGLI col2, float thickness);
GLI_API void gli_line_strip(PointGLI *points, int pNum, ColorGLI col, float thickness);
GLI_API void gli_line_strip_base(PointGLI *points, int pNum, ColorGLI col, float thickness);

// triangle
GLI_API void gli_triangle(PointGLI p1, PointGLI p2, PointGLI p3, ColorGLI col, float thickness GLI_IF_CPP(= 1.0));
GLI_API void gli_triangle_filled(PointGLI p1, PointGLI p2, PointGLI p3, ColorGLI col);
GLI_API void gli_triangle_base(
    PointGLI p1, ColorGLI col1,
    PointGLI p2, ColorGLI col2,
    PointGLI p3, ColorGLI col3,
    BoolGLI filled GLI_IF_CPP(= false)
);

// rectangle - TODO: test 2d and 3d (0, 1)-(1, 0) and delta(x or y or z) = 0
GLI_API void gli_rectangle(PointGLI p1, PointGLI p2, ColorGLI col, float thickness);
GLI_API void gli_rectangle_filled(PointGLI p1, PointGLI p2, ColorGLI col);
GLI_API void gli_rectangle_base(
    PointGLI p1, PointGLI p2,
    ColorGLI colRD, ColorGLI colRU, ColorGLI colLU, ColorGLI colLD,
    BoolGLI filled GLI_IF_CPP(= false), float thickness GLI_IF_CPP(= 1.0)
);

// circle and ngon
GLI_API void gli_circle(
    PointGLI center, float radius, ColorGLI col,
    int segmentsNum GLI_IF_CPP(= 0), float thickness GLI_IF_CPP(= 1.0)
);
GLI_API void gli_circle_filled(
    PointGLI center, float radius, ColorGLI col,
    int segmentsNum GLI_IF_CPP(= 0)
);
GLI_API void gli_ngon(
    PointGLI center, float radius, ColorGLI col,
    int segmentsNum GLI_IF_CPP(= 5), float thickness GLI_IF_CPP(= 1.0)
);
GLI_API void gli_ngon_filled(
    PointGLI center, float radius, ColorGLI col,
    int segmentsNum GLI_IF_CPP(= 5)
);

GLI_API void gli_polygon(PointGLI *points, int pNums, ColorGLI col, float thickness GLI_IF_CPP(= 1.0));
GLI_API void gli_polygon_filled(PointGLI *points, int pNums, ColorGLI col);

GLI_API void gli_coordinate();
GLI_API void gli_space();
GLI_API PointGLI gli_pos_obj(float x, float y, float z);
GLI_API void gli_pos_normalization(PointGLI *p1, PointGLI *p2);
GLI_API PointGLI gli_col_obj(float x, float y, float z);

#ifdef __cplusplus
} /* extern "C" */
#endif

GLI_API void gli_point(PointGLI p, ColorGLI col, float size) {
    struct GraphicsDataGLI data;

    data.mode.draw = GLI_POINTS;
    data.mode.color = GLI_COL_ONE;
    data.vertexNums = 1;
    data.colors = &col.r;
    data.vertexs = &p.x;
    data.thickness = size;

    gli_draw(&data);
}

GLI_API void gli_line(PointGLI p1, PointGLI p2, ColorGLI col, float thickness) {
    struct GraphicsDataGLI data;

    float vertexs[2 * 3] = { p1.x, p1.y, p1.z, p2.x, p2.y, p2.z };

    data.mode.draw = GLI_LINES;
    data.mode.color = GLI_COL_ONE;
    data.vertexNums = 2;
    data.colors = &col.r;
    data.vertexs = vertexs;
    data.thickness = thickness;

    gli_draw(&data);
}

GLI_API void gli_line_strip(PointGLI *points, int pNum, ColorGLI col, float thickness) {
    struct GraphicsDataGLI data;

    data.mode.draw = GLI_LINE_STRIP;
    data.mode.color = GLI_COL_ONE;
    data.vertexNums = pNum;
    data.colors = &col.r;
    data.vertexs = &(points->x);
    data.thickness = thickness;

    gli_draw(&data);
}

GLI_API void gli_rectangle(PointGLI p1, PointGLI p2, ColorGLI col, float thickness) {
    struct GraphicsDataGLI data;

    gli_pos_normalization(&p1, &p2);

    float deltaX = p2.x - p1.x;
    float deltaY = p2.y - p1.y;
    float deltaZ = p2.z - p1.z;

    float vertexs[3 * 4] = {
        p1.x, p1.y, p1.z, // LT
        p1.x + deltaX, p1.y, p1.z + (deltaY == 0 ? 0 : deltaZ), // RT
        p2.x, p2.y, p2.z, // RB
        p1.x, p1.y + deltaY, p1.z + (deltaY == 0 ? deltaZ : 0) // LB
    };

    data.mode.draw = GLI_LINE_LOOP;
    data.mode.color = GLI_COL_ONE;
    data.vertexNums = 4;
    data.vertexs = vertexs;
    data.colors = &col.r;

    gli_draw(&data);
}

GLI_API void gli_rectangle_filled(PointGLI p1, PointGLI p2, ColorGLI col) {
    struct GraphicsDataGLI data;

    gli_pos_normalization(&p1, &p2);

    float deltaX = p2.x - p1.x;
    float deltaY = p2.y - p1.y;
    float deltaZ = p2.z - p1.z;

    float vertexs[3 * 3 * 2] = {
        p1.x, p1.y, p1.z, // LT
        p1.x + deltaX, p1.y, p1.z + (deltaY == 0 ? 0 : deltaZ), // RT
        p2.x, p2.y, p2.z, // RB
        p2.x, p2.y, p2.z, // RB
        p1.x, p1.y + deltaY, p1.z + (deltaY == 0 ? deltaZ : 0), // LB
        p1.x, p1.y, p1.z, // LT
    };

    data.mode.draw = GLI_TRIANGLES;
    data.mode.color = GLI_COL_ONE;
    data.vertexNums = 6;
    data.vertexs = vertexs;
    data.colors = &col.r;

    gli_draw(&data);
}

GLI_API void gli_rectangle_base(
    PointGLI p1, PointGLI p2,
    ColorGLI colRD, ColorGLI colRU, ColorGLI colLU, ColorGLI colLD,
    BoolGLI filled, float thickness
) {
    struct GraphicsDataGLI data;

    ColorGLI colors[4] = { colRD, colRU, colLU, colLD };

    gli_pos_normalization(&p1, &p2);

    float deltaX = p2.x - p1.x;
    float deltaY = p2.y - p1.y;
    float deltaZ = p2.z - p1.z;

    float vertexs[3 * 4] = {
        p1.x, p1.y, p1.z, // LT
        p1.x + deltaX, p1.y, p1.z + (deltaY == 0 ? 0 : deltaZ), // RT
        p2.x, p2.y, p2.z, // RB
        p1.x, p1.y + deltaY, p1.z + (deltaY == 0 ? deltaZ : 0) // LB
    };

    data.mode.draw = GLI_LINE_LOOP;
    data.mode.color = GLI_COL_MULTI;
    data.vertexNums = 4;
    data.vertexs = vertexs;
    data.colors = &colors[0].r;
    data.thickness = thickness;

    gli_draw(&data);
}

GLI_API void gli_polygon(PointGLI *points, int pNums, ColorGLI col, float thickness) {

    struct GraphicsDataGLI data;

    data.mode.draw = GLI_LINE_LOOP;
    data.mode.color = GLI_COL_ONE;
    data.vertexNums = pNums;
    data.vertexs = &(points[0].x);
    data.colors = &col.r;

    gli_draw(&data);
}

GLI_API void gli_triangle(PointGLI p1, PointGLI p2, PointGLI p3, ColorGLI col, float thickness) {
    PointGLI points[3] = { p1, p2, p3 };
    gli_polygon(points, 3, col, 1);  
}

GLI_API void gli_triangle_filled(PointGLI p1, PointGLI p2, PointGLI p3, ColorGLI col) {
    struct GraphicsDataGLI data;

    PointGLI vertexs[3] = {p1, p2, p3};

    data.mode.draw = GLI_TRIANGLE_STRIP;
    data.mode.color = GLI_COL_ONE;
    data.vertexNums = 3;
    data.vertexs = &(vertexs[0].x);
    data.colors = &(col.r);

    gli_draw(&data);
}

GLI_API void gli_triangle_base(
    PointGLI p1, ColorGLI col1,
    PointGLI p2, ColorGLI col2,
    PointGLI p3, ColorGLI col3,
    BoolGLI filled
) {
    struct GraphicsDataGLI data;

    PointGLI vertexs[3] = {p1, p2, p3};
    ColorGLI colors[3] = {col1, col2, col3};

    data.mode.draw = filled ? GLI_TRIANGLES : GLI_LINE_STRIP;
    data.mode.color = GLI_COL_MULTI;
    data.vertexNums = 3;
    data.vertexs = &(vertexs[0].x);
    data.colors = &(colors[0].r);

    gli_draw(&data);
}

// helper

GLI_API PointGLI gli_pos_obj(float x, float y, float z) {
    PointGLI p = {x, y, z};
    return p;
}

GLI_API ColorGLI gli_color_obj(float r, float g, float b, float a) {
    ColorGLI col = {r, g, b, a};
    return col;
}

GLI_API void gli_pos_normalization(PointGLI *p1, PointGLI *p2) {
    PointGLI p;
    // TODO: Optimize
    p.x = p1->x < p2->x ? p1->x : p2->x;
    p.y = p1->y < p2->y ? p1->y : p2->y;
    p.z = p1->z < p2->z ? p1->z : p2->z;
    p2->x = p1->x > p2->x ? p1->x : p2->x;
    p2->y = p1->y > p2->y ? p1->y : p2->y;
    p2->z = p1->z > p2->z ? p1->z : p2->z;
    p1->x = p.x;
    p1->y = p.y;
    p1->z = p.z;
}

GLI_API PointGLI gli_pos_add(PointGLI p1, PointGLI p2) {
    PointGLI p = {p1.x + p2.x, p1.y + p2.y, p1.z + p2.z};
    return p;
}

GLI_API void gli_coordinate() {
    gli_line(GLI_POSITION.ORIGIN, GLI_POSITION.RIGHT, GLI_COLORS.RED, 1);
    gli_line(GLI_POSITION.ORIGIN, GLI_POSITION.UP, GLI_COLORS.GREEN, 1);
    gli_line(GLI_POSITION.ORIGIN, GLI_POSITION.FRONT, GLI_COLORS.BLUE, 1);
}

GLI_API void gli_space() {
    gli_point(GLI_POSITION.ORIGIN, GLI_COLORS.BLACK, 10);

    gli_point(GLI_POSITION.RIGHT, GLI_COLORS.RED, 5);
    gli_point(GLI_POSITION.LEFT, GLI_COLORS.RED, 5);
    gli_point(GLI_POSITION.UP, GLI_COLORS.RED, 5);
    gli_point(GLI_POSITION.DOWN, GLI_COLORS.RED, 5);
    gli_point(GLI_POSITION.FRONT, GLI_COLORS.RED, 5);
    gli_point(GLI_POSITION.BACK, GLI_COLORS.RED, 5);

    gli_point(GLI_POSITION.RU, GLI_COLORS.GREEN, 5);
    gli_point(GLI_POSITION.LU, GLI_COLORS.GREEN, 5);
    gli_point(GLI_POSITION.LD, GLI_COLORS.GREEN, 5);
    gli_point(GLI_POSITION.RD, GLI_COLORS.GREEN, 5);
    gli_point(GLI_POSITION.UF, GLI_COLORS.GREEN, 5);
    gli_point(GLI_POSITION.DF, GLI_COLORS.GREEN, 5);
    gli_point(GLI_POSITION.DB, GLI_COLORS.GREEN, 5);
    gli_point(GLI_POSITION.UB, GLI_COLORS.GREEN, 5);

    gli_point(GLI_POSITION.RF, GLI_COLORS.GREEN, 5);
    gli_point(GLI_POSITION.RB, GLI_COLORS.GREEN, 5);
    gli_point(GLI_POSITION.LF, GLI_COLORS.GREEN, 5);
    gli_point(GLI_POSITION.LB, GLI_COLORS.GREEN, 5); 

    gli_point(GLI_POSITION.RUF, GLI_COLORS.BLUE, 5);
    gli_point(GLI_POSITION.RDF, GLI_COLORS.BLUE, 5);
    gli_point(GLI_POSITION.RDB, GLI_COLORS.BLUE, 5);
    gli_point(GLI_POSITION.RUB, GLI_COLORS.BLUE, 5);
    gli_point(GLI_POSITION.LUF, GLI_COLORS.BLUE, 5);
    gli_point(GLI_POSITION.LDF, GLI_COLORS.BLUE, 5);
    gli_point(GLI_POSITION.LDB, GLI_COLORS.BLUE, 5);
    gli_point(GLI_POSITION.LUB, GLI_COLORS.BLUE, 5);
}

#endif /* GL_INTERFACE_H_GLI */