#include "window.hpp"

int main(int argc, char **argv) {
  try {
    // Inicializa o aplicativo
    abcg::Application app(argc, argv);
    // Cria a janela
    Window window;
    window.setOpenGLSettings({});
    window.setWindowSettings({
        .width = 1200,
        .height = 600,
        .title = "Caça ao Objeto",
    });
    // Executa o aplicativo
    app.run(window);
  } catch (std::exception const &exception) {
    fmt::print(stderr, "{}\n", exception.what());
    return -1;
  }
  return 0;
}