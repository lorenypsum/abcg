#include "window.hpp"

// Função para criar a janela
int main(int argc, char **argv) {
  try {
    abcg::Application app(argc, argv);

    Window window;
    window.setOpenGLSettings({});
    window.setWindowSettings({
        .width = 1200,
        .height = 1200,
        .title = "Vôo dos Pássaros",
    });

    app.run(window);
  } catch (std::exception const &exception) {
    fmt::print(stderr, "{}\n", exception.what());
    return -1;
  }
  return 0;
}