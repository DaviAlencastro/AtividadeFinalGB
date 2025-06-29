#include "ItemVisual.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "include/stb_image.h"
#include <iostream>

ItemVisual::ItemVisual(const std::string& imagemPath, GLuint shaderID, int tileSize)
    : shader(shaderID), tileSize(tileSize) {

    glGenTextures(1, &texturaID);
    glBindTexture(GL_TEXTURE_2D, texturaID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int largura, altura, canais;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* dados = stbi_load(imagemPath.c_str(), &largura, &altura, &canais, STBI_rgb_alpha);
    if (!dados) {
        std::cerr << "Erro ao carregar imagem do item: " << imagemPath << std::endl;
        return;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, largura, altura, 0, GL_RGBA, GL_UNSIGNED_BYTE, dados);
    stbi_image_free(dados);

    float vertices[] = {
        -0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  1.0f, 0.0f,

        -0.5f,  0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  1.0f, 1.0f
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

ItemVisual::~ItemVisual() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texturaID);
}

void ItemVisual::desenhar(glm::vec2 posicao) {
    glUseProgram(shader);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(posicao.x, posicao.y - tileSize * 0.2f, 0.0f));
    model = glm::scale(model, glm::vec3(tileSize * 0.6f, tileSize * 0.6f, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform2f(glGetUniformLocation(shader, "offset"), 0.0f, 0.0f);
    glUniform2f(glGetUniformLocation(shader, "scale"), 1.0f, 1.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texturaID);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
