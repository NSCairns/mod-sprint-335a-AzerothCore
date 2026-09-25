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
        if (!player)
            return false;

        if (sSprintManager->GetStamina(player) <= 0.0f)
            return true; // Block quietly without error spam

            sSprintManager->StartSprint(player);
        return true;
    }

    static bool HandleSprintStopCommand(ChatHandler* handler)
    {
        if (!sSprintConfig->IsEnabled())
            return false;

        Player* player = handler->GetSession()->GetPlayer();
        if (!player)
            return false;

        sSprintManager->StopSprint(player);
        return true;
    }
};

void AddSprintCommandScript()
{
    new SprintCommandScript();
}
