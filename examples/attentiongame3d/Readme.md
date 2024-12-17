# Treinamento de atenção seletiva através de jogo 3D

- Lorena Silva Sampaio - 11201812025
- Jonas de Mello Oliveira - 11000116

![Gameplay GIF](giphy.gif)

Este projeto apresenta o jogo/aplicação 3D **Voo dos Pássaros**, uma ferramenta desenvolvida após uma série de entrevistas empáticas com professores e com uma profissional da neuropsicologia. 

Sua finalidade é treinar habilidades de atenção e concentração dos jogadores, desafiando-os a identificar e coletar objetos específicos em meio a um ambiente visual dinâmico (no caso, pássaros brancos).

O repositório contém o código desenvolvido em C++ com OpenGL, SDL e ImGui. 

Na aplicação, o usuário precisa capturar os alvos corretos (pássaros brancos) enquanto evita o contato com objetos de distração (pássaros de outras cores). 

Cada pássaro branco que atinge o cesto aumenta a pontuação, enquanto que atingir uma distração diminui a pontuação. 

O objetivo é conseguir a maior pontuação possível antes do tempo acabar.

## Tabela de Conteúdos

- [Motivação do Projeto](#motivação-do-projeto)
  - [Entrevistas com Educadores](#entrevistas-com-educadores)
  - [Consultoria Neuropsicológica](#consultoria-neuropsicológica)
  - [Avaliação do resultado](#avaliação-do-resultado)
- [Funcionalidades do Jogo](#funcionalidades-do-jogo)
- [Estrutura do Código](#estrutura-do-código)
- [Técnicas Implementadas](#técnicas-implementadas)
  - [Interface de Usuário com ImGui](#interface-de-usuário-com-imgui)
  - [Interação do Usuário com SDL](#interação-do-usuário-com-sdl)
  - [Controle de Tempo e Estado do Jogo](#controle-de-tempo-e-estado-do-jogo)
  - [Renderização de Objetos com OpenGL](#renderização-de-objetos-com-opengl)
- [Avaliação do Projeto e Possíveis Melhorias](#avaliação-do-projeto-e-possíveis-melhorias)
- [Instalação e Execução](#instalação-e-execução)
- [Referências](#referências)

## Motivação do Projeto

O projeto é uma continuação em 3D do projeto originado na Atividade 1 (Treinamento de Atenção Seletiva), que foi desenvolvido em 2D, e que já havia passado por uma evolução na Atividade 2. A motivação do tema do jogo foi baseada em **entrevistas empáticas** realizadas com professores via Google Forms, que apontaram a falta de atenção e concentração como maior dificuldade enfrentada no ambiente escolar.

### Entrevistas com educadores

Nove educadores responderam nosso [formulário](https://skybox.blockadelabs.com/345f1511b1568ea07e0edacafe02564a), para muitos deles, a **falta de atenção e concentração** é a maior dificuldade enfrentada no ambiente escolar. D

iante disso, ficou evidente a necessidade de criar uma ferramenta que auxiliasse no treinamento e desenvolvimento dessas habilidades cognitivas em um contexto interativo. 

Abaixo as respostas obtidas:

![alt text](image.png)
![alt text](image-1.png)
![alt text](image-2.png)
![alt text](image-3.png)
![alt text](image-4.png)
![alt text](image-5.png)
![alt text](image-6.png)
![alt text](image-8.png)
![alt text](image-9.png)

[Resultado](https://docs.google.com/forms/d/1IAE4KtZw2pSLaoXtc2e_4C2onLBX11Kny89mkhgaCDM/edit#responses)

### Consultoria Neuropsicológica

Para validar a proposta, uma neuropsicóloga especializada em treinamento cognitivo foi entrevistada e sugeriu uma série de ideias para o desenvolvimento do jogo, resumidas abaixo:

- **Mecânica de Memória e Atenção**: Sugeriu-se que o jogo combinasse elementos de memória, concentração e percepção visual, com o objetivo de manter o jogador focado em uma tarefa específica enquanto é desafiado por distrações.

- **Progressão por Níveis**: A neuropsicóloga recomendou que o jogo apresentasse desafios progressivos, onde o número de objetos e distrações aumentasse em cada nível, dificultando o reconhecimento dos objetos a serem coletados.

- **Objetos Únicos e Distrações Dinâmicas**: Outro ponto foi a inclusão de objetos que aparecem apenas uma vez em certos níveis, incentivando o jogador a desenvolver uma atenção seletiva para identificar rapidamente o que é relevante em meio a uma grande quantidade de estímulos visuais.

Então, pensou-se em um jogo onde o jogador precisa focar em uma tarefa específica, enquanto evitaa as distrações. 

Essas sugestões foram simplificadas em um mvp (mínimo produto viável) e incorporadas ao projeto, dando origem ao **Caça ao Pombo Branco** na Atividade 1, e **Voo dos Pássaros** nas Atividades 2 e 3.

### Avaliação do resultado
Usuários diversos, incluindo a neuropsicológa a quem consultamos durante o desenvolvimento responderam nosso [formulário de avaliação](https://docs.google.com/forms/d/e/1FAIpQLSfDd1byk0fjFMBKVXc6Ua6ZFz98xesOv3jOeVzAq-ku-3l5JQ/viewform), 

o resultado pode ser verificado abaixo:

-- Falta acrescentar imagens (esperando mais respostas).

[Resultado](https://docs.google.com/forms/d/1hA7HiJLJCaGIad1HFRY20SkIu5C4w1KNj6MDwGKEL5U/edit#responses)

## Funcionalidades do jogo

O jogo foi modelado como uma caça ao pássaro, no qual o jogador deve capturar os pássaros brancos enquanto tenta evitar os pássaros de outra cor, incluindo as funcionalidades abaixo: 

**1. Contador de tempo**
**2. Um sistema de pontuação:**
	* + um ponto por pássaro certo, 
	* - um por pássaro errado, 
**3. Estados de jogo (início, jogando, vitória e game over).**

A versão do jogo entregue na atividade A3 apresenta um cenário 3d de fundo (skybox), representando um ambiente natural, com vários pássaros vindo em sua direção de maneira similar ao efeito _starfield_, porém com alteração da movimentação dos mesmos, além disso, há um cesto de captura que você deve guiar para capturar os pássaros brancos enquanto evita os outros, no prazo de trinta segundos.

## 🎮 Instruções do Jogo

1. Clicar na tela;
2. O tempo iniciará uma contagem regressiva de 30s automaticamente, assim que um pássaro atingir o cesto;;
3. Movimentar o cesto com as setas direcionais do teclado; 

- **Teclas Direcionais**:
  - **⬆️ Cima**: Move o objeto para cima.
  - **⬇️ Baixo**: Move o objeto para baixo.
  - **⬅️ Esquerda**: Move o objeto para a esquerda.
  - **➡️ Direita**: Move o objeto para a direita.

4. Desviar dos pássaros coloridos enquanto tenta obter os pássaros brancos;
5. A pontuação aumenta ao capturar um pássaro branco e diminui ao capturar um pássaro colorido;
6. O jogo termina quando o tempo acaba e mostra a pontuação final.

## Estrutura do Código

- **`main.cpp`**: Arquivo principal responsável pela inicialização do jogo e da interface gráfica.
- **`window.cpp`**: Gerencia a configuração da janela e a renderização dos elementos do jogo.
- **`window.hpp`**: Define a classe `Window`, que controla a janela do jogo e suas interações.
- **`model.cpp`**: Gerencia a renderização dos modelos 3D do jogo.
- **`model.hpp`**: Define a classe `Model`, que carrega e renderiza os modelos 3D.
- **`game_objects.cpp`**: Gerencia a renderização e interação dos objetos do jogo.
- **`game_objects.hpp`**: Define as classes que representam os objetos do jogo (alvo e distrações).
- **`gamedata.hpp`**: Define a estrutura `GameData`, que armazena os estados e informações relevantes do jogo.
- **`trackball.hpp`**: Define a classe `Trackball`, que implementa o trackball virtual.
- **`trackball.cpp`**: Gerencia a rotação dos elementos, como o cesto que muda de ângulo conforme o movimento.
- pasta **`assets`**: Contém os modelos dos objetos do jogo (pássaros, cesto)
- pasta **`shaders`**: Contém os *shaders* de vértice e de fragmento.
- pasta **`cube`**: Contém as imagens que formam o cenário.

## Técnicas Implementadas

O desenvolvimento se deu com várias técnicas de computação gráficas, abaixo serão explicitadas as partes onde modificações foram feitas em relação ao material original das aulas:

### Interface de Usuário com ImGui

Na classe *window.cpp* no método `paintUI()`, foi implementada a interface gráfica do usuário (UI) com a biblioteca **ImGui**:

Na interface do usuário (UI), foi usada a biblioteca **ImGui** para mostrar os dados de:
- **Pontuação e Tempo**: Um contador de tempo e pontuação são exibidos na tela, atualizados em tempo real, ajudando o jogador a acompanhar seu progresso.
- **Estados do Jogo**: Mensagens informativas são exibidas dependendo do estado do jogo:
  - **Start**: Orienta o jogador a pegar o alvo correto.
  - **Playing**: Exibe o tempo e a pontuação enquanto o jogo está ativo.
  - **Game Over**: Mostra a pontuação final quando o tempo acaba. 
  - **Win**: Exibe uma mensagem de vitória se o jogador atingir uma nova pontuação recorde. //Esse estado ainda apresenta bugs para ser apresentado.

Na classe *game_objects.cpp,* no método `paintUI()`, foi implementado as seguintes funcionalidades de UI:
- **Configuração da Janela do Widget:** tamanho, posição, estilo, combo box foram definidos. 
- **Combo box:** permite ao usuário alternar entre duas opções, "Vôo dos Pássaros" "Cenário 3D".
- **Modo "Vôo dos Pássaros":** a matriz de projeção (m_projMatrix) é configurada com uma projeção em perspectiva com valores específicos. Um controle slider ajusta a posição da câmera no eixo Z (m_viewMatrix[3][2]) dentro do intervalo [-10.0, 10.0].
- **Modo "Cenário 3D":** a matriz de projeção (m_projMatrix) também é configurada com uma projeção em perspectiva, mas com um intervalo diferente para o plano próximo (near plane).
Um controle slider permite ajustar o campo de visão (FOV) entre 5° e 179°.


### Interação do Usuário com SDL

Na classe **window.cpp**, no método `onEvent()`, foi implementado a interação do usuário com a aplicação, para detectar movimentos do cesto na tela, é utilizada a biblioteca biblioteca **SDL**:

- **SDL_KEYDOWN**: Evento que captura as teclas pressionadas para atualizar a posição do objeto.  
  - **⬆️ SDLK_UP**: Move o cesto para cima.  
  - **⬇️ SDLK_DOWN**: Move o cesto para baixo.  
  - **⬅️ SDLK_LEFT**: Move o cesto para a esquerda.  
  - **➡️ SDLK_RIGHT**: Move o cesto para a direita.  

- **Limitação da Posição**: A função **`std::clamp`** é utilizada para garantir que o cesto não ultrapasse os limites da tela, restringindo o movimento nos eixos **X** e **Y**:
  - **Eixo X**: de **-0.8** a **0.8**.  
  - **Eixo Y**: de **-0.8** a **0.8**.


### Controle de Tempo e Estado do Jogo

O estado do jogo e o cronômetro são geridos utilizando a biblioteca **chrono** do C++:
- **Cronômetro**: Cada segundo, o tempo é decrementado. Ao atingir zero, o jogo termina.
- **Estados do Jogo**: São definidos vários estados (Start, Playing, Game Over, Win) que determinam a lógica e as mensagens exibidas ao jogador. A função `checkGameStatus()` atualiza o estado com base na pontuação e tempo restantes.
- **Pontuação**: A pontuação é atualizada com base nas capturas corretas e incorretas do jogador, aumentando ou diminuindo a pontuação conforme necessário.

### Renderização de Objetos com OpenGL

A renderização é feita usando **OpenGL**:

#### Shaders

	Fizemos o uso de três programas de shader diferentes, o texture, normal_mapping e skybox, eles não foram alterados em relação aos códigos de shaders fornecido pelo viewer6 do material de aulas, mas uma breve explicação a respeito deles é fornecida abaixo:

- **Texture**: Para renderização dos passáros.

	1. **Blinn-Phong**: sistema de sombreamento usando o modelo Blinn-Phong para iluminação.

	2. **Mapeamento de Textura**: mapeamento de textura em GLSL (OpenGL Shading Language). 

	3. **Vertex Shader**: informações do vértice, como posição, normais e coordenadas de textura, são transformadas usando as matrizes modelo, visão e projeção, além de calcular vetores úteis para iluminação (como o vetor luz e o vetor para o observador). 

	4. **Fragment Shader**: informações do vertex shader são repassadas ao fragment shader para computar a cor de cada fragmento. Nesse, a iluminação é calculada usando o modelo Blinn-Phong, que considera componentes de luz ambiente, difusa e especular. Além disso, ele permite o uso de diferentes coordenadas de textura (planejamento, cilíndrico, esférico ou mapeamento da malha) que não aplicamos nesse projeto. 

	5. **Resultado**: O resultado é uma combinação de textura e iluminação física para gerar a cor final de cada pixel vista nos objetos (modelos de pássaro).

 - **Skybox Shader**: responsável por renderizar o cenário **skybox** utiliza **samplerCube** para mapeamento cúbico e apresenta os seguintes aspectos:

	1. **Vertex Shader**:  
  	- Recebe a posição dos vértices da skybox.
  	- Calcula as coordenadas de textura cúbica (**fragTexCoord**) a partir da posição do vértice.
  	- Transforma os vértices usando as matrizes de visão (**viewMatrix**) e projeção (**projMatrix**).

	2. **Fragment Shader**:  
  	- Utiliza o **samplerCube** para acessar a textura cúbica baseada em **fragTexCoord**.
  	- Define a cor de saída (**outColor**) como a amostrada da textura.

	3. **Resultado**: Renderiza um ambiente cúbico ao fundo da cena, permitindo criar a ilusão de um céu ou ambiente infinito ao redor do jogador.

- **Normal Mapping Shader**: o shader de **Normal Mapping** adiciona detalhes à superfície dos objetos sem alterar sua geometria, simulando variações na iluminação com base em texturas normais. Seus aspectos incluem:

	1. **Vertex Shader**:
  	- Calcula as posições e vetores necessários para iluminação no espaço da câmera.
  	- Determina os vetores de tangente (**TObj**), bitangente (**BObj**) e normal (**NObj**) no espaço do objeto.
  	- Repassa informações para o fragment shader para cálculo da iluminação.

	2. **Fragment Shader**:
  	- Realiza o cálculo de iluminação baseado no modelo **Blinn-Phong**, com suporte a mapas normais.
  	- Constrói a matriz **TBN** (Tangente, Bitangente e Normal) para transformar vetores no espaço da câmera para o espaço tangente.
  	- Suporta múltiplos modos de mapeamento:
    - **Triplanar Mapping**: Mapeamento planar adaptado a cada eixo.
    - **Cylindrical Mapping**: Mapeamento cilíndrico.
    - **Spherical Mapping**: Mapeamento esférico.
    - **Mesh Mapping**: Usa diretamente as coordenadas de textura da malha.
  	- A textura normal é amostrada e utilizada para ajustar o vetor normal na iluminação.

	3. **Resultado**: O shader combina as propriedades da textura difusa e normal para criar superfícies altamente detalhadas e realistas, aprimorando a interação da luz com os objetos no jogo, permitindo a renderização detalhada do cesto e suas respectivas "fibras emaranhadas" e textura..

#### Classe Model

	Esta classe praticamente não foi alterada em relação ao código fornecido originalmente, apenas houve simplificação/retirada de trechos de código que não foram utilizados, também fornecemos abaixo uma breve explicação sobre o que foi estudado sobre a classe durante o desenvolvimento do projeto:

- **Modelo (model.hpp e model.cpp)**: O código manipula modelos 3D (.obj) com texturas e materiais (.mtl) para renderizar objetos 3D.

	1. **Definição de Vértices e Índices**: Os vértices contêm posição, normais e coordenadas de textura. A estrutura de hash otimiza a busca por vértices únicos.

	2. **Cálculo de Normais**: Computa as normais dos vértices acumulando normais de suas faces adjacentes (através do produto vetorial de arestas). Normais acumuladas são normalizadas para garantir consistência.

	3. **Cálculo de Tangentes**: Computa tangentes e bitangentes com base nas coordenadas de textura. As tangentes são reortogonalizadas em relação às normais para evitar distorções.

	4. **Carregamento de Arquivos OBJ**: Usa `tinyobjloader` para processar modelos OBJ. Gera vértices e índices e configura propriedades como texturas difusas e parâmetros de material.

	5. **Centralização e Normalização**: Redimensiona o modelo para que caiba em um cubo unitário e centraliza-o na origem, facilitando sua manipulação no espaço 3D.

	6. **Buffers (VBO, EBO, VAO)**:
		- **`createBuffers`**: Cria e preenche os buffers de vértices e índices.
		- **`setupVAO`**: Configura os atributos de vértice no Vertex Array Object (VAO), incluindo posição, normais, tangentes e coordenadas de textura.

	7. **Carregamento de Texturas**: **`loadDiffuseTexture`**: Carrega uma textura difusa (imagem) para aplicação nos modelos. 

	8. **Renderização**: **`render`**: Renderiza o modelo configurando a textura, ativando o VAO e chamando `glDrawElements` para desenhar os triângulos.

	9. **Destruição**: Libera recursos como texturas, buffers e VAO quando o objeto é destruído.

#### Classe Game Objects
	
	Esta classe foi criada para separar a lógica de carregamento e manipulação dos objetos das funções da classe window, sendo a que mais sofreu alterações em relação ao código fornecido originalmente, funcionando como um intermediário entre a classe window e a classe model, fornecemos abaixo uma breve explicação sobre suas funcionalidades:

- **Objetos do Jogo (game_objects.hpp e game_objects.cpp)**: O código gerencia a renderização e interação dos objetos do jogo.

	1. **GameEntities::create():** inicializa os elementos do jogo, como modelos, shaders, posições iniciais e objetos de cena.
		1. **Habilita do teste de profundidade**: `abcg::glEnable(GL_DEPTH_TEST)`.
		2. **Carrega shaders**:
			- Textura básica (`texture.vert` e `texture.frag`).
			- Mapeamento normal (`normalmapping.vert` e `normalmapping.frag`).
			- Skybox (`skybox.vert` e `skybox.frag`).
		3. **Cria de objetos 3D**:
			- Skybox.
			- Objetos de distração e alvo (`bird.obj`).
			- Rede (`wicker_basket.obj`).
		4. **Define de posições iniciais e configuração dos objetos**.
			- Aleatoriza a posição de objetos utilizando `setupSceneObjects`.

	2. **GameEntities::update(float deltaTime):** atualiza os objetos de cena com base no tempo (`deltaTime`) e os reposiciona caso saiam da tela.
		1. **Parâmetros**:
			- `deltaTime`: tempo desde a última atualização.
			- velocidades e intervalos (e.g., `incZ`, `incX`, `incY`) para alterar as posições dos objetos.
		2. **Funcionalidade**:
			- Move os objetos no espaço 3D (eixos X, Y, Z).
			- Reposiciona objetos para coordenadas iniciais caso saiam do campo de visão.
			- Atualiza o ângulo de rotação dos objetos.

	3. **GameEntities::createSkybox(GLuint &program):** configura o skybox da cena, não foi alterado em relação ao original.
		1. Carrega a textura cúbica do skybox (`maps/cube/`).
		2. Configura o VBO e VAO para os vértices.
		3. Associa atributos como posição (`inPosition`) ao shader.
 	 
	4. **GameEntities::createObject():** configura objetos "pássaro" no jogo, serve apenas para modularizar o código e evitar repetição.
		1. **Parâmetros**:
			- `program`: Shader associado ao objeto.
			- `m_model`: Modelo a ser configurado.
			- `assetsPath`: Caminho para os arquivos de assets.
			- `objPath`: Caminho do modelo 3D (`.obj`).
			- `texturePath`: Caminho para a textura difusa.
			- `normalPath`: Caminho para a textura normal.
		2. **Funcionalidade**:
			- Carrega o modelo 3D, texturas (difusa e normal), e configura o VAO.

	5. **GameEntities::setupSceneObjects():** inicializa a posição de objetos de cena de forma aleatória em um intervalo especificado.
		1. **Parâmetros**:
  			- Intervalos (e.g., `minX`, `maxX`) para definir os limites de posição no espaço 3D.
		2. **Funcionalidade**:
  			- Utiliza a função `randomizeSceneObject` para definir aleatorização da posição inicial e eixo de rotação.

	6. **GameEntities::updateSceneObjects():** atualiza os objetos de cena, movendo-os no espaço 3D e reposicionando-os caso saiam do campo de visão.
		1. **Parâmetros**:
  			- `deltaTime`: Tempo desde a última atualização.
  			- Velocidades (`incX`, `incY`, `incZ`) e limites de posição.
		2. **Funcionalidade**:
			- Incrementa a posição dos objetos.
			- Reposiciona o objeto para uma nova posição aleatória quando necessário.
			- Atualiza o ângulo de rotação dos objetos.

	7. **GameEntities::randomizeSceneObject():** gera uma nova posição aleatória para um objeto.
		1. **Parâmetros**:
  			- Intervalos para os eixos X, Y e Z.
		2. **Funcionalidade**:
			- Define uma nova posição 3D utilizando distribuições uniformes.
			- Gera um novo eixo de rotação aleatório utilizando (`glm::sphericalRand`).

	8. **GameEntities::paint():** renderiza todos os elementos da cena.
		1. Limpa o buffer de cor e profundidade (`glClear`).
		2. Chama funções de renderização específicas para cada tipo de objeto:
			- Skybox.
			- Objetos de distração.
			- Objetos-alvo.
			- Rede.
		3. Usa shaders apropriados para cada modelo.

	9. **GameEntities::renderObject():** renderiza um conjunto de objetos no espaço 3D.
		1. **Parâmetros**:
		- `program`: Shader associado.
		- `m_model`: Modelo a ser renderizado.
		- `m_scObjects`: Vetor contendo os objetos da cena.
		2. **Funcionalidade**:
		- Configura variáveis uniformes no shader (e.g., `viewMatrix`, `lightDirWorldSpace`).
		- Calcula e aplica as transformações (posição, escala, rotação).
		- Renderiza cada objeto utilizando `m_model.render()`.

	10. **GameEntities::renderSkybox():** renderiza o skybox da cenário, não foi alterado em relação ao original.
		1. Configura variáveis uniformes para o shader.
		2. Renderiza o cubo do skybox com textura cúbica.
		3. Garante que o skybox seja desenhado corretamente com funções como `glDepthFunc(GL_LEQUAL)`.

	11. **GameEntities::destroySkybox():** libera recursos associados ao skybox.
		1. Deleta o programa de shader de cada um dos modelos.
		2. Deleta os buffers e VAO associados ao skybox.

	12. GameEntities::update():** atualiza a lógica e estado dos objetos no jogo.
		1. Atualiza posições e rotações de objetos de distração e alvo.
		2. Movimenta os objetos no eixo Z para criar uma sensação de profundidade.

	13. **GameEntities::renderNet():** renderiza a rede/cesto da cena.
  		1. Configura variáveis uniformes no shader.
  		2. Aplica transformações (posição e rotação) na matriz de modelo.
  		3. Renderiza a rede utilizando o modelo associado.

	14. **Resumo**: este código é responsável por gerenciar toda a lógica de criação, movimentação, atualização e renderização das entidades do jogo. Ele utiliza recursos avançados de OpenGL, como shaders personalizados, texturas cúbicas para o skybox e mapeamento normal para melhorar a aparência dos modelos 3D.

#### Classe Window

- **Renderização de Janela (window.hpp e window.cpp)**: 

1. **Configuração Inicial:**
A cor de fundo é definida (skyblue) utilizando a função `glClearColor`. A fonte do texto é carregada a partir de um arquivo TTF. Objetos do jogo são inicializados, e o jogo começa no estado inicial (`GameState::Start`).

2. **Lógica de Jogo:**
	- **Estados do Jogo**: O jogo pode estar nos estados Start, Playing, GameOver, ou Win.
	- **Tempo e Pontuação**: O temporizador decrementa a cada segundo enquanto o estado é Playing. A pontuação aumenta ou diminui com base em cliques corretos ou incorretos.
	- **Fim do Jogo**: O jogo termina (GameOver) quando o tempo chega a zero.
    Se a pontuação atual exceder o recorde anterior, o estado muda para Win.

	3. **Eventos de Interação**
		- **Clique do mouse**: Libera o movimento do cesto ou habilita as mudanças de configurações.
		- **Toques nas setas direcionais**: Movimentam o cesto

	4. **Renderização**
Renderiza os objetos do jogo e uma interface do usuário usando ImGui para exibir a pontuação e o tempo restante.

	5. **Reinicialização**
Funções como `resetGame()` e `startGame()` permitem reiniciar o jogo para o estado inicial, com objetos reposicionados aleatoriamente e variáveis reiniciadas.

## Avaliação do Projeto e Possíveis Melhorias

O objetivo da atividade, que era obter uma aplicação didática funcional com gráficos 3D, usando os padrões e conceitos do curso (OpenGL, ABCG, etc), foi alcançado. Alguns problemas foram notados, como a presença de _bugs_ ao executar o jogo em um dos computadores da sala de aula, enquanto que o computador ao lado executava sem problemas. Foi assumido que o problema estava na configuração do computador em questão. 
Possíveis melhorias para a aplicação podem incluir:
- **Controle do cesto pelo mouse**: Garantir maior agilidade e liberdade de movimento que as setas do teclado permitem.
- **Opções de tempo de jogo**: Permitir ao usuário escolher diversas opções de tempo.
- **Voo**: Eliminar a rotação dos modelos e gerar apenas o movimento das asas. 
- **Níveis de Dificuldade**: Implementar níveis de dificuldade progressivos, com mais objetos e distrações em níveis avançados.
- **Sons e Efeitos Visuais**: Adicionar efeitos sonoros e visuais para tornar o jogo mais imersivo e envolvente.
- **Feedback Visual**: Adicionar feedback visual para indicar acertos e erros.
- **Bugs**: Maior atenção com as compatibilidades requeridas para evitar situações de *bugs* em diferentes sistemas operacionais e configurações.
- **Reiniciar**: Adicionar um botão de reinício na tela após o término do jogo.

No entanto, boa parte dessas melhorias já vão além do conteúdo da disciplina, e já tornariam o aplicativo menos próximo de uma versão didática e mais próximo de uma versão comercial. Assim, tendo em vista o objetivo desejado, o grupo considerou o resultado final plenamente satisfatório.

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
[WebAssembly do jogo](https://lorenypsum.github.io/abcg/attentiongame3d/)

## Vídeo de Gameplay
[Link do vídeo](https://drive.google.com/drive/folders/1Twt4VrN87Xa7bjZ_nGLe0jpwOSqIPjjB?usp=sharing)

## Referências
- [Bird Object](https://www.cgtrader.com/items/3816085/download-page)
- [Wicker Basket Object](https://www.cgtrader.com/free-3d-models/household/other/wicker-basket-0fad0b42-b8f4-45b4-930f-62d9499e6b60)
- [Skybox](https://skybox.blockadelabs.com/345f1511b1568ea07e0edacafe02564a)
- [Hbatagelo](https://hbatagelo.github.io/cg/)
- [OpenGL](https://www.opengl.org/)
- [SDL](https://www.libsdl.org/)


