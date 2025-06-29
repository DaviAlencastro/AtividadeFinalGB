// Personagem.h
#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "C:\Users\Davi\Documentos\OpenGL\AtividadeGB-DaviPedroJulia\SpriteAnimado.h"
#include "C:\Users\Davi\Documentos\OpenGL\AtividadeGB-DaviPedroJulia\TileMap.h"

class Personagem {
public:
    Personagem(const std::string& pathIdle, const std::string& pathWalk, GLuint shaderID);
    void atualizar(float deltaTime);
    void desenhar();
    glm::vec2 getPosicao();

private:
    TileMap* mapa;
    SpriteAnimado spriteCima, spriteBaixo, spriteEsquerda, spriteDireita;
    SpriteAnimado spriteCimaIdle, spriteBaixoIdle, spriteEsquerdaIdle, spriteDireitaIdle;
    SpriteAnimado* spriteAtual;
    glm::vec2 posicao;
    float velocidade;
    GLuint shader;
    glm::vec2 direcao;
    SpriteAnimado* ultimoSpriteMovimento;
};

#endif // PERSONAGEM_H
