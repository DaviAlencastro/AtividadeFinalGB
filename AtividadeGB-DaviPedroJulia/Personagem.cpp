#include "C:\Users\Davi\Documentos\OpenGL\AtividadeGB-DaviPedroJulia\Personagem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Personagem::Personagem(const std::string& pathIdle, const std::string& pathWalk, GLuint shaderID)
    : spriteIdle(pathIdle, 1, 1),
      spriteWalk(pathWalk, 1, 4),
      posicao(400.0f, 300.0f),
      velocidade(100.0f),
      andando(false) {

    spriteIdle.setShader(shaderID);
    spriteWalk.setShader(shaderID);
    spriteAtual = &spriteIdle;
}

void Personagem::processarInput(GLFWwindow* janela, float deltaTime) {
    glm::vec2 direcao(0.0f);

    if (glfwGetKey(janela, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(janela, GLFW_KEY_UP) == GLFW_PRESS) {
        direcao.y -= 1.0f;
    }
    if (glfwGetKey(janela, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(janela, GLFW_KEY_DOWN) == GLFW_PRESS) {
        direcao.y += 1.0f;
    }
    if (glfwGetKey(janela, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(janela, GLFW_KEY_LEFT) == GLFW_PRESS) {
        direcao.x -= 1.0f;
    }
    if (glfwGetKey(janela, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(janela, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        direcao.x += 1.0f;
    }

    if (direcao != glm::vec2(0.0f)) {
        posicao += direcao * velocidade * deltaTime;
        spriteAtual = &spriteWalk;
        andando = true;
    } else {
        spriteAtual = &spriteIdle;
        andando = false;
    }

    spriteAtual->atualizar(deltaTime);
}

void Personagem::desenhar() {
    spriteAtual->desenhar(posicao);
}
