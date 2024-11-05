#include "controller.hpp"

int main(void) {
  setlocale(LC_CTYPE, "");
  WIN_INIT(INITIAL_TIMEOUT);
  PrintOverlay();
  // можно сделать разделение на разные игры, запуск либо одного контроллера либо другого
  GameLoop();
  endwin();
  return SUCCESS;
}