#ifndef CHARACTER_CONTROLLER_H
#define CHARACTER_CONTROLLER_H

#include "SpriteAnimado.h"
#include "TileMap.h"
#include <GLFW/glfw3.h>

class CharacterController {
public:
    CharacterController(GLFWwindow* win, GLuint shaderID, TileMap* mapa);
    void atualizar(float deltaTime);
    void desenhar();
    int getI() const;
    int getJ() const;
    int moedasTotal = 0;

private:
    void resetarPosicao(); // Adicione esta declaração

    GLFWwindow* janela;
    TileMap* tilemap;
    SpriteAnimado* spriteAtual;
    SpriteAnimado spriteFrente, spriteCosta, spriteEsquerda, spriteDireita;
    SpriteAnimado spriteFrenteIdle, spriteCostaIdle, spriteEsquerdaIdle, spriteDireitaIdle;
    int linha, coluna;
    const int linhaInicial, colunaInicial; // Adicione estas constantes
    float velocidade;
    float ultimoTempoInput;
};

#endif
