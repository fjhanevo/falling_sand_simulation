#include "renderer.h"
#include "constants.h"
#include "particle.h"
#include "particle_registry.h"
#include "grid.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

Renderer::Renderer(int gridW, int gridH)
    : m_gridW(gridW), m_gridH(gridH),
      m_pixelBuffer(gridW * gridH * 3) // 3 for RGB
{
    buildQuad();
    buildShader();

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, gridW, gridH,
                 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Renderer::drawGrid(const Grid& grid)
{
    // fill pixel buffer from grid data
    for (int y { 0 }; y < m_gridH; ++y) {
        for (int x { 0 }; x < m_gridW; ++x) {
            Particle type { grid.getType(x, y) };
            auto [r, g, b] { getParticleColor(type) };
            int i { (x + y * m_gridW) * 3 };
            m_pixelBuffer[i]   = r;
            m_pixelBuffer[i+1] = g;
            m_pixelBuffer[i+2] = b;
        }
    }

    // upload textures to the GPU
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_gridW, m_gridH,
                 0, GL_RGB, GL_UNSIGNED_BYTE, m_pixelBuffer.data());

    // draw fullscreen quad
    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::buildQuad()
{
    float xMax { 1.0f - (float)RIGHT_PANEL_PX  / (SCREEN_WIDTH  / 2.0f) };
    float yMin{ 1.0f - (float)BOTTOM_PANEL_PX / (SCREEN_HEIGHT / 2.0f) };

    float vertices[] = {
            // position     //tex coords
            -1.0f, 1.0f,      0.0f, 1.0f,    // top left
            -1.0f, yMin,      0.0f, 0.0f,    // bottom left
             xMax, yMin,      1.0f, 0.0f,    // bottom right

            -1.0f, 1.0f,      0.0f, 1.0f,    // top left
             xMax, yMin,      1.0f, 1.0f,    // bottom right
             xMax, 1.0f,      1.0f, 0.0f     // top right
        };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute (location = 0)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // tex coord attribute (location = 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Renderer::buildShader()
{
    // read shader source files
    auto readFile = [](const std::string& path) -> std::string {
        std::ifstream file(path);
        if (!file) 
            throw std::runtime_error("Failed to open shader: " + path);
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    };

    std::string vertSrc = readFile("res/particles.vert");
    std::string fragSrc = readFile("res/particles.frag");

    const char* vSrcPtr = vertSrc.c_str();
    const char* fSrcPtr = fragSrc.c_str();

    // compile vertex shaders
    GLuint vert { glCreateShader(GL_VERTEX_SHADER) };
    glShaderSource(vert, 1, &vSrcPtr, nullptr);
    glCompileShader(vert);
    checkCompileErrors(vert, "VERTEX");

    // compile fragment shader
    GLuint frag { glCreateShader(GL_FRAGMENT_SHADER) };
    glShaderSource(frag, 1, &fSrcPtr, nullptr);
    glCompileShader(frag);
    checkCompileErrors(frag, "FRAGMENT");

    // link the program
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vert);
    glAttachShader(m_shaderProgram, frag);
    glLinkProgram(m_shaderProgram);
    checkCompileErrors(m_shaderProgram, "PROGRAM");

    glDeleteShader(vert);
    glDeleteShader(frag);
    
}

void Renderer::checkCompileErrors(GLuint shader, const std::string &type)
{
    GLint success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << '\n'
                << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << '\n'
                << infoLog << std::endl;
        }
    }
}
