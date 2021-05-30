#include "model.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <GL/gl.h>

class GLModel::GLModel {
    public:
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        GLuint tex;
};