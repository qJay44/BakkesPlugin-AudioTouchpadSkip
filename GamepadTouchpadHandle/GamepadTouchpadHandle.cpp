#include "pch.h"
#include "GamepadTouchpadHandle.h"

BAKKESMOD_PLUGIN(GamepadTouchpadHandle, "Play previous/next song with a touchpad", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void GamepadTouchpadHandle::onLoad() {
	_globalCvarManager = cvarManager;

	enabled = std::make_shared<bool>(false);
	_globalCvarManager->registerCvar("plugin_enabled", "0", "Enable the plugin", true, true, 0, true, 1).bindTo(enabled);

	gameWrapper->HookEvent("Function TAGame.PlayerInput_TA.PlayerInput", std::bind(&GamepadTouchpadHandle::handleInput, this));
}

void GamepadTouchpadHandle::onUnload() {
  gameWrapper->UnhookEvent("Function TAGame.PlayerInput_TA.PlayerInput");
}

void GamepadTouchpadHandle::handleInput() {
  // TODO: Set cooldown before handling it again
  if (enabled.get()) {
    switch (gamepad.handleTouchpad()) {
      case TOUCHPAD_RIGHT_SIDE:
        LOG("Next track");
        break;
      case TOUCHPAD_LEFT_SIDE:
        LOG("Previous track");
        break;
      case TOUCHPAD_NOTHING:
        break;
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
      LOG("enabledPtr: {}", (bool)this->enabled.get());
      _globalCvarManager->executeCommand("writeconfig", false);
    }

    if (ImGui::IsItemHovered())
      ImGui::SetTooltip("Toggle plugin");
  }
  //====================================//
}

