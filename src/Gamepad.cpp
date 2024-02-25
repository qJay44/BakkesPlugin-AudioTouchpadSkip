#include "Gamepad.hpp"

#include <SetupAPI.h>
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>

#pragma comment (lib, "Setupapi.lib")
#pragma comment (lib, "Hid.lib")

Gamepad::Gamepad(std::string vendorId, std::string productId) {
  HDEVINFO hDevInfoSet;
  SP_DEVINFO_DATA devInfoData;
  SP_DEVICE_INTERFACE_DATA devIfcData;
  PSP_DEVICE_INTERFACE_DETAIL_DATA devIfcDetailData;

  DWORD dwMemberIdx = 0, dwSize, dwType;
  GUID hidGuid;
  PBYTE byteArrayBuffer;

  HidD_GetHidGuid(&hidGuid);
  printf("HID GUID: {%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}\n",
    hidGuid.Data1, hidGuid.Data2, hidGuid.Data3,
    hidGuid.Data4[0], hidGuid.Data4[1], hidGuid.Data4[2], hidGuid.Data4[3],
    hidGuid.Data4[4], hidGuid.Data4[5], hidGuid.Data4[6], hidGuid.Data4[7]);

  hDevInfoSet = SetupDiGetClassDevs(&hidGuid, NULL, 0, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
  printf("hDevInfoSet Handle: %p\n", hDevInfoSet);

  if (hDevInfoSet != INVALID_HANDLE_VALUE) {
    while (true) {
      devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

      if (!SetupDiEnumDeviceInfo(hDevInfoSet, dwMemberIdx, &devInfoData))
        break;

      SetupDiGetDeviceRegistryProperty(hDevInfoSet, &devInfoData, SPDRP_HARDWAREID, &dwType, NULL, 0, &dwSize);

      byteArrayBuffer = (PBYTE)malloc(dwSize * sizeof(BYTE));

      if (SetupDiGetDeviceRegistryProperty(hDevInfoSet, &devInfoData, SPDRP_HARDWAREID, &dwType, byteArrayBuffer, dwSize, NULL)) {
        std::string vid = "VID_" + vendorId;
        std::string pid = "PID_" + productId;

        // Check for the substings of vendor and product ids
        if (strstr((char*)byteArrayBuffer, (char*)&vid) && strstr((char*)byteArrayBuffer, (char*)&pid)) {
          devIfcData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
          SetupDiEnumDeviceInterfaces(hDevInfoSet, NULL, &hidGuid, dwMemberIdx, &devIfcData);

          SetupDiGetDeviceInterfaceDetail(hDevInfoSet, &devIfcData, NULL, 0, &dwSize, NULL);

          devIfcDetailData = (PSP_INTERFACE_DEVICE_DETAIL_DATA)malloc(dwSize);
          devIfcDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

          SetupDiGetDeviceInterfaceDetail(hDevInfoSet, &devIfcData, devIfcDetailData, dwSize, &dwSize, NULL);

          hHidDeviceObject = CreateFile((devIfcDetailData->DevicePath), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);

          // Getting device info
          HidD_GetPreparsedData(hHidDeviceObject, &preparsedData);
          HidP_GetCaps(preparsedData, &caps);

          free(devIfcDetailData);
        }
      }
      free(byteArrayBuffer);
      dwMemberIdx++;
    }
  } else
    printf("hDevInfo == INVALID_HANDLE_VALUE\n");

  SetupDiDestroyDeviceInfoList(hDevInfoSet);
}

Gamepad::~Gamepad() {
  HidD_FreePreparsedData(preparsedData);
  CloseHandle(hHidDeviceObject);
}

void Gamepad::onTouchpadEvent() {
  DWORD dwRead;
  PBYTE inputReport = (PBYTE)malloc(caps.InputReportByteLength);

  ReadFile(hHidDeviceObject, inputReport, caps.InputReportByteLength, &dwRead, 0);

  uint8_t touchId = 0;
  uint16_t touchX = 0;
  uint16_t touchY = 0;

  // 0 == finger is touching the touchpad
  if (!(inputReport[35] & 0b1000'0000)) {
    touchId = inputReport[35] & 0b0111'1111; // Each new touch has a different id
    touchX = ((uint16_t)inputReport[37] << 8 | (uint16_t)inputReport[36]) & 0b0000'1111'1111'1111;
    touchY = ((uint16_t)inputReport[38] << 4 | (uint16_t)inputReport[37] >> 4) & 0b0000'1111'1111'1111;
  };

  printf("x: %hu, y: %hu\n", touchX, touchY);

  free(inputReport);
}

void Gamepad::printLastError() {
  DWORD dwMessageId = GetLastError();
  LPSTR lpBuffer = nullptr;

  size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL, dwMessageId, 0, (LPSTR)&lpBuffer, 0, NULL);

  printf("LastError: %s\n", lpBuffer);

  LocalFree(lpBuffer);
}

