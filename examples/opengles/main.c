#include <stdio.h>
#include <math.h>

#include <GLES3/gl3.h>  // 对于 OpenGL ES 3.0
#include <EGL/egl.h>
#include <GLFW/glfw3.h>

#include "gl_interface.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    gli_viewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

int main() {
    // 初始化 GLFW
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    // 设置 OpenGL ES 版本（这里假设使用 OpenGL ES 3.0）
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "GLI-OpenGL-ES Demo", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 设置视口大小回调
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
/*
    // gl interface backend 初始化
    // 获取原生窗口句柄
    EGLNativeWindowType native_window;
    #if defined(_WIN32)
        native_window = (EGLNativeWindowType)glfwGetWin32Window(window);
    #elif defined(__APPLE__)
        native_window = (EGLNativeWindowType)glfwGetCocoaWindow(window);
    #else
        native_window = (EGLNativeWindowType)glfwGetX11Window(window);
    #endif
*/
    gli_backend_init(NULL /* (void *)native_window */);
    gli_viewport(0, 0, 800, 600);

    gli_camera_pos(0, 5, 5);
    gli_camera_update();

    // 主循环
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        gli_clear(0.2, 0.1, 0.3, 0.5);

        gli_camera_rotation(2);

        gli_coordinate();
        gli_space();

        // 您的绘制代码保持不变
        PointGLI controlPos[4] = {
            gli_pos_obj(-1, 0, 0),
            gli_pos_obj(-0.5, 0.5, 1),
            gli_pos_obj(0.5, 0.5, -1),
            gli_pos_obj(1, 0, 0)
        };

        gli_bezier_curve(controlPos, 4, GLI_COLORS.CYAN, 36, 3);

        GLI_2D(3, 0, 0, {
            gli_rectangle(
                gli_pos_obj(1, 1, 0), gli_pos_obj(2, 2, 0),
                GLI_COLORS.ORANGE, 3
            );
        });

        gli_rectangle(
            gli_pos_obj(1, 1, 0), gli_pos_obj(2, 2, 0),
            GLI_COLORS.ORANGE, 3
        );

        GLI_2D(3, 0, 0, {
            gli_line(
                gli_pos_obj(1, 1, 0), gli_pos_obj(2, 2, 0),
                GLI_COLORS.ORANGE, 3
            );
        });

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    gli_backend_deinit();

    glfwTerminate();

    return 0;
}