#include <PickingMaterial.hpp>

#ifdef __GNUC__
#define LINUX
#else
#define WINDOWS
#endif

void PickingMaterial::CreateMaterial()
{
#ifdef LINUX
    _program->AttachShader("Resources/PickingMaterial/PickingVertexShader.glsl", GL_VERTEX_SHADER);
    _program->AttachShader("Resources/PickingMaterial/PickingFragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->LinkShader();
#endif
#ifdef WIN32
    _program->AttachShader("Resources\\PickingMaterial\\PickingVertexShader.glsl", GL_VERTEX_SHADER);
    _program->AttachShader("Resources\\PickingMaterial\\PickingFragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->LinkShader();
#endif
}