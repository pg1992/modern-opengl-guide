#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);  // Windowed
    // GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", glfwGetPrimaryMonitor(), nullptr);  // Fullscreen

    glfwMakeContextCurrent(window);

    // Testing GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    float vertices[] = {
         0.0f,  0.5f,  // Vertex 1 (X, Y)
         0.5f, -0.5f,  // Vertex 2 (X, Y)
        -0.5f, -0.5f,  // Vertex 3 (X, Y)
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;  // GL unsigned int...
    glGenBuffers(1, &vbo);  // Generate 1 buffer (VBO == Vertex Buffer Object)

    glBindBuffer(GL_ARRAY_BUFFER, vbo);  // make it the active array buffer object
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // copy vertex data to it

    const char* vertexSource = R"glsl(
        #version 150 core

        in vec2 position;

        void main()
        {
            gl_Position = vec4(position, 0.0, 1.0);
        }
    )glsl";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLint vertexStatus;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexStatus);

    if (vertexStatus != GL_TRUE)
    {
        char buffer[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
        printf("%s", buffer);
    }

    const char* fragmentSource = R"glsl(
        #version 150 core

        out vec4 outColor;

        void main()
        {
            outColor = vec4(1.0, 1.0, 1.0, 1.0);
        }
    )glsl";

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    GLint fragmentStatus;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentStatus);

    if (fragmentStatus != GL_TRUE)
    {
        char buffer[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
        printf("%s", buffer);
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glBindFragDataLocation(shaderProgram, 0, "outColor");

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    while (!glfwWindowShouldClose(window))
    {
        // glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }

    glfwTerminate();
    return 0;
}
