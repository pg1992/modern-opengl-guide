#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

static const float vertices[] =
{
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  // Top-left
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  // Top-right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f,  // Bottom-right

     0.5f, -0.5f, 0.0f, 0.0f, 1.0f,  // Bottom-right
    -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  // Bottom-left
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  // Top-left
};

int main()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antiAliasingLevel = 2;  // Optional
    // Request OpenGL version 3.2
    settings.majorVersion = 3;
    settings.minorVersion = 2;
    settings.attributeFlags = sf::ContextSettings::Core;

    // create the window
    sf::Window window(sf::VideoMode({800, 600}), "OpenGL", sf::Style::Close, sf::State::Windowed, settings);

    sf::ContextSettings actualSettings = window.getSettings();

    std::cout << "Actual settings:" << std::endl;
    std::cout << "  depth bits: " << actualSettings.depthBits << std::endl;
    std::cout << "  stencil bits: " << actualSettings.stencilBits << std::endl;
    std::cout << "  antialiasing level: " << actualSettings.antiAliasingLevel << std::endl;
    std::cout << "  version: " << actualSettings.majorVersion << "." << actualSettings.minorVersion << std::endl;

    window.setVerticalSyncEnabled(true);

    // activate the window
    if (!window.setActive(true)) {
        std::cerr << "Failed to activate window!\n";
        return -1;
    }

    glewExperimental = GL_TRUE;
    glewInit();

    // Vertex Array Objects
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // The next step is to upload the vertex data to the graphics card
    GLuint vbo;
    glGenBuffers(1, &vbo);  // Generate 1 buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);  // make it the active object
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // copy the data

    // Vertex Shader Source
    const char* vertex_shader_source = R"glsl(
        #version 150 core

        in vec2 position;
        in vec3 color;

        out vec3 Color;

        void main()
        {
            Color = color;
            gl_Position = vec4(position, 0.0, 1.0);
        }
    )glsl";

    // Create a shader object
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    GLint vertex_status;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_status);
    if (vertex_status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(vertex_shader, 512, NULL, buffer);
        std::cerr << "Vertex Shader compilation error: " << buffer << std::endl;
        return -1;
    }

    // Fragment Shader Source
    const char* fragment_shader_source = R"glsl(
        #version 150 core

        in vec3 Color;

        out vec4 outColor;

        void main()
        {
            outColor = vec4(Color, 1.0);
        }
    )glsl";

    // Create a shader object
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    GLint fragment_status;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_status);
    if (fragment_status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(fragment_shader, 512, NULL, buffer);
        std::cerr << "Fragment Shader compilation error: " << buffer << std::endl;
        return -1;
    }

    // Combining shaders into a program
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    // Bind the shader variable to frag shader output
    glBindFragDataLocation(shader_program, 0, "outColor");

    // Linking and using the shader program
    glLinkProgram(shader_program);
    glUseProgram(shader_program);

    // Making the link between vertex data and attributes
    GLint posAttrib = glGetAttribLocation(shader_program, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
    glEnableVertexAttribArray(posAttrib);

    GLint colorAttrib = glGetAttribLocation(shader_program, "color");
    glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(colorAttrib);

    // run the main loop
    bool running = true;
    while (running)
    {
        // handle events
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                // end the program
                running = false;
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    running = false;
            }
            else if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, resized->size.x, resized->size.y);
            }
        }

        // clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw...
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...

    return 0;
}
