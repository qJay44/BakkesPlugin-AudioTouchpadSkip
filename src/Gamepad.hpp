#include <windows.h>
#include <string>
#include <hidsdi.h>

class Gamepad {
  public:
    Gamepad(std::string vendorId, std::string productId);
    ~Gamepad();

    void onTouchpadEvent();

  private:
    HANDLE hHidDeviceObject;
    PHIDP_PREPARSED_DATA preparsedData;
    HIDP_CAPS caps;

  private:
    void printLastError();
};

