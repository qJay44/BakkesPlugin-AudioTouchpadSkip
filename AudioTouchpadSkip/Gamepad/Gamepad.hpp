#pragma once

#include <windows.h>
#include <hidsdi.h>
#include <stdint.h>

enum TouchpadState : uint8_t {
  TOUCHPAD_LEFT_SIDE,
  TOUCHPAD_RIGHT_SIDE,
  TOUCHPAD_NOTHING
};

class Gamepad {
  public:
    Gamepad();
    ~Gamepad();

    TouchpadState handleTouchpad() const;
    void printInfo(const std::shared_ptr<CVarManagerWrapper>& _globalCvarManager) const;

  private:
    bool gamepadAllocated = false;
    bool isDualsense = false;

    HANDLE hHidDeviceObject;
    PHIDP_PREPARSED_DATA preparsedData;
    HIDP_CAPS caps;
};

