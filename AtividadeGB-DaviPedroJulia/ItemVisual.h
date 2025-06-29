// ItemVisual.h
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

class ItemVisual {
public:
    ItemVisual(const std::string& caminho, GLuint shaderID); // sem int extra
    ~ItemVisual();

    void desenhar(glm::vec2 pos, glm::vec2 tamanho = glm::vec2(32, 32));

private:
    GLuint textureID;
    GLuint shaderProgram;
};
