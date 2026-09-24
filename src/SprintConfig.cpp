#include "SprintConfig.h"
#include "Config.h"

SprintConfig* SprintConfig::GetInstance()
{
    static SprintConfig instance;
    return &instance;
}

void SprintConfig::LoadConfig()
{
    _enabled = sConfigMgr->GetOption<bool>("Sprint.Enabled", true);
    _maxStamina = sConfigMgr->GetOption<float>("Sprint.MaxStamina", 100.0f);
    _speedMultiplier = sConfigMgr->GetOption<float>("Sprint.SpeedMultiplier", 1.5f);
}
