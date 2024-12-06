# Treinamento de atenção seletiva através de jogo 3D

- Lorena Silva Sampaio - 11201812025
- Pablo Renato Nunes dos Santos - 11202232225
- Jonas de Mello Oliveira - 11000116

![Gameplay GIF](giphy.gif) <!-- Substitua com o link para o GIF -->

Este projeto apresenta o jogo/aplicação 3D **Vôo dos Pássaro**, uma ferramenta desenvolvida após uma série de entrevistas empáticas com professores e com uma profissional da neuropsicologia, sua finalidade é treinar habilidades de atenção e concentração dos jogadores, desafiando-os a identificar e coletar objetos específicos em meio a um ambiente visual dinâmico. (No caso, pássaros brancos).

O repositório contém o código desenvolvido em C++ com OpenGL, SDL e ImGui. Na aplicação, o usuário precisa clicar no alvo correto (pássaro branco) enquanto evita clicar em objetos de distração (pássaros azuis). 
Cada acerto no alvo aumenta a pontuação, enquanto clicar em uma distração diminui a pontuação. O objetivo é conseguir a maior pontuação possível antes do tempo acabar.

## Tabela de Conteúdos

- [Motivação do Projeto](#motivação-do-projeto)
  - [Entrevistas com Educadores](#entrevistas-com-educadores)
  - [Consultoria Neuropsicológica](#consultoria-neuropsicológica)
- [Objetivo e Justificativa](#objetivo-e-justificativa)
- [Funcionalidades](#funcionalidades)
- [Técnicas Implementadas](#técnicas-implementadas)
  - [Renderização de Objetos com OpenGL](#renderização-de-objetos-com-opengl)
  - [Interface de Usuário com ImGui](#interface-de-usuário-com-imgui)
  - [Interação do Usuário com SDL](#interação-do-usuário-com-sdl)
  - [Controle de Tempo e Estado do Jogo](#controle-de-tempo-e-estado-do-jogo)
  - [Estrutura do Código](#estrutura-do-código)
  - [Melhorias Futuras](#melhorias-futuras)
  - [Correções Necesárias](#correções-necesárias)
- [Instalação e Execução](#instalação-e-execução)
- [Estrutura do Código](#estrutura-do-código)
- [Referências](#referências)

## Motivação do Projeto

O projeto é uma continuação em 3D do projeto da Atividade 1 (Treinamente de Atenção Seletiva), que foi desenvolvido em 2D.

## Funcionalidades

- **Treinamento de Atenção**: Foco em clicar no alvo correto (um objeto 3D) que movimentan em um espaço 3D, enquanto evitar clicar nos objetos incorretos.
- **Contador de Tempo**: O usuário tem tempo limitado para acumular a maior pontuação possível.
- **Sistema de Pontuação**: A pontuação aumenta ao clicar no alvo correto e diminui ao clicar nas distrações.
- **Estados do Jogo**: Início, jogando, vitória e game over.

## Estrutura do Código

- **`main.cpp`**: Arquivo principal responsável pela inicialização do jogo e da interface gráfica.
- **`window.cpp`**: Gerencia a configuração da janela e a renderização dos elementos do jogo.
- **`window.hpp`**: Define a classe `Window`, que controla a janela do jogo e suas interações.
- **`model.cpp`**: Gerencia a renderização dos modelos 3D do jogo.
- **`model.hpp`**: Define a classe `Model`, que carrega e renderiza os modelos 3D.
- **`game_objects.cpp`**: Gerencia a renderização e interação dos objetos do jogo.
- **`game_objects.hpp`**: Define as classes que representam os objetos do jogo (alvo e distrações).
- **`gamedata.hpp`**: Define a estrutura `GameData`, que armazena os estados e informações relevantes do jogo.	

## Técnicas Implementadas

O desenvolvimento se deu com várias técnicas de computação gráficas explicitadas abaixo:

### Interface de Usuário com ImGui

Na interface do usuário (UI), **ImGui** é mostrado:
- **Pontuação e Tempo**: Um contador de tempo e pontuação são exibidos na tela, atualizados em tempo real, ajudando o jogador a acompanhar seu progresso.
- **Estados do Jogo**: Mensagens informativas são exibidas dependendo do estado do jogo:
  - **Start**: Orienta o jogador a pegar o alvo correto.
  - **Playing**: Exibe o tempo e a pontuação enquanto o jogo está ativo.
  - **Game Over**: Mostra a pontuação final quando o tempo acaba. 
  - **Win**: Exibe uma mensagem de vitória se o jogador atingir uma nova pontuação recorde. //Esse estado ainda apresenta bugs para ser apresentado.

### Interação do Usuário com SDL

Para detectar cliques do mouse e toques na tela, o jogo utiliza **SDL**:
- **SDL_MOUSEBUTTONDOWN** e **SDL_FINGERDOWN**: Eventos capturam a posição de cliques e toques, permitindo que o jogador interaja com o jogo.
- **Conversão de Coordenadas**: As posições de clique são convertidas para coordenadas normalizadas, permitindo uma fácil verificação de colisão entre o clique e os objetos na tela.
- **Interatividade**: Ao clicar no alvo correto, a pontuação aumenta, enquanto clicar em uma distração resulta em perda de pontos.
- **Colisões**: A função `checkCollision()` verifica se a posição do clique corresponde à posição do alvo, permitindo que o jogador colete o alvo correto e aumente sua pontuação.
- **Transformações**: As posições dos objetos são recalculadas periodicamente, com posições aleatórias a cada nova rodada, utilizando-se diversas funções.

### Controle de Tempo e Estado do Jogo

O estado do jogo e o cronômetro são geridos utilizando a biblioteca **chrono** do C++:
- **Cronômetro**: Cada segundo, o tempo é decrementado. Ao atingir zero, o jogo termina.
- **Estados do Jogo**: São definidos vários estados (Start, Playing, Game Over, Win) que determinam a lógica e as mensagens exibidas ao jogador. A função `checkGameStatus()` atualiza o estado com base na pontuação e tempo restantes.
- **Pontuação**: A pontuação é atualizada com base nos cliques corretos e incorretos do jogador, aumentando ou diminuindo a pontuação conforme necessário.

### Renderização de Objetos com OpenGL

A renderização é feita usando **OpenGL**:
- **Shaders (texture.vert e texture.frag)**: O projeto utiliza o código de shaders "textura" baseado no viewer6 do material das aulas.
```
1. Blinn-Phong:
O programda de shader utilizado implementa um sistema de sombreamento usando o modelo Blinn-Phong para iluminação.

2. Mapeamento de Textura:
Utiliza-se também o mapeamento de textura em GLSL (OpenGL Shading Language). 

3. Vertex Shader: 
No vertex shader, as informações do vértice, como posição, normais e coordenadas de textura, são transformadas usando as matrizes modelo, visão e projeção, além de calcular vetores úteis para iluminação (como o vetor luz e o vetor para o observador). 

4. Fragment Shader:
As informações do vertex sahder são repassadas ao fragment shader para computar a cor de cada fragmento. Nesse, a iluminação é calculada usando o modelo Blinn-Phong, que considera componentes de luz ambiente, difusa e especular. Além disso, ele permite o uso de diferentes coordenadas de textura (planejamento, cilíndrico, esférico ou mapeamento da malha) que não aplicamos nesse projeto. 

5. Resultado:
O resultado é uma combinação de textura e iluminação física para gerar a cor final de cada pixel vista nos objetos (modelos de pássaro).

```
- **Modelo (model.hpp e model.cpp)**: O código manipula modelos 3D (.obj) com texturas e materiais (.mtl) para renderizar objetos 3D.
```
1. Definição de Vértices e Índices: 
Os vértices contêm posição, normais e coordenadas de textura. A estrutura de hash otimiza a busca por vértices únicos.

2. Cálculo de Normais:
Computa as normais dos vértices acumulando normais de suas faces adjacentes (através do produto vetorial de arestas).
Normais acumuladas são normalizadas para garantir consistência.

3. Cálculo de Tangentes:
Computa tangentes e bitangentes com base nas coordenadas de textura.
As tangentes são reortogonalizadas em relação às normais para evitar distorções.

4. Carregamento de Arquivos OBJ:
Usa tinyobjloader para processar modelos OBJ.
Gera vértices e índices e configura propriedades como texturas difusas e parâmetros de material.

5. Centralização e Normalização:
Redimensiona o modelo para que caiba em um cubo unitário e centraliza-o na origem, facilitando sua manipulação no espaço 3D.

6. Buffers (VBO, EBO, VAO):
createBuffers: Cria e preenche os buffers de vértices e índices.
setupVAO: Configura os atributos de vértice no Vertex Array Object (VAO), incluindo posição, normais, tangentes e coordenadas de textura.

7.Carregamento de Texturas:
loadDiffuseTexture: Carrega uma textura difusa (imagem) para aplicação nos modelos.
Os trecho comentados são para o uso futuro para texturas normais e cubo.

8. Renderização:
render: Renderiza o modelo configurando a textura, ativando o VAO e chamando glDrawElements para desenhar os triângulos.

9. Destruição:
Libera recursos como texturas, buffers e VAO quando o objeto é destruído.

```
- **Objetos do Jogo (game_objects.hpp e game_objects.cpp)**: O código gerencia a renderização e interação dos objetos do jogo.
```
1. Estrutura Principal
Objetos do jogo são representados por dois grupos:
    m_distractionObjects: Objetos que distraem o jogador (pássaros com penas azuis).
    m_targetObjects: Objetos que o jogador deve focar (pássaros com penas brancas).    
Shaders: 
    O programa de sombreamento (m_program) é configurado para aplicar texturas e iluminação aos objetos.
Modelos 3D: 
    Os modelos de distração e alvo são carregados a partir de arquivos .obj e associados às texturas.

2. Geração e Atualização de Objetos
setupSceneObjects: 
    Inicializa as posições e propriedades aleatórias dos objetos dentro de limites 3D definidos.
updateSceneObjects: 
    Atualiza a posição dos objetos com base no tempo (deltaTime) e velocidade. Se o objeto sair da área visível, ele é reposicionado aleatoriamente.

3. Renderização
paint: 
    Configura o ambiente de renderização, define variáveis uniformes para o shader e chama renderObject para desenhar os objetos na tela.
renderObject: 
    Aplica transformações como translação, escala e rotação antes de renderizar cada objeto.

4. Interação do Usuário
checkClickOnObject:
    Verifica se um clique do usuário atinge algum objeto na tela.
    Converte as coordenadas do objeto para o espaço da tela e compara com a posição do clique.
    Reposiciona o objeto atingido.

5. Interface do Usuário
paintUI:
    Cria uma interface gráfica usando ImGui para alterar o tipo de projeção (perspectiva ou ortográfica) e ajustar o campo de visão (baseado no projeto efeito Starfield do material das aulas).
```
- **Renderização de Janela (window.hpp e window.cpp)**: 
```
1. Configuração Inicial
A cor de fundo é definida (skyblue) utilizando a função glClearColor.
A fonte do texto é carregada a partir de um arquivo TTF.
Objetos do jogo são inicializados, e o jogo começa no estado inicial (GameState::Start).

2.Lógica de Jogo
Estados do Jogo: 
    O jogo pode estar nos estados Start, Playing, GameOver, ou Win.
Tempo e Pontuação:
    O temporizador decrementa a cada segundo enquanto o estado é Playing.
    A pontuação aumenta ou diminui com base em cliques corretos ou incorretos.
Fim do Jogo:
    O jogo termina (GameOver) quando o tempo chega a zero.
    Se a pontuação atual exceder o recorde anterior, o estado muda para Win.

3.Eventos de Interação
Clique do mouse: 
    Converte a posição do clique em coordenadas normalizadas e verifica se o jogador clicou em um alvo ou distração.

4.Renderização
Renderiza os objetos do jogo e uma interface do usuário usando ImGui para exibir a pontuação e o tempo restante.

5. Reinicialização
Funções como resetGame() e startGame() permitem reiniciar o jogo para o estado inicial, com objetos reposicionados aleatoriamente e variáveis reiniciadas.
```



### Melhorias Futuras`
- **Ajustar reconhecimento de cliques**: Melhorar a detecção de cliques para garantir que os cliques sejam registrados corretamente.
- **Remover objetos clicados**: Remover objetos clicados da tela para evitar cliques repetidos.
- **Carregar Cena 3D**: Adicionar um cenário 3D com objetos e texturas para aplicar mais técnicas do OpenGL.
- **Níveis de Dificuldade**: Implementar níveis de dificuldade progressivos, com mais objetos e distrações em níveis avançados.
- **Sons e Efeitos Visuais**: Adicionar efeitos sonoros e visuais para tornar o jogo mais imersivo e envolvente.
- **Feedback Visual**: Adicionar feedback visual para indicar acertos e erros.

### Correções Necesárias
- **Corrigir reconhecimento de cliques**: Ajustar a detecção de cliques para garantir que os cliques sejam registrados corretamente.

## Instalação e Execução

Para compilar e executar o jogo:

1. Clone este repositório:
   ```bash
   git clone https://github.com/usuario/jogo-treinamento-atencao.git
   cd jogo-treinamento-atencao
    ```
2. Seguir as instruções de instalação encontradas em:
[Configuração do ambiente](https://hbatagelo.github.io/cg/config.html)

3. O jogo pode ser acessado pelo browser no link:
[WebAssembly do jogo](https://lorenypsum.github.io/abcg/attentioneval3d/)

## Video de Gameplay
[Link do vídeo](https://drive.google.com/drive/folders/1Twt4VrN87Xa7bjZ_nGLe0jpwOSqIPjjB?usp=sharing)

## Referências
- [Bird Object](https://www.cgtrader.com/items/3816085/download-page)
- [Hbatagelo](https://hbatagelo.github.io/cg/)
- [OpenGL](https://www.opengl.org/)
- [SDL](https://www.libsdl.org/)



