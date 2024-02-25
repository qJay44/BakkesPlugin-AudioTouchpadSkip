#include "Gamepad.hpp"
#include <thread>

void dummyThread(Gamepad& gp) {
  int limit = 10;

  while (limit--) {
    gp.onTouchpadEvent();
    Sleep(3000);
  }

  printf("The thread has ended");
}

int main() {
  printf("Begin.\n\n");

  Gamepad gamepad("054", "09");
  std::thread t([&gamepad]{dummyThread(gamepad);});
  t.join();

  printf("\nEnd\n");

  return 0;
}

