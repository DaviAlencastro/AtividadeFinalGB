#ifndef TILEMAP_H
#define TILEMAP_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class TileMap {
public:
    TileMap(const std::string& mapaPath, const std::string& tilesetPath,
        int linhas, int colunas, int tileW, int tileH, GLuint shader);

    void setPlayerPosition(int i, int j);
    void setDarkenFactor(float factor);
    void setTile(int linha, int coluna, int novoValor);
    void desenhar(int centroI, int centroJ);
    bool isBloqueado(int linha, int coluna);
    int getTile(int linha, int coluna);

private:
    void carregarMapa(const std::string& path);
    void carregarTileset(const std::string& caminho);
    void configurarOpenGL();



    GLuint tilesetID;
    GLuint VAO, VBO;
    GLuint shader;
    int tileWidth, tileHeight;

    int linhas, colunas;
    int tileSize;
    int tilesPorLinha, tilesPorColuna;

    std::vector<std::vector<int>> mapa;
    std::vector<std::vector<bool>> bloqueado;
};

#endif
