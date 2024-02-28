#include "pch.h"
#include <ctime>
#include "GamepadTouchpadHandle.h"

BAKKESMOD_PLUGIN(GamepadTouchpadHandle, "Play previous/next song with a touchpad", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void GamepadTouchpadHandle::onLoad() {
	_globalCvarManager = cvarManager;

	enabledPtr = std::make_shared<bool>(false);
	_globalCvarManager->registerCvar("plugin_enabled", "0", "Enabled", true, true, 0, true, 1).bindTo(enabledPtr);

	gameWrapper->HookEvent("Function TAGame.PlayerInput_TA.PlayerInput", std::bind(&GamepadTouchpadHandle::handleInput, this));
}

void GamepadTouchpadHandle::handleInput() {
  if (*enabledPtr) {
    constexpr double invCPS = 1. / CLOCKS_PER_SEC * 1000.; // Always 1?
    static std::clock_t clockStart = std::clock();

    std::clock_t clockEnd = std::clock();
    double elapsed = (clockEnd - clockStart) * invCPS;

    if (elapsed > 500.) {
      switch (gamepad.handleTouchpad()) {
        case TOUCHPAD_RIGHT_SIDE:
          _globalCvarManager->executeCommand("audio_next");
          clockStart = clockEnd;
          break;
        case TOUCHPAD_LEFT_SIDE:
          _globalCvarManager->executeCommand("audio_prev");
          clockStart = clockEnd;
          break;
        case TOUCHPAD_NOTHING:
          break;
      }
    }
  }
}

void GamepadTouchpadHandle::RenderSettings() {

  //======= Enable plugin option =======//

  CVarWrapper enableCvar = cvarManager->getCvar("plugin_enabled");
  if (enableCvar) {
    bool enabled = enableCvar.getBoolValue();
    if (ImGui::Checkbox(enableCvar.getDescription().c_str(), &enabled)) {
      enableCvar.setValue(enabled);
      _globalCvarManager->executeCommand("writeconfig", false);
    }

    if (ImGui::IsItemHovered())
      ImGui::SetTooltip("Toggle the plugin");
  }
  //====================================//
}

