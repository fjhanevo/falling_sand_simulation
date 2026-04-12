// renderer.h
#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include "grid.h"

class Renderer 
{
public:
    Renderer(int gridW, int gridH);
    ~Renderer();

    void drawGrid(const Grid& grid);
    void buildQuad(int winWidth, int winHeight);

private:
    int m_gridW, m_gridH;
    GLuint m_texture {};
    GLuint m_vao {};
    GLuint m_vbo {};
    GLuint m_shaderProgram {};

    std::vector<uint8_t> m_pixelBuffer;

    void buildShader();
    void checkCompileErrors(GLuint shader, const std::string& type);
};
