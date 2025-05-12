#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

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

    std::cout << "Settings:" << std::endl;
    std::cout << "  depth bits: " << settings.depthBits << std::endl;
    std::cout << "  stencil bits: " << settings.stencilBits << std::endl;
    std::cout << "  antialiasing level: " << settings.antiAliasingLevel << std::endl;
    std::cout << "  version: " << settings.majorVersion << "." << settings.minorVersion << std::endl;

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

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...

    return 0;
}
