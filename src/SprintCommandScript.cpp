#include "ScriptMgr.h"
#include "Chat.h"
#include "Player.h"
#include "SprintConfig.h"
#include "SprintManager.h"

using namespace Acore::ChatCommands;

class SprintCommandScript : public CommandScript
{
public:
    SprintCommandScript() : CommandScript("SprintCommandScript") {}

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable commandTable =
        {
            { "sprintstart", HandleSprintStartCommand, SEC_PLAYER, Console::No },
            { "sprintstop",  HandleSprintStopCommand,  SEC_PLAYER, Console::No }
        };
        return commandTable;
    }

    static bool HandleSprintStartCommand(ChatHandler* handler)
    {
        if (!sSprintConfig->IsEnabled())
            return false;

        Player* player = handler->GetSession()->GetPlayer();
        if (!player || player->IsMounted())
            return false;

        if (sSprintManager->GetStamina(player) <= 0.0f)
            return false;

        sSprintManager->SetSprinting(player, true);
        player->SetSpeed(MOVE_RUN, sSprintConfig->GetSpeedMultiplier(), true);
        return true;
    }

    static bool HandleSprintStopCommand(ChatHandler* handler)
    {
        if (!sSprintConfig->IsEnabled())
            return false;

        Player* player = handler->GetSession()->GetPlayer();
        if (!player)
            return false;

        sSprintManager->SetSprinting(player, false);
        player->SetSpeed(MOVE_RUN, 1.0f, true);
        return true;
    }
};

void AddSprintCommandScript()
{
    new SprintCommandScript();
}
