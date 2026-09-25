#pragma once

#include "Chat.h"
#include "Player.h"
#include "SprintConfig.h"
#include <unordered_map>

class SprintManager
{
public:
    static SprintManager* GetInstance()
    {
        static SprintManager instance;
        return &instance;
    }

    void InitializePlayer(Player* player)
    {
        if (!player)
            return;

        uint32 lowGuid = player->GetGUID().GetCounter();
        if (_playerStamina.find(lowGuid) == _playerStamina.end())
        {
            _playerStamina[lowGuid] = sSprintConfig->GetMaxStamina();
            _playerSprinting[lowGuid] = false;
            _originalSpeed[lowGuid] = 1.0f;
        }
    }

    float GetStamina(Player* player)
    {
        if (!player)
            return 0.0f;

        InitializePlayer(player);
        return _playerStamina[player->GetGUID().GetCounter()];
    }

    void SetStamina(Player* player, float amount)
    {
        if (!player)
            return;

        uint32 lowGuid = player->GetGUID().GetCounter();
        float maxStam = sSprintConfig->GetMaxStamina();

        if (amount < 0.0f) amount = 0.0f;
        if (amount > maxStam) amount = maxStam;

        _playerStamina[lowGuid] = amount;
    }

    bool IsSprinting(Player* player)
    {
        if (!player)
            return false;

        InitializePlayer(player);
        return _playerSprinting[player->GetGUID().GetCounter()];
    }

    void SetSprinting(Player* player, bool sprinting)
    {
        if (sprinting)
            StartSprint(player);
        else
            StopSprint(player);
    }

    void StartSprint(Player* player)
    {
        if (!player)
            return;

        uint32 lowGuid = player->GetGUID().GetCounter();
        InitializePlayer(player);

        if (!_playerSprinting[lowGuid])
        {
            _playerSprinting[lowGuid] = true;
            _originalSpeed[lowGuid] = player->GetSpeedRate(MOVE_RUN);

            float sprintBonus = sSprintConfig->GetSpeedMultiplier();
            player->SetSpeed(MOVE_RUN, _originalSpeed[lowGuid] + sprintBonus, true);
        }
    }

    void StopSprint(Player* player)
    {
        if (!player)
            return;

        uint32 lowGuid = player->GetGUID().GetCounter();
        InitializePlayer(player);

        if (_playerSprinting[lowGuid])
        {
            _playerSprinting[lowGuid] = false;
            player->SetSpeed(MOVE_RUN, _originalSpeed[lowGuid], true);
        }
    }

    void SendStaminaUpdate(Player* player)
    {
        if (!player || !player->GetSession())
            return;

        float current = GetStamina(player);
        float maxStam = sSprintConfig->GetMaxStamina();

        int percentage = static_cast<int>((current / maxStam) * 100.0f);
        std::string message = "AC_SPRINT:" + std::to_string(percentage);

        ChatHandler(player->GetSession()).SendSysMessage(message);
    }

private:
    std::unordered_map<uint32, float> _playerStamina;
    std::unordered_map<uint32, bool> _playerSprinting;
    std::unordered_map<uint32, float> _originalSpeed;
};

#define sSprintManager SprintManager::GetInstance()
