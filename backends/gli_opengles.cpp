#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// OpenGL ES
#include <GLES3/gl3.h>
#include <EGL/egl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLI
#define GLI_BUILD_LIB
#include <gl_interface.h>

// EGL variables
static EGLDisplay gEglDisplay = EGL_NO_DISPLAY;
static EGLSurface gEglSurface = EGL_NO_SURFACE;
static EGLContext gEglContext = EGL_NO_CONTEXT;

const char *vertexShaderSource = "#version 300 es\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec4 aColor;\n"
    "out vec4 posColor;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * vec4(aPos, 1.0);\n"
    "   posColor = aColor;\n"
    "}\0";

const char *fragmentShaderSource = "#version 300 es\n"
    "precision mediump float;\n"
    "in vec4 posColor;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = posColor;\n"
    "}\n\0";

static unsigned int gShaderProgram;
static unsigned int gVBO, gVAO;
static unsigned int gCmdMapTable[100] = { 0 };

static struct {
    struct {
        float w, h; // window
    } window;
    struct {
        float pointSize;
        float lineThickness;
        float r, g, b, a;
    } graphics; // default config
    glm::vec4 viewport;
    float vertexBuff[7 * 180]; // TODO: use dynamic memory
    // camera
    float fov, aspect, near, far;
    glm::vec3 camPos, camTarget, camDirectionGLI;
    glm::mat4 model, view, projection;
} gGLI;

void * gli_malloc(unsigned int size) {
    return malloc(size);
}

void gli_free(void *ptr) {
    free(ptr);
}

void gli_backend_init(void *extend) {
    // EGL initialization
/*  TODO: use glfw to create egl context or use eglCreateContext ?
    gEglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (gEglDisplay == EGL_NO_DISPLAY) {
        printf("Failed to get EGL display\n");
        return;
    }

    EGLint majorVersion, minorVersion;
    if (!eglInitialize(gEglDisplay, &majorVersion, &minorVersion)) {
        printf("Failed to initialize EGL\n");
        return;
    }

    const EGLint configAttribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_STENCIL_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_NONE
    };

    EGLConfig config;
    EGLint numConfigs;
    if (!eglChooseConfig(gEglDisplay, configAttribs, &config, 1, &numConfigs)) {
        printf("Failed to choose config\n");
        return;
    }

    gEglSurface = eglCreateWindowSurface(gEglDisplay, config, (EGLNativeWindowType)extend, NULL);
    if (gEglSurface == EGL_NO_SURFACE) {
        printf("Failed to create EGL surface\n");
        return;
    }

    EGLint contextAttribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
    };
    gEglContext = eglCreateContext(gEglDisplay, config, EGL_NO_CONTEXT, contextAttribs);
    if (gEglContext == EGL_NO_CONTEXT) {
        printf("Failed to create EGL context\n");
        return;
    }

    if (!eglMakeCurrent(gEglDisplay, gEglSurface, gEglSurface, gEglContext)) {
        printf("Failed to make context current\n");
        return;
    }
*/
    // Shader compilation and program creation
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    gShaderProgram = glCreateProgram();
    glAttachShader(gShaderProgram, vertexShader);
    glAttachShader(gShaderProgram, fragmentShader);
    glLinkProgram(gShaderProgram);

    GLint success;
    glGetProgramiv(gShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        printf("shader program error\n");
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &gVAO);
    glGenBuffers(1, &gVBO);
    glBindVertexArray(gVAO);

    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gGLI.vertexBuff), gGLI.vertexBuff, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    gGLI.fov = 45.0f; gGLI.aspect = 1;
    gGLI.near = 1, gGLI.far = -1;
    gGLI.camPos = { 0, 0, 5.0f };
    gGLI.camTarget = { 0, 0, 0 };
    gGLI.camDirectionGLI = { 0, 1.0f, 0 };

    gCmdMapTable[GLI_POINTS] = GL_POINTS;
    gCmdMapTable[GLI_LINES] = GL_LINES;
    gCmdMapTable[GLI_LINE_STRIP] = GL_LINE_STRIP;
    gCmdMapTable[GLI_LINE_LOOP] = GL_LINE_LOOP;
    gCmdMapTable[GLI_TRIANGLES] = GL_TRIANGLES;
    gCmdMapTable[GLI_TRIANGLE_FAN] = GL_TRIANGLE_FAN;

    glUseProgram(gShaderProgram);
    gGLI.view = glm::mat4(1);
    gGLI.projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f);
    glUniformMatrix4fv(glGetUniformLocation(gShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(gGLI.view));
    glUniformMatrix4fv(glGetUniformLocation(gShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(gGLI.projection));

    printf("gli_backend_init: done\n");
}

void gli_viewport(int x, int y, int w, int h) {
    gGLI.aspect = w * 1.0 / h;
    gGLI.viewport = {x, y, w, h};
    glViewport(x, y, w, h);
}

void gli_clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gli_camera_pos(float x, float y, float z) {
    gGLI.camPos = { x, y, z };
}

void gli_camera_target(float x, float y, float z) {
    gGLI.camTarget = { x, y, z };
}

void gli_camera_direction(VectorGLI direction) {
    gGLI.camDirectionGLI = { direction.x, direction.y, direction.z };
}

void gli_camera_fov(float fov) {
    gGLI.fov = fov;
}

GLI_API_V void gli_camera_aspect(float aspect) {
    gGLI.aspect = aspect;
}

GLI_API_V void gli_camera_clipping(float near, float far) {
    gGLI.near = near;
    gGLI.far = far;
}

GLI_API_V void gli_2d(float size, float x, float y) {
    glUniformMatrix4fv(
        glGetUniformLocation(gShaderProgram, "view"),
        1, GL_FALSE,
        glm::value_ptr(glm::mat4(1))
    );
    glUniformMatrix4fv(
        glGetUniformLocation(gShaderProgram, "projection"),
        1, GL_FALSE,
        glm::value_ptr(glm::ortho(
            -size * gGLI.aspect + x, size * gGLI.aspect + x,
            -size + y, size + y,
            -size, size
        ))
    );
}

void gli_camera_update() {
    glUseProgram(gShaderProgram);

    gGLI.view = glm::lookAt(gGLI.camPos, gGLI.camTarget, gGLI.camDirectionGLI);

    gGLI.projection = glm::perspective(
        glm::radians(gGLI.fov),
        gGLI.aspect,
        gGLI.near, gGLI.far
    );

    glUniformMatrix4fv(
        glGetUniformLocation(gShaderProgram, "view"),
        1, GL_FALSE, glm::value_ptr(gGLI.view)
    );
    glUniformMatrix4fv(
        glGetUniformLocation(gShaderProgram, "projection"),
        1, GL_FALSE, glm::value_ptr(gGLI.projection)
    );
}

void gli_backend_deinit() {
    glDeleteVertexArrays(1, &gVAO);
    glDeleteBuffers(1, &gVBO);
    glDeleteProgram(gShaderProgram);

    eglMakeCurrent(gEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(gEglDisplay, gEglContext);
    eglDestroySurface(gEglDisplay, gEglSurface);
    eglTerminate(gEglDisplay);

    printf("gli_backend_deinit: done\n");
}

void gli_draw(struct GraphicsDataGLI *gData) {
    if (GLI_FALSE == gData->filled) {
        //glPointSize(gData->thickness);
        glLineWidth(gData->thickness);
    }

    for (int i = 0; i < gData->vertexNums; i++) {
        gGLI.vertexBuff[7 * i] = gData->vertices[3 * i];
        gGLI.vertexBuff[7 * i + 1] = gData->vertices[3 * i + 1];
        gGLI.vertexBuff[7 * i + 2] = gData->vertices[3 * i + 2];

        if (gData->mode.color == GLI_COL_ONE) {
            gGLI.vertexBuff[7 * i + 3] = gData->colors[0];
            gGLI.vertexBuff[7 * i + 4] = gData->colors[1];
            gGLI.vertexBuff[7 * i + 5] = gData->colors[2];
            gGLI.vertexBuff[7 * i + 6] = gData->colors[3];
        } else if (gData->mode.color == GLI_COL_MULTI) {
            gGLI.vertexBuff[7 * i + 3] = gData->colors[4 * i];
            gGLI.vertexBuff[7 * i + 4] = gData->colors[4 * i + 1];
            gGLI.vertexBuff[7 * i + 5] = gData->colors[4 * i + 2];
            gGLI.vertexBuff[7 * i + 6] = gData->colors[4 * i + 3];
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferSubData(
        GL_ARRAY_BUFFER, 0,
        gData->vertexNums * sizeof(float) * 7,
        gGLI.vertexBuff
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(gCmdMapTable[gData->mode.draw], 0, gData->vertexNums);
}

GLI_API_V void gli_render() {
    eglSwapBuffers(gEglDisplay, gEglSurface);
}

GLI_API_V float gli_sin(float x) {
    return sinf(x);
}

GLI_API_V float gli_cos(float x) {
    return cosf(x);
}