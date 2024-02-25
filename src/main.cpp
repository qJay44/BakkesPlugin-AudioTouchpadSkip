#include "Gamepad.hpp"
#include <thread>

void dummyThread(Gamepad& gp) {
  int limit = 10;

  while (limit--) {
    printf("state: %d\n", gp.touchpadButtonPressed());
    Sleep(1000);
  }

  printf("The thread has ended");
}

int main() {
  Gamepad gamepad;
  std::thread t([&gamepad]{dummyThread(gamepad);});
  t.join();

  return 0;
}

