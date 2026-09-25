#pragma once

#include "Config.h"

class SprintConfig
{
public:
    static SprintConfig* GetInstance()
    {
        static SprintConfig instance;
        return &instance;
    }

    bool IsEnabled() const
    {
        return sConfigMgr->GetOption<bool>("Sprint.Enabled", true);
    }

    float GetMaxStamina() const
    {
        return sConfigMgr->GetOption<float>("Sprint.MaxStamina", 100.0f);
    }

    float GetSpeedOnFoot() const
    {
        return sConfigMgr->GetOption<float>("Sprint.SpeedMultiplier.OnFoot", 0.5f);
    }

    float GetSpeedGroundMount() const
    {
        return sConfigMgr->GetOption<float>("Sprint.SpeedMultiplier.GroundMount", 0.7f);
    }

    float GetSpeedFlyingMount() const
    {
        return sConfigMgr->GetOption<float>("Sprint.SpeedMultiplier.FlyingMount", 0.4f);
    }
};

#define sSprintConfig SprintConfig::GetInstance()
