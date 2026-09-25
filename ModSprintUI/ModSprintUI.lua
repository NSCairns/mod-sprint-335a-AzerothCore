-- SavedVariablesPerCharacter: SprintBarDB

local sprintFrameLoaded = false
local loader = CreateFrame("Frame", "SprintBarLoaderFrame", UIParent)
loader:RegisterEvent("PLAYER_LOGIN")

loader:SetScript("OnEvent", function()
local name = UnitName("PLAYER")
local realm = GetRealmName()
if not name or name == "" then return end

    local charKey = name .. "_" .. realm

    -- Initialize global database table if it doesn't exist
    if not SprintBarDB then
        SprintBarDB = {}
        end
        if not SprintBarDB[charKey] then
            SprintBarDB[charKey] = {
                point = "CENTER",
                 relativePoint = "CENTER",
                 x = 0,
                 y = -200,
                 sprintKey = "SHIFT"
            }
            end

            local db = SprintBarDB[charKey]

            -- Create or fetch the persistent global UI Frame
            local f = _G["SprintBarFrame"] or CreateFrame("Frame", "SprintBarFrame", UIParent)
            f:SetSize(200, 16)
            f:ClearAllPoints()
            f:SetPoint(db.point, UIParent, db.relativePoint, db.x, db.y)
            f:SetMovable(true)
            f:EnableMouse(true)
            f:RegisterForDrag("LeftButton")

            local isLocked = true

            f:SetScript("OnDragStart", function(self)
            if not isLocked then f:StartMoving() end
                end)

            f:SetScript("OnDragStop", function(self)
            f:StopMovingOrSizing()
            local point, _, relativePoint, x, y = f:GetPoint()
            db.point = point
            db.relativePoint = relativePoint
            db.x = x
            db.y = y
            DEFAULT_CHAT_FRAME:AddMessage("|cff00ff00[Sprint Bar]|r Position saved successfully!")
            end)

            if not sprintFrameLoaded then
                local bg = f:CreateTexture(nil, "BACKGROUND")
                bg:SetAllPoints(f)
                bg:SetTexture("Interface\\Buttons\\WHITE8x8")
                bg:SetVertexColor(0, 0, 0, 0.6)

                local bar = f:CreateTexture(nil, "ARTWORK", nil, 1)
                bar:SetPoint("TOPLEFT", f, "TOPLEFT", 2, -2)
                bar:SetSize(196, 12)
                bar:SetTexture("Interface\\Buttons\\WHITE8x8")
                bar:SetVertexColor(0.2, 0.6, 1.0, 1.0)
                f.barTexture = bar

                local text = f:CreateFontString(nil, "OVERLAY", "GameFontHighlightSmall")
                text:SetPoint("CENTER", f, "CENTER", 0, 0)
                text:SetText("Stamina: 100%")
                f.textString = text

                ChatFrame_AddMessageEventFilter("CHAT_MSG_SYSTEM", function(self, event, message)
                if message and message:find("AC_SPRINT:") then return true end
                    return false
                    end)

                local eventFrame = CreateFrame("Frame")
                eventFrame:RegisterEvent("CHAT_MSG_SYSTEM")
                eventFrame:SetScript("OnEvent", function(self, event, message)
                if message and message:find("AC_SPRINT:") then
                    local percent = tonumber(message:match("AC_SPRINT:(%d+)")) or 100
                    if f.barTexture and f.textString then
                        f.barTexture:SetWidth(196 * (percent / 100))
                        f.textString:SetText(string.format("Stamina: %d%%", percent))
                        end
                        end
                        end)

                sprintFrameLoaded = true
                end

                local sprintButton = _G["ModSprintSecureButton"] or CreateFrame("Button", "ModSprintSecureButton", UIParent, "SecureActionButtonTemplate")
                sprintButton:SetAttribute("type", "macro")
                sprintButton:SetAttribute("macrotext", "/sprintstart")

                local modifierTracker = _G["ModSprintModifierFrame"] or CreateFrame("Frame", "ModSprintModifierFrame", UIParent)

                local function ApplySprintBinding(key)
                ClearOverrideBindings(sprintButton)
                key = strupper(key)
                modifierTracker:UnregisterEvent("MODIFIER_STATE_CHANGED")

                if key == "SHIFT" or key == "LSHIFT" or key == "RSHIFT" then
                    modifierTracker:RegisterEvent("MODIFIER_STATE_CHANGED")
                    modifierTracker:SetScript("OnEvent", function(self, event, modKey, state)
                    if modKey == "LSHIFT" or modKey == "RSHIFT" then
                        if state == 1 then
                            SendChatMessage(".sprintstart", "SAY")
                            else
                                SendChatMessage(".sprintstop", "SAY")
                                end
                                end
                                end)
                    else
                        SetOverrideBindingClick(sprintButton, true, key, "ModSprintSecureButton")
                        if not f.pulseHooked then
                            local pulseFrame = CreateFrame("Frame", nil, UIParent)
                            local isSprinting = false
                            local timer = 0

                            pulseFrame:SetScript("OnUpdate", function(self, elapsed)
                            if sprintButton:GetButtonState() == "PUSHED" then
                                isSprinting = true
                                timer = timer + elapsed
                                if timer >= 0.25 then
                                    timer = 0
                                    SendChatMessage(".sprintstart", "SAY")
                                    end
                                    else
                                        if isSprinting then
                                            isSprinting = false
                                            timer = 0
                                            SendChatMessage(".sprintstop", "SAY")
                                            end
                                            end
                                            end)
                            f.pulseHooked = true
                            end
                            end
                            end

                            -- Apply the saved key on login
                            ApplySprintBinding(db.sprintKey)

                            -- Lock/Unlock command
                            if not _G["SprintLockInitialized"] then
                                SLASH_SPRINTLOCK1 = "/sprintlock"
                                SlashCmdList["SPRINTLOCK"] = function()
                                isLocked = not isLocked
                                if isLocked then
                                    DEFAULT_CHAT_FRAME:AddMessage("|cff00ff00[Sprint Bar]|r Locked in place.")
                                    else
                                        DEFAULT_CHAT_FRAME:AddMessage("|cffff0000[Sprint Bar]|r Unlocked. Click and drag to reposition.")
                                        end
                                        end

                                        -- Rebind command linked directly to current character's db context
                                        SLASH_SPRINTBIND1 = "/sprintbind"
                                        SlashCmdList["SPRINTBIND"] = function(msg)
                                        msg = strupper(strtrim(msg or ""))
                                        if msg ~= "" then
                                            db.sprintKey = msg
                                            ApplySprintBinding(msg)
                                            DEFAULT_CHAT_FRAME:AddMessage("|cff00ff00[Sprint]|r Hold key successfully bound to: |cffffff00" .. msg .. "|r")
                                            else
                                                DEFAULT_CHAT_FRAME:AddMessage("|cff00ff00[Sprint]|r Current hold key: |cffffff00" .. db.sprintKey .. "|r")
                                                DEFAULT_CHAT_FRAME:AddMessage("Usage: /sprintbind <KEY> (e.g., SHIFT, F, R)")
                                                end
                                                end
                                                _G["SprintLockInitialized"] = true
                                                end
                                                end)
