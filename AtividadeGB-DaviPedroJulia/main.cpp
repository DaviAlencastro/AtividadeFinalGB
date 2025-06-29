#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TileMap.h"
#include "CharacterController.h"

#include <iostream>

const int LARGURA = 800;
const int ALTURA = 600;

GLuint criarShader(const char* vs, const char* fs);

const char* vs = R"(
#version 330 core
layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 tex;

uniform mat4 model;
uniform mat4 projection;
uniform vec2 offset;
uniform vec2 scale;

out vec2 TexCoord;

void main() {
    gl_Position = projection * model * vec4(pos, 0.0, 1.0);
    TexCoord = tex * scale + offset;
})";

const char* fs = R"(
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D imagem;
uniform vec2 playerTileCoord;  // Coordenada do tile do jogador
uniform vec2 currentTileCoord; // Coordenada do tile atual sendo desenhado
uniform float darkenFactor;    // Fator de escurecimento (0 = normal, 1 = totalmente escuro)
void main() {
    vec4 color = texture(imagem, TexCoord);

    // Se estiver no mesmo tile que o jogador, aplica o efeito
    if (distance(playerTileCoord, currentTileCoord) < 0.1) {
        color.rgb *= (1.0 - darkenFactor); // Escurece a cor
    }

    FragColor = color;
})";

int main() {
    if (!glfwInit()) {
        std::cerr << "Erro ao iniciar GLFW\n";
        return -1;
    }

    GLFWwindow* janela = glfwCreateWindow(LARGURA, ALTURA, "Trabalho GB", NULL, NULL);
    if (!janela) {
        std::cerr << "Erro ao criar janela GLFW\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(janela);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Erro ao iniciar GLEW\n";
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint shader = criarShader(vs, fs);
    glUseProgram(shader);

    glm::mat4 proj = glm::ortho(0.0f, float(LARGURA), float(ALTURA), 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
    glUniform1i(glGetUniformLocation(shader, "imagem"), 0);

    // Cria tilemap e personagem
    TileMap tilemap("mapa.txt", "imagens/tilesetIso.png", 15, 15, 114, 57, shader);
    CharacterController player(janela, shader, &tilemap);


    float tempoAnterior = glfwGetTime();

    while (!glfwWindowShouldClose(janela)) {
        float tempoAtual = glfwGetTime();
        float delta = tempoAtual - tempoAnterior;
        tempoAnterior = tempoAtual;

        glfwPollEvents();

        player.atualizar(delta);

        glClearColor(0.2f, 0.2f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        tilemap.desenhar(player.getI(), player.getJ());
        player.desenhar();

        glfwSwapBuffers(janela);
    }

    glfwTerminate();
    return 0;
}

GLuint criarShader(const char* vs, const char* fs) {
    GLuint v = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v, 1, &vs, NULL);
    glCompileShader(v);

    GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f, 1, &fs, NULL);
    glCompileShader(f);

    GLuint p = glCreateProgram();
    glAttachShader(p, v);
    glAttachShader(p, f);
    glLinkProgram(p);
    glDeleteShader(v);
    glDeleteShader(f);
    return p;

}
