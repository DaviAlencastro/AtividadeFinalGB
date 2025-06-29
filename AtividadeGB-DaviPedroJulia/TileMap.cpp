#include "TileMap.h"
#include <fstream>
#include <sstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TileMap::TileMap(const std::string& mapaPath, const std::string& tilesetPath,
                 int linhas, int colunas, int tileW, int tileH, GLuint shader)
    : linhas(linhas), colunas(colunas), tileWidth(tileW), tileHeight(tileH), shader(shader)
{
    carregarMapa(mapaPath);
    carregarTileset(tilesetPath);
    configurarOpenGL();
}


// Modificar a função carregarMapa
void TileMap::carregarMapa(const std::string& path) {
    std::ifstream arquivo(path);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o mapa: " << path << std::endl;
        return;
    }

    std::string nomeTileset;
    int qtdTiles;
    int tileW, tileH;
    int lin, col;

    // Lê header
    arquivo >> nomeTileset >> qtdTiles >> tileW >> tileH;
    arquivo >> col >> lin;

    // Atualiza propriedades internas
    this->tileSize = tileW;
    this->linhas = lin;
    this->colunas = col;

    // Lê matriz
    mapa.resize(linhas, std::vector<int>(colunas, 0));
    bloqueado.resize(linhas, std::vector<bool>(colunas, false));

    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            int tile;
            if (!(arquivo >> tile)) {
                std::cerr << "Erro lendo o valor do tile (" << i << "," << j << ")\n";
                continue;
            }
            mapa[i][j] = tile;
            //Bloqueia tile
            bloqueado[i][j] = (tile == 4);
        }
    }
}

int TileMap::getTile(int i, int j) {
    if (i < 0 || j < 0 || i >= linhas || j >= colunas) return -1;
    return mapa[i][j];
}


void TileMap::carregarTileset(const std::string& caminho) {
    glGenTextures(1, &tilesetID);
    glBindTexture(GL_TEXTURE_2D, tilesetID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int largura, altura, canais;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* dados = stbi_load(caminho.c_str(), &largura, &altura, &canais, STBI_rgb_alpha);
    if (!dados) {
        std::cerr << "Erro ao carregar tileset: " << caminho << std::endl;
        return;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, largura, altura, 0, GL_RGBA, GL_UNSIGNED_BYTE, dados);
    stbi_image_free(dados);

    tilesPorLinha = 7;
    tilesPorColuna = 1;

}

void TileMap::configurarOpenGL() {


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

int playerI = -1, playerJ = -1;
float currentDarken = 0.0f;

void TileMap::setPlayerPosition(int i, int j) {
    playerI = i;
    playerJ = j;
}

void TileMap::setDarkenFactor(float factor) {
    currentDarken = factor;
}

void TileMap::setTile(int i, int j, int novoValor) {
    if (i >= 0 && i < linhas && j >= 0 && j < colunas) {
        mapa[i][j] = novoValor;
        bloqueado[i][j] = (novoValor == 2);
    }
}
void TileMap::desenhar(int centroI, int centroJ) {
    glUseProgram(shader);
    glBindTexture(GL_TEXTURE_2D, tilesetID);
    glBindVertexArray(VAO);
    // Envia a posição do jogador para o shader
    glUniform2f(glGetUniformLocation(shader, "playerTileCoord"), playerI, playerJ);
    glUniform1f(glGetUniformLocation(shader, "darkenFactor"), currentDarken);

    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            glUniform2f(glGetUniformLocation(shader, "currentTileCoord"), i, j);
            int id = mapa[i][j];

            float tx = (id % tilesPorLinha) * (1.0f / tilesPorLinha);
            float ty = 0.0f;
            float scaleX = 1.0f / tilesPorLinha;
            float scaleY = 1.0f;

            float isoX = (j - i) * (tileSize / 2.0f) + 400;
            float isoY = (j + i) * (tileSize / 4.0f) + 100;

            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(isoX, isoY, 0.0f));
            model = glm::scale(model, glm::vec3(tileSize, tileSize / 2.0f, 1.0f));

            glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniform2f(glGetUniformLocation(shader, "offset"), tx, ty);
            glUniform2f(glGetUniformLocation(shader, "scale"), scaleX, scaleY);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    glBindVertexArray(0);
}



bool TileMap::isBloqueado(int i, int j) {
    if (i < 0 || j < 0 || i >= linhas || j >= colunas) return true;
    return bloqueado[i][j];
}
