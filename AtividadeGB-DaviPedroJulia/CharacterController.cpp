#include "CharacterController.h"
#include <iostream>

CharacterController::CharacterController(GLFWwindow* win, GLuint shaderID, TileMap* mapa)
    : janela(win), tilemap(mapa),
      spriteFrente("imagens/D_Walk.png", 1, 3),
      spriteCosta("imagens/U_Walk.png", 1, 3),
      spriteEsquerda("imagens/L_Walk.png", 1, 3),
      spriteDireita("imagens/R_Walk.png", 1, 3),
      spriteFrenteIdle("imagens/D_Idle.png", 1, 2),
      spriteCostaIdle("imagens/U_Idle.png", 1, 2),
      spriteEsquerdaIdle("imagens/L_Idle.png", 1, 2),
      spriteDireitaIdle("imagens/R_Idle.png", 1, 2),
      linha(7), coluna(7), // Posição atual
      linhaInicial(7), colunaInicial(7), // Inicialize aqui
      velocidade(100.0f), ultimoTempoInput(0.0f)
{
    spriteFrente.setShader(shaderID);
    spriteCosta.setShader(shaderID);
    spriteEsquerda.setShader(shaderID);
    spriteDireita.setShader(shaderID);

    spriteFrenteIdle.setShader(shaderID);
    spriteCostaIdle.setShader(shaderID);
    spriteEsquerdaIdle.setShader(shaderID);
    spriteDireitaIdle.setShader(shaderID);

    spriteAtual = &spriteFrenteIdle;
}

int CharacterController::getI() const { return linha; }
int CharacterController::getJ() const { return coluna; }

// CharacterController.cpp - Modificar a função atualizar
void CharacterController::atualizar(float deltaTime) {
    static SpriteAnimado* ultimaAnimacao = &spriteFrenteIdle;
    static int ultimaLinha = -1;
    static int ultimaColuna = -1;
    int novaLinha = linha;
    int novaColuna = coluna;
    bool moveu = false;
    float tempoAtual = glfwGetTime();
    // Informa ao tilemap a posição atual do jogador (NOVO)
    tilemap->setPlayerPosition(linha, coluna);
    // Calcula fator de escurecimento pulsante (NOVO)
    static float pulseTime = 0.0f;
    pulseTime += deltaTime;
    float darken = 0.5f * (1.0f + sin(pulseTime * 3.0f));
    tilemap->setDarkenFactor(darken);
    if (tempoAtual - ultimoTempoInput >= 0.15f) {
        // Lógica de movimento
        if (glfwGetKey(janela, GLFW_KEY_E) == GLFW_PRESS) {
            novaLinha--;
            ultimaAnimacao = &spriteDireita;
            moveu = true;
        } else if (glfwGetKey(janela, GLFW_KEY_Z) == GLFW_PRESS) {
            novaLinha++;
            ultimaAnimacao = &spriteEsquerda;
            moveu = true;
        } else if (glfwGetKey(janela, GLFW_KEY_Q) == GLFW_PRESS) {
            novaColuna--;
            ultimaAnimacao = &spriteEsquerda;
            moveu = true;
        } else if (glfwGetKey(janela, GLFW_KEY_C) == GLFW_PRESS) {
            novaColuna++;
            ultimaAnimacao = &spriteDireita;
            moveu = true;
        } else if (glfwGetKey(janela, GLFW_KEY_W) == GLFW_PRESS) {
            novaLinha--;
            novaColuna--;
            ultimaAnimacao = &spriteCosta;
            moveu = true;
        } else if (glfwGetKey(janela, GLFW_KEY_S) == GLFW_PRESS) {
            novaLinha++;
            novaColuna++;
            ultimaAnimacao = &spriteFrente;
            moveu = true;
        } else if (glfwGetKey(janela, GLFW_KEY_A) == GLFW_PRESS) {
            novaLinha++;
            novaColuna--;
            ultimaAnimacao = &spriteEsquerda;
            moveu = true;
        } else if (glfwGetKey(janela, GLFW_KEY_D) == GLFW_PRESS) {
            novaLinha--;
            novaColuna++;
            ultimaAnimacao = &spriteDireita;
            moveu = true;
        }

        if (moveu && !tilemap->isBloqueado(novaLinha, novaColuna)) {
            // Verifica se pisou em um tile diferente
            if (novaLinha != ultimaLinha || novaColuna != ultimaColuna) {
                // Verifica o tile atual
                int tileAtual = tilemap->getTile(novaLinha, novaColuna);

                // Lógica para tiles especiais
                if (tileAtual == 0) {
                    moedasTotal++;
                    std::cout << "Voce coletou uma moeda! Total: " + std::to_string(moedasTotal) << std::endl;
                    tilemap->setTile(novaLinha, novaColuna, 1);
                } else if (tileAtual == 3) {
                    std::cout << "Morreu!(Lava)" << std::endl;
                    novaLinha = 7;
                    novaColuna = 7;
                    moedasTotal = 0;
                } else if (tileAtual == 6) {
                    std::cout << "Vitoria! Total de moedas: " + std::to_string(moedasTotal) << std::endl;
                    novaLinha = 7;
                    novaColuna = 7;
                    moedasTotal = 0;
                }

                // Atualiza a última posição
                ultimaLinha = novaLinha;
                ultimaColuna = novaColuna;
            }

            // Atualiza a posição do jogador
            linha = novaLinha;
            coluna = novaColuna;
            spriteAtual = ultimaAnimacao;
            spriteAtual->atualizar(deltaTime);
            ultimoTempoInput = tempoAtual;
        }
    }

    // Lógica para animação idle
    if (!moveu) {
        if (ultimaAnimacao == &spriteFrente) spriteAtual = &spriteFrenteIdle;
        else if (ultimaAnimacao == &spriteCosta) spriteAtual = &spriteCostaIdle;
        else if (ultimaAnimacao == &spriteEsquerda) spriteAtual = &spriteEsquerdaIdle;
        else if (ultimaAnimacao == &spriteDireita) spriteAtual = &spriteDireitaIdle;
    }
}

void CharacterController::desenhar() {
    float halfTileW = 57 / 2.0f;
    float halfTileH = 57 / 4.0f;
    float offsetX = 400;
    float offsetY = 100;

    glm::vec2 posIso(
        (coluna - linha) * halfTileW + offsetX,
        (coluna + linha) * halfTileH + offsetY
    );

    glm::vec2 escala = (spriteAtual == &spriteFrenteIdle || spriteAtual == &spriteCostaIdle ||
                        spriteAtual == &spriteEsquerdaIdle || spriteAtual == &spriteDireitaIdle)
                        ? glm::vec2(70.0f, 70.0f)
                        : glm::vec2(64.0f, 64.0f);

    spriteAtual->desenhar(posIso, escala);
}
