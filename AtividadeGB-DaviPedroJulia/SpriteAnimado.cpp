#include "SpriteAnimado.h"
#include "include/stb_image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

SpriteAnimado::SpriteAnimado(const std::string& caminho, int linhas, int colunas)
    : numLinhas(linhas), numColunas(colunas), frameAtual(0), animacaoAtual(0), tempoAcumulado(0.0f), tempoPorFrame(0.15f), shader(0)
{
    glGenTextures(1, &texturaID);
    glBindTexture(GL_TEXTURE_2D, texturaID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int largura, altura, canais;
    stbi_set_flip_vertically_on_load(false);
    unsigned char* dados = stbi_load(caminho.c_str(), &largura, &altura, &canais, STBI_rgb_alpha);
    if (!dados) {
        std::cerr << "Erro ao carregar imagem: " << caminho << std::endl;
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, largura, altura, 0, GL_RGBA, GL_UNSIGNED_BYTE, dados);
        stbi_image_free(dados);
    }

    float ds = 1.0f / numColunas;
    float dt = 1.0f / numLinhas;

    float vertices[] = {
        -0.5f,  0.5f,   0.0f, dt,
        -0.5f, -0.5f,   0.0f, 0.0f,
         0.5f, -0.5f,   ds,   0.0f,

        -0.5f,  0.5f,   0.0f, dt,
         0.5f, -0.5f,   ds,   0.0f,
         0.5f,  0.5f,   ds,   dt,
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SpriteAnimado::~SpriteAnimado() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texturaID);
}

void SpriteAnimado::setShader(GLuint shaderID) {
    shader = shaderID;
}

void SpriteAnimado::setAnimacao(int linha) {
    if (linha >= 0 && linha < numLinhas) {
        animacaoAtual = linha;
    }
}

void SpriteAnimado::atualizar(float deltaTime) {
    tempoAcumulado += deltaTime;
    if (tempoAcumulado >= tempoPorFrame) {
        tempoAcumulado = 0.0f;
        frameAtual = (frameAtual + 1) % numColunas;
    }
}

void SpriteAnimado::desenhar(glm::vec2 posicao, glm::vec2 escala) {
    if (!shader) return;

    glUseProgram(shader);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(posicao, 0.0f));
    model = glm::scale(model, glm::vec3(escala, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));

    float ds = 1.0f / numColunas;
    float dt = 1.0f / numLinhas;
    float offsetS = frameAtual * ds;
    float offsetT = animacaoAtual * dt;
    glUniform2f(glGetUniformLocation(shader, "offset"), offsetS, offsetT);
    glUniform2f(glGetUniformLocation(shader, "scale"), ds, dt);

    glBindTexture(GL_TEXTURE_2D, texturaID);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
