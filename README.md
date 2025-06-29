# Jogo com Tilemap Isométrico em C++ e OpenGL

Este é um projeto de jogo 2D com visual isométrico desenvolvido em **C++**, utilizando as bibliotecas **OpenGL**, **GLFW**, **GLEW** e **GLM**. O jogador pode se movimentar por tiles, coletar moedas, evitar armadilhas (lava), e alcançar um tile de vitória.

## Requisitos

Antes de compilar, certifique-se de ter as seguintes bibliotecas instaladas:

- **OpenGL** (>= 3.3)
- **GLFW**
- **GLEW**
- **GLM**
- **stb_image** (incluso no projeto)

No Windows, recomenda-se usar um ambiente como MinGW ou MSVC e configurar as bibliotecas manualmente.

## Compilação

### Windows (MinGW, usando g++)

Certifique-se de ter as DLLs e includes corretamente configurados. Exemplo:

```bash
g++ main.cpp SpriteAnimado.cpp TileMap.cpp CharacterController.cpp -o jogo.exe ^
  -lglew32 -lglfw3 -lopengl32 -lgdi32 -Iinclude
```

## Execução

Após compilar, basta executar:

```bash
jogo.exe   # No Windows
```
## Controles

A movimentação é baseada em diagonais isométricas com as seguintes teclas:

```
Q W E
A   D
Z S C
```

Cada tecla move o personagem em uma das 8 direções possíveis.

## Lógica do Jogo

- **Objetivo:** Coletar o máximo de moedas (`tile 0`) e alcançar o tile de vitória (`tile 6`) sem morrer.
- **Perigo:** Se o jogador pisar em um `tile 3` (lava), morre e reinicia.
- **Tiles bloqueados:** `tile 4` (água) impede movimentação.
- A posição inicial é o tile `5`.


## Dependências

O projeto utiliza:

- [`stb_image`](https://github.com/nothings/stb) para carregamento de imagens PNG
- [`GLFW`](https://www.glfw.org/) para criação de janelas e gerenciamento de input
- [`GLEW`](http://glew.sourceforge.net/) para funções OpenGL modernas
- [`GLM`](https://glm.g-truc.net/) para operações com matrizes e vetores

## Autores

Trabalho desenvolvido por:

- **Davi Alencastro**
- **Júlia Oliveira**
- **Pedro Roza**

---

> Projeto apresentado na disciplina de **Processamento Gráfico** (Unisinos) — 2025.
