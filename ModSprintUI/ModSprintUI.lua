SprintBarDB = SprintBarDB or {}
SprintBarDB.point = SprintBarDB.point or "CENTER"
SprintBarDB.relativePoint = SprintBarDB.relativePoint or "CENTER"
SprintBarDB.x = SprintBarDB.x or 0
SprintBarDB.y = SprintBarDB.y or -200
SprintBarDB.sprintKey = SprintBarDB.sprintKey or "F"

-- Main UI Frame
local f = CreateFrame("Frame", "SprintBarFrame", UIParent)
f:SetSize(200, 16)
f:SetPoint(SprintBarDB.point, UIParent, SprintBarDB.relativePoint, SprintBarDB.x, SprintBarDB.y)
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
SprintBarDB.point = point
SprintBarDB.relativePoint = relativePoint
SprintBarDB.x = x
SprintBarDB.y = y
DEFAULT_CHAT_FRAME:AddMessage("|cff00ff00[Sprint Bar]|r Position saved.")
end)

-- Visuals
local bg = f:CreateTexture(nil, "BACKGROUND")
bg:SetAllPoints(f)
bg:SetTexture("Interface\\Buttons\\WHITE8x8")
bg:SetVertexColor(0, 0, 0, 0.6)

local bar = f:CreateTexture(nil, "ARTWORK")
bar:SetPoint("TOPLEFT", f, "TOPLEFT", 2, -2)
bar:SetSize(196, 12)
bar:SetTexture("Interface\\Buttons\\WHITE8x8")
bar:SetVertexColor(0.2, 0.6, 1.0, 1.0)

local text = f:CreateFontString(nil, "OVERLAY", "GameFontHighlightSmall")
text:SetPoint("CENTER", f, "CENTER", 0, 0)
text:SetText("Stamina: 100%")

-- Hide system packet spam
ChatFrame_AddMessageEventFilter("CHAT_MSG_SYSTEM", function(self, event, message)
if message and message:find("AC_SPRINT:") then return true end
    return false
    end)

local eventFrame = CreateFrame("Frame")
eventFrame:RegisterEvent("CHAT_MSG_SYSTEM")
eventFrame:SetScript("OnEvent", function(self, event, message)
if message and message:find("AC_SPRINT:") then
    local percent = tonumber(message:match("AC_SPRINT:(%d+)")) or 100
    bar:SetWidth(196 * (percent / 100))
    text:SetText(string.format("Stamina: %d%%", percent))
    end
    end)

-- Secure Action Button Setup for 3.3.5a
local sprintButton = CreateFrame("Button", "ModSprintSecureButton", UIParent, "SecureActionButtonTemplate")
sprintButton:SetAttribute("type", "macro")
sprintButton:SetAttribute("macrotext", "/sprintstart")

-- Global function to apply bindings safely
function ApplySprintBinding(key)
ClearOverrideBindings(sprintButton)
-- Handle friendly names for shifts if entered
key = strupper(key)
if key == "LSHIFT" or key == "RSHIFT" then
    key = "SHIFT-SPACE" -- Fallback or modifier combo suggestion
    DEFAULT_CHAT_FRAME:AddMessage("|cffff0000[Sprint]|r Standalone Shift cannot be bound directly. Try a combo like SHIFT-F or use another key.")
    end
    SetOverrideBindingClick(sprintButton, true, key, "ModSprintSecureButton")
    end

    -- Initialize binding with saved or default key
    ApplySprintBinding(SprintBarDB.sprintKey)

    -- Heartbeat and Release Handler
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

    -- Lock/Unlock command
    SLASH_SPRINTLOCK1 = "/sprintlock"
    SlashCmdList["SPRINTLOCK"] = function()
    isLocked = not isLocked
    if isLocked then
        DEFAULT_CHAT_FRAME:AddMessage("|cff00ff00[Sprint Bar]|r Locked in place.")
        else
            DEFAULT_CHAT_FRAME:AddMessage("|cffff0000[Sprint Bar]|r Unlocked. Click and drag to reposition.")
            end
            end

            -- Rebind command
            SLASH_SPRINTBIND1 = "/sprintbind"
            SlashCmdList["SPRINTBIND"] = function(msg)
            msg = strupper(strtrim(msg or ""))
            if msg ~= "" then
                SprintBarDB.sprintKey = msg
                ApplySprintBinding(msg)
                DEFAULT_CHAT_FRAME:AddMessage("|cff00ff00[Sprint]|r Hold key successfully bound to: |cffffff00" .. msg .. "|r")
                else
                    DEFAULT_CHAT_FRAME:AddMessage("|cff00ff00[Sprint]|r Current hold key: |cffffff00" .. SprintBarDB.sprintKey .. "|r")
                    DEFAULT_CHAT_FRAME:AddMessage("Usage: /sprintbind <KEY> (e.g., F, R, SHIFT-F, MouseButton4)")
                    end
                    end
