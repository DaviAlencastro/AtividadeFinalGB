#ifndef SPRITE_ANIMADO_H
#define SPRITE_ANIMADO_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

class SpriteAnimado {
public:
    SpriteAnimado(const std::string& path, int nFrames, int nAnimations);
    virtual ~SpriteAnimado();

    virtual void desenhar(glm::vec2 posicao, glm::vec2 escala = glm::vec2(64.0f, 64.0f));
    virtual void atualizar(float deltaTime);
    void setAnimacao(int linha);
    void setShader(GLuint shaderID);

protected:
    GLuint texturaID;
    GLuint VAO, VBO;
    int numColunas, numLinhas;
    int frameAtual, animacaoAtual;
    float tempoPorFrame, tempoAcumulado;
    GLuint shader;
};

#endif
