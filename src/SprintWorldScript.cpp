#include "ScriptMgr.h"
#include "Player.h"
#include "World.h"
#include "WorldSession.h"
#include "WorldSessionMgr.h"
#include "SprintConfig.h"
#include "SprintManager.h"
#include "Chat.h"

class SprintWorldScript : public WorldScript
{
public:
    SprintWorldScript() : WorldScript("SprintWorldScript") {}

    void OnUpdate(uint32 diff) override
    {
        if (!sSprintConfig->IsEnabled())
            return;

        WorldSessionMgr::SessionMap const& sessionMap = sWorldSessionMgr->GetAllSessions();
        for (auto itr = sessionMap.begin(); itr != sessionMap.end(); ++itr)
        {
            if (Player* player = itr->second->GetPlayer())
            {
                if (!player->IsInWorld())
                    continue;

                const float drainRate = 25.0f; // Drains 25% per second
                const float regenRate = 20.0f; // Regens 20% per second

                if (sSprintManager->IsSprinting(player))
                {
                    float currentStamina = sSprintManager->GetStamina(player);

                    if (currentStamina <= 0.0f)
                    {
                        sSprintManager->StopSprint(player);
                        ChatHandler(player->GetSession()).SendSysMessage("You are out of stamina!");
                        sSprintManager->SendStaminaUpdate(player);
                        continue;
                    }

                    float drainAmount = drainRate * (static_cast<float>(diff) / 1000.0f);
                    float newStamina = currentStamina - drainAmount;
                    sSprintManager->SetStamina(player, newStamina);
                    sSprintManager->SendStaminaUpdate(player);
                }
                else
                {
                    float currentStamina = sSprintManager->GetStamina(player);
                    float maxStamina = sSprintConfig->GetMaxStamina();

                    if (currentStamina < maxStamina)
                    {
                        float regenAmount = regenRate * (static_cast<float>(diff) / 1000.0f);
                        float newStamina = std::min(maxStamina, currentStamina + regenAmount);
                        sSprintManager->SetStamina(player, newStamina);
                        sSprintManager->SendStaminaUpdate(player);
                    }
                }
            }
        }
    }
};

void AddSprintWorldScript()
{
    new SprintWorldScript();
}
