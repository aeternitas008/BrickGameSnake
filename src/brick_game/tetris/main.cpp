#include "controller.hpp"

int main(void) {
  setlocale(LC_CTYPE, "");
  WIN_INIT(INITIAL_TIMEOUT);
  PrintOverlay();
  GameLoop();
  endwin();
  return SUCCESS;
}