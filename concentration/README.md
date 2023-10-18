
# - Jogo de Memória -

# Nome: Ana Luiza Ferreira Figueiredo
# RA: 11121015
Este é um projeto desenvolvido para a disciplina DA1MCTA008-17SA - Computação Gráfica, implementado usando a biblioteca [abcg](https://github.com/amidos2006/abcg).

## Instruções

Para jogar o jogo, execute o programa em `abcg\build\bin\concentration` e siga as instruções na interface do usuário. O jogador deve virar as cartas e encontrar todos os pares para vencer o jogo.

## Demonstração

Tive problemas para utilizar o WebAssembly, então fiz uma demonstração do jogo rodando em vídeo.

## Descrição

O projeto consiste em um jogo de memória onde o jogador deve encontrar todos os pares de cartas, clicando para virá-las e comparando seus valores. O jogo é composto por várias classes que controlam a lógica do jogo e a interface do usuário.

### `main.cpp`

`main.cpp` é o ponto de entrada do jogo. Ele cria uma instância da classe `abcg::Application`, inicializa e configura a janela do jogo e inicia o loop principal do jogo.

### `Card.cpp` e `Card.hpp`

`Card.cpp` e `Card.hpp` definem a classe `Card`, que representa uma carta do jogo de memória. Cada carta tem um valor e um estado (virada para cima ou para baixo). A classe `Card` inclui métodos para acessar o valor da carta (`getValue()`), verificar o estado da carta (`isFaceUp()`) e virar a carta (`flip()`). Para permitir comparar cartas, também foi implementado o operador `==`, que retorna TRUE se elas tiverem o mesmo valor e FALSE em caso contrário.

### `Window.cpp` e `Window.hpp`

`Window.cpp` e `Window.hpp` contêm a classe `Window`, que representa a janela principal do jogo. A classe `Window` inclui a lógica do jogo, como criar o tabuleiro de cartas, gerenciar estados de jogo e verificar se o jogo foi vencido. Além disso, a classe `Window` cuida da interface do usuário, permitindo que o jogador clique nas cartas e reinicie o jogo.

#### Atributos

- `m_N`: Tamanho do tabuleiro do jogo, que é uma matriz com `m_N` linhas e `m_N` colunas.
- `m_matches`: O número de pares de cartas encontrados. Utilizado para verificar se o jogo foi ganho.
- `m_gameState`: O estado atual do jogo (Play, Win ou Wait).
    - `Play`: O jogador está tentando encontrar pares de cartas.
    - `Win`: O jogador venceu o jogo encontrando todos os pares.
    - `Wait`: O jogador fez uma jogada incorreta e está aguardando antes de continuar. Existe para que o jogador tenha tempo de ver as duas cartas viradas antes que o jogo as desvire.
- `m_noneRevealed`: Uma flag que controla se nenhuma carta foi virada ou se uma carta já foi virada. Utilizada para controlar as rodadas do jogo.
- `current_card_offset` e `last_card_offset`: Guardam os índices da carta atualmente virada e da última carta virada. Utilizados para poder comparar as cartas.
- `m_board`: Vetor de objetos `Card` que representa o tabuleiro do jogo.
- `m_font`: Fonte usada no jogo.

#### Métodos

- `void restartGame()`: Inicializa o jogo, criando pares de cartas com valores únicos,  embaralhando-os e distribuindo as cartas no tabuleiro.
    - Aqui são criados os possíveis valores das cartas (letras de A a Z)
    ```c++
        std::vector<std::string> uniqueCardValues;

    // Populate the vector with unique card values.
    for (char c = 'A'; c <= 'Z'; ++c) {
        uniqueCardValues.push_back(std::string(1, c));  // Add uppercase letters as values.
    }

    // Shuffle the vector of card values to randomize their order.
    std::shuffle(uniqueCardValues.begin(), uniqueCardValues.end(), std::mt19937(std::random_device()()));
    ```
    - Para o caso de ser um novo jogo após um outro, o vetor do tabuleiro é limpo com `m_board.clear();`
    - Repete o processo de: escolher valores aleatórios, criar os pares de cartas e adicionar ao tabuleiro, até que tenha criado todas as cartas necessárias
    ```c++
    for (size_t i = 0; i < m_N * m_N / 2; ++i) {
        const std::string value = uniqueCardValues[i];

        // Create two Card objects with the same value.
        Card card1(value);
        Card card2(value);

        // Add the Card objects to the game board.
        m_board.push_back(card1);
        m_board.push_back(card2);
    }
    ```
    - Como cada par de cartas foi adicionado em ordem, é preciso embaralhar as cartas no vetor do tabuleiro, usando `std::shuffle(m_board.begin(), m_board.end(), std::mt19937(std::random_device()()));`
    - Por fim, as variáveis usadas no controle do jogo são reiniciadas
    ```c++
    m_matches = 0;
    m_noneRevealed = true;
    m_gameState = GameState::Play;
    ```
- `void onCreate()`: Quando a janela é criada, carrega a fonte e inicia o jogo chamando a função `restartGame()`.
- `void onPaintUI()`: Método que trata dos eventos ligados à renderização. Desenha a interface do usuário do jogo, o tabuleiro de jogo, as cartas e os botões. Também trata os eventos do jogo.
    - Recebe o tamanho da tela
    ```c++
    auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};
    auto const appWindowHeight{gsl::narrow<float>(getWindowSettings().height)};
    ```
    - Cria os elementos visuais do jogo
        - Texto na janela
        ```c++
        auto const flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoCollapse};
        ImGui::Begin("Memory Match", nullptr, flags);
        ```
        - Menu com botão de reiniciar
        ```c++
        {
        bool restartSelected{};
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Game")) {
            ImGui::MenuItem("Restart", nullptr, &restartSelected);
            ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        if (restartSelected) {
            restartGame();
        }
        }
        ```
        - Texto de instruções que reage ao estado do jogo
        ```c++
        std::string text;
        switch (m_gameState) {
        case GameState::Play:
            text = m_noneRevealed ? "Clique numa carta" : "Clique noutra carta";
            break;
        case GameState::Win:
            text = "Player wins!";
            break;
        case GameState::Wait:
            text = "Errou! Clique em qualquer carta para desvirar.";
            break;
        }
        ImGui::SetCursorPosX(
            (appWindowWidth - ImGui::CalcTextSize(text.c_str()).x) / 2);
        ImGui::Text("%s", text.c_str());
        ImGui::Spacing();
        ```
        - Percorre o vetor do tabuleiro do jogo criando botões com o valor das cartas viradas e botões em branco para cartas sem virar.
        ```c++
        auto const gridHeight{appWindowHeight - 22 - 60 - (m_N * 10) - 60};
        auto const buttonHeight{gridHeight / m_N};

        std::string button_label;

        // Use custom font
        ImGui::PushFont(m_font);
        if (ImGui::BeginTable("Game board", m_N)) {
        for (auto i : iter::range(m_N)) {
            ImGui::TableNextRow();
            for (auto j : iter::range(m_N)) {
            ImGui::TableSetColumnIndex(j);
            auto const offset{i * m_N + j};

            // Replace null character with whitespace because the button label
            // cannot be an empty string
            if (m_board.at(offset).isFaceUp()) {
                button_label = m_board.at(offset).getValue();
            }
            else {
                button_label = " ";
            }
        ```
        - Há duas opções para quando os botões das cartas são clicados:
            - No estado de "Play" quando uma carta desvirada é clicada, se for a primeira virada, guarda a posição. Se não for, chama o método `checkEndCondition()` que verifica se ela é par da última virada e se cheou o fim do jogo.
            - No estado de `Wait` (ou seja, se não foi encontrado o par) quando qualquer carta é clicada, as cartas viradas são desviradas e o jogo volta ao estado de `Play`. 
        ```c++
        auto buttonText{fmt::format("{}##{}{}", button_label, i, j)};
        if (ImGui::Button(buttonText.c_str(), ImVec2(-1, buttonHeight))) {
            if (m_gameState == GameState::Play && button_label == " ") {
            
            // Flip
            m_board.at(offset).flip();

            if (m_noneRevealed) {
                current_card_offset = offset;
            } else {
                last_card_offset = current_card_offset;
                current_card_offset = offset;
                checkEndCondition();
            }
            m_noneRevealed = !m_noneRevealed;
            }
            else if (m_gameState == GameState::Wait) {
            m_board.at(current_card_offset).flip();
            m_board.at(last_card_offset).flip();
            m_gameState = GameState::Play;
            }
        }
        ```
        - Cria o botão de reiniciar no pé da janela
        ```c++
        if (ImGui::Button("Restart game", ImVec2(-1, 50))) {
            restartGame();
        }
        ```
- `bool checkEndCondition()`: Verifica se o jogo terminou, se todas as cartas foram encontradas e atualiza o estado do jogo. Retorna `true` quando o jogo é vencido.
    - Só faz sentido se o estado do jogo estiver em `Play`
    ```c++
    if (m_gameState != GameState::Play) {
        return false;
    }
    ```
    - Se as cartas viradas forem de mesmo valor, adiciona um ponto no contador de pares e verifica se todos foram encontrados. Se sim, muda o estado para `Win`
    ```c++
    if (m_board.at(current_card_offset) == m_board.at(last_card_offset)) {
        m_matches += 1;
        if (m_matches == m_N*2) {
        m_gameState = GameState::Win;
        return true;
        }
    }
    ```
    - Se as cartas viradas forem diferentes, muda o estado do jogo para `Wait`, para que o jogador possa ver as cartas e o jogo saiba que deve desvirá-las.
    ```c++
    else {
        m_gameState = GameState::Wait;
    }
    return false;
    ```