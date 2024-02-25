#include <windows.h>
#include <hidsdi.h>
#include <stdint.h>

enum TouchpadState: uint8_t {
  TOUCHPAD_RIGHT_SIDE,
  TOUCHPAD_LEFT_SIDE,
  TOUCHPAD_NOTHING
};

class Gamepad {
  public:
    Gamepad();
    ~Gamepad();

    uint8_t touchpadButtonPressed();

  private:
    HANDLE hHidDeviceObject;
    PHIDP_PREPARSED_DATA preparsedData;
    HIDP_CAPS caps;
};

