#pragma once

class SprintConfig
{
public:
    static SprintConfig* GetInstance();

    void LoadConfig();

    bool IsEnabled() const { return _enabled; }
    float GetMaxStamina() const { return _maxStamina; }
    float GetSpeedMultiplier() const { return _speedMultiplier; }

private:
    bool _enabled{true};
    float _maxStamina{100.0f};
    float _speedMultiplier{1.5f}; // Safe default prevents crashes
};

#define sSprintConfig SprintConfig::GetInstance()
