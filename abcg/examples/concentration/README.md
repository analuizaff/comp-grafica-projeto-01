# RA XXXXXXXX - Jogo de Memória

Este é um projeto desenvolvido para a disciplina DA1MCTA008-17SA - Computação Gráfica, implementado usando a biblioteca [abcg](https://github.com/amidos2006/abcg).

## Descrição

O projeto consiste em um jogo de memória onde o jogador deve encontrar todos os pares de cartas, clicando para virá-las e comparando seus valores. O jogo é composto por várias classes que controlam a lógica do jogo e a interface do usuário.

## Classes Principais

### `Card.cpp` e `Card.hpp`

`Card.cpp` e `Card.hpp` definem a classe `Card`, que representa uma carta do jogo de memória. Cada carta tem um valor e um estado (virada para cima ou para baixo). A classe `Card` inclui métodos para acessar o valor da carta, verificar o estado da carta e virar a carta. Também foi implementado o operador `==`, para permitir comparar cartas.

#### Descrição

O arquivo `Card.cpp` contém a implementação da classe `Card`, que possui métodos para representar uma carta em um jogo. A classe `Card` inclui as seguintes funcionalidades:

- Construtores para criar objetos `Card` com um valor específico e com a face virada para cima ou para baixo.
- Métodos para obter o valor da carta e verificar se a face está virada para cima.
- Um método para virar a carta, alternando o estado da face.
- Um operador de igualdade para comparar cartas com base em seus valores.

### `Window.cpp` e `Window.hpp`

`Window.cpp` e `Window.hpp` contêm a classe `Window`, que representa a janela principal do jogo. A classe `Window` inclui a lógica do jogo, como criar o tabuleiro de cartas, gerenciar estados de jogo e verificar se o jogo foi vencido. Além disso, a classe `Window` cuida da interface do usuário, permitindo que o jogador clique nas cartas e reinicie o jogo.

#### Membros Importantes

- `m_N`: Tamanho da placa de jogo, que é uma matriz `m_N x m_N`.
- `m_matches`: O número de pares de cartas encontrados.
- `m_gameState`: O estado atual do jogo (Play, Win ou Wait).
- `m_noneRevealed`: Uma flag que controla se nenhuma carta foi virada ou se uma carta já foi virada.
- `current_card_offset` e `last_card_offset`: Guardam os índices das cartas atualmente viradas e da última carta virada.
- `m_board`: Vetor de objetos `Card` que representa o tabuleiro do jogo.
- `m_font`: Uma fonte personalizada usada para as cartas.

#### Funções Principais

- `void restartGame()`: Inicializa o jogo, criando um vetor de valores de cartas únicos, embaralhando-o e distribuindo pares de cartas na placa de jogo.
- `void onCreate()`: Carrega uma fonte com um tamanho maior para as cartas e inicia o jogo chamando a função `restartGame()`.
- `void onPaintUI()`: Desenha a interface do usuário do jogo, incluindo o tabuleiro de jogo, as cartas e os botões.

#### Fluxo do Jogo

O jogo possui estados diferentes:
- `Play`: O jogador está tentando encontrar pares de cartas.
- `Win`: O jogador venceu o jogo encontrando todos os pares.
- `Wait`: O jogador fez uma jogada incorreta e está aguardando antes de continuar.

#### Funções Importantes

- `bool checkEndCondition()`: Verifica se o jogo terminou, se todas as cartas foram encontradas e atualiza o estado do jogo.
- `void restartGame()`: Reinicia o jogo, criando um novo tabuleiro e reiniciando as variáveis.

### `main.cpp`

`main.cpp` é o ponto de entrada do jogo. Ele cria uma instância da classe `abcg::Application`, inicializa a janela do jogo, configura as configurações da janela e inicia o loop principal do jogo.

## Instruções

Para jogar o jogo, execute o programa em `abcg\build\bin\concentration` e siga as instruções na interface do usuário. O jogador deve virar as cartas e encontrar todos os pares para vencer o jogo.

