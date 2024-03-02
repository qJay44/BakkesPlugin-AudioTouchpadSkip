#include "pch.h"
#include <ctime>
#include "AudioTouchpadSkip.h"

BAKKESMOD_PLUGIN(AudioTouchpadSkip, "AudioTouchpadSkip", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void AudioTouchpadSkip::onLoad() {
	_globalCvarManager = cvarManager;

	enabledPtr = std::make_shared<bool>(false);
  gamepad = new Gamepad();

	_globalCvarManager->registerCvar("plugin_enabled", "0", "Enabled", true, true, 0, true, 1).bindTo(enabledPtr);

	gameWrapper->HookEvent("Function TAGame.PlayerInput_TA.PlayerInput", std::bind(&AudioTouchpadSkip::handleInput, this));
	gameWrapper->HookEvent("Function TAGame.GFxData_EOSVoiceManager_TA.HandleAudioDevicesUpdated",
    [this](std::string eventName) {
      delete gamepad; gamepad = new Gamepad();
  });

#ifdef NDEBUG
  _globalCvarManager->registerNotifier("gamepad_info", [this](std::vector<std::string> args) {
    gamepad->printInfo(_globalCvarManager);
  }, "", PERMISSION_ALL);
#endif
}

void AudioTouchpadSkip::onUnload() {
  delete gamepad;
}

void AudioTouchpadSkip::handleInput() {
  if (*enabledPtr && gamepad) {
    constexpr double invCPS = 1. / CLOCKS_PER_SEC * 1000.; // Always 1?
    static std::clock_t clockStart = std::clock();

    std::clock_t clockEnd = std::clock();
    double elapsed = (clockEnd - clockStart) * invCPS;

    if (elapsed > 500.) {
      switch (gamepad->handleTouchpad()) {
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

void AudioTouchpadSkip::RenderSettings() {

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

