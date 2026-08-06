# 🏓 Pong 2026

Um remake do clássico **Pong**, desenvolvido em **C** utilizando a biblioteca **SDL2** como projeto de estudo da disciplina de Programação da graduação em Ciência da Computação.

O objetivo deste projeto é aplicar conceitos fundamentais de desenvolvimento de jogos, programação estruturada, manipulação de eventos, renderização gráfica e organização de código em C.

> **🚧 Projeto em desenvolvimento**
>
> Novas funcionalidades estão sendo implementadas continuamente.

---

## Demonstração

*Em breve será adicionado um vídeo demonstrando a gameplay e as principais funcionalidades do jogo.*

---

## Funcionalidades atuais

* Introdução animada com o título **PONG 2026**
* Movimento das raquetes por dois jogadores
* Colisão entre bola e raquetes
* Rebote nas bordas superior e inferior
* Reinício automático da bola após cada ponto
* Renderização utilizando SDL2
* Código organizado em funções para facilitar manutenção e evolução

---

## Próximas funcionalidades

* Placar da partida
* Efeitos sonoros
* Tela de Game Over
* Melhorias visuais
* Refatoração em módulos independentes

---

## Tecnologias utilizadas

* Linguagem C
* SDL2
* SDL2_ttf
* SDL2_gfx

---

## Estrutura do projeto

```text
pong/
├── assets/
│   └── fonts/
├── intro.c
├── intro.h
├── main.c
└── README.md
```

---

## Compilação

Instale as dependências:

```bash
sudo apt install libsdl2-dev libsdl2-ttf-dev libsdl2-gfx-dev
```

Compile o projeto:

```bash
gcc main.c intro.c -o pong \
$(sdl2-config --cflags --libs) \
-lSDL2_ttf \
-lSDL2_gfx
```

Execute:

```bash
./pong
```

---

## Objetivos de aprendizagem

Este projeto foi desenvolvido com foco no estudo de:

* Programação em C
* Estruturas de dados e manipulação de memória
* Programação orientada a eventos
* Desenvolvimento de jogos com SDL2
* Organização e refatoração de código
* Uso de bibliotecas externas

---

## Autor

**Fabiano Cunha**

Graduando em Ciência da Computação.

GitHub: https://github.com/oanbifa-del
