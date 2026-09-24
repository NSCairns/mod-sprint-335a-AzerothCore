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

        ObjectGuid guid = player->GetGUID();
        if (_playerStamina.find(guid) == _playerStamina.end())
        {
            _playerStamina[guid] = sSprintConfig->GetMaxStamina();
            _playerSprinting[guid] = false;
        }
    }

    float GetStamina(Player* player)
    {
        if (!player)
            return 0.0f;

        InitializePlayer(player);
        return _playerStamina[player->GetGUID()];
    }

    void SetStamina(Player* player, float amount)
    {
        if (!player)
            return;

        ObjectGuid guid = player->GetGUID();
        float maxStam = sSprintConfig->GetMaxStamina();

        if (amount < 0.0f) amount = 0.0f;
        if (amount > maxStam) amount = maxStam;

        _playerStamina[guid] = amount;
    }

    bool IsSprinting(Player* player)
    {
        if (!player)
            return false;

        InitializePlayer(player);
        return _playerSprinting[player->GetGUID()];
    }

    void SetSprinting(Player* player, bool sprinting)
    {
        if (!player)
            return;

        InitializePlayer(player);
        _playerSprinting[player->GetGUID()] = sprinting;
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
    std::unordered_map<ObjectGuid, float> _playerStamina;
    std::unordered_map<ObjectGuid, bool> _playerSprinting;
};

#define sSprintManager SprintManager::GetInstance()
