#include "controller.hpp"

int main(void) {
  setlocale(LC_CTYPE, "");
  WIN_INIT(INITIAL_TIMEOUT);
  PrintOverlay();
  GameLoop();
  endwin();
  return SUCCESS;
}
// For ubuntu if need to realise clock_gettime
// #define _POSIX_C_SOURCE 199309L
//  #define CLOCK_REALTIME 0
// int clock_gettime(int clk_id, struct timespec *tp) {
//   struct timeval now;
//   int rv = gettimeofday(&now, NULL);
//   if (rv == 0) {
//     tp->tv_sec = now.tv_sec;
//     tp->tv_nsec =
//         now.tv_usec * 1000;  // Конвертируем микросекунды в наносекунды
//   }
//   return rv;
// }