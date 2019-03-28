#include <DefaultMaterial.hpp>
#ifdef __GNUC__
#define LINUX
#else
#define WINDOWS
#endif
void DefaultMaterial::CreateMaterial()
{
#ifdef LINUX
    _program->AttachShader("Resources/DefaultMaterial/VertexShader.glsl", GL_VERTEX_SHADER);
    _program->AttachShader("Resources/DefaultMaterial/FragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->LinkShader();
#endif

#ifdef WINDOWS
    _program->AttachShader("Resources\\DefaultMaterial\\VertexShader.glsl", GL_VERTEX_SHADER);
    _program->AttachShader("Resources\\DefaultMaterial\\FragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->LinkShader();
#endif

    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "color");
    glUniform4fv(location, 1, (float*)&color);
}

void DefaultMaterial::UpdateColor(glm::vec4 color)
{
    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "color");
    glUniform4fv(location, 1, (float*)&color);
}