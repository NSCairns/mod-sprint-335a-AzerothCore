#include "ScriptMgr.h"
#include "SprintConfig.h"
#include "SprintManager.h"
#include "WorldSessionMgr.h"
#include "WorldSession.h"
#include "Player.h"
#include "Chat.h"

class SprintWorldScript : public WorldScript
{
private:
    uint32 _updateTimer = 500; // Run tick every 500ms

public:
    SprintWorldScript() : WorldScript("SprintWorldScript") {}

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sSprintConfig->LoadConfig();
    }

    void OnUpdate(uint32 diff) override
    {
        if (_updateTimer <= diff)
        {
            _updateTimer = 500;

            auto const& sessions = sWorldSessionMgr->GetAllSessions();
            for (auto const& pair : sessions)
            {
                WorldSession* session = pair.second;
                if (!session)
                    continue;

                Player* player = session->GetPlayer();
                if (!player || !player->IsInWorld() || player->IsGameMaster())
                    continue;

                bool isSprinting = sSprintManager->IsSprinting(player);
                float currentStam = sSprintManager->GetStamina(player);
                float maxStam = sSprintConfig->GetMaxStamina();

                if (isSprinting)
                {
                    float drainAmount = 10.0f; // 10 stamina per 500ms tick
                    float newStam = currentStam - drainAmount;

                    if (newStam <= 0.0f)
                    {
                        newStam = 0.0f;
                        sSprintManager->SetSprinting(player, false);
                        player->SetSpeed(MOVE_RUN, 1.0f, true);

                        if (player->GetSession())
                            ChatHandler(player->GetSession()).SendSysMessage("Exhausted! Sprint deactivated.");
                    }

                    sSprintManager->SetStamina(player, newStam);
                    sSprintManager->SendStaminaUpdate(player);
                }
                else
                {
                    if (currentStam < maxStam)
                    {
                        float regenAmount = 7.5f; // 7.5 stamina per 500ms tick
                        float newStam = currentStam + regenAmount;
                        if (newStam > maxStam)
                            newStam = maxStam;

                        sSprintManager->SetStamina(player, newStam);
                        sSprintManager->SendStaminaUpdate(player);
                    }
                }
            }
        }
        else
        {
            _updateTimer -= diff;
        }
    }
};

void AddSprintWorldScript()
{
    new SprintWorldScript();
}
