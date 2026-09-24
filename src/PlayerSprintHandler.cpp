#include "ScriptMgr.h"
#include "Player.h"
#include "SprintManager.h"
#include "SprintConfig.h"

class PlayerSprintHandler : public PlayerScript
{
public:
    PlayerSprintHandler() : PlayerScript("PlayerSprintHandler") {}

    void OnPlayerLogin(Player* player) override
    {
        sSprintManager->InitializePlayer(player);
    }

    void OnPlayerLogout(Player* player) override
    {
        player->SetSpeed(MOVE_RUN, 1.0f);
    }
};

void AddPlayerSprintHandler()
{
    new PlayerSprintHandler();
}
