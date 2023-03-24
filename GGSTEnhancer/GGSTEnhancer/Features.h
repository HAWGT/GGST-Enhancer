#pragma once

#include "Constants.h"

bool UnlockColorSelection();
bool UncensorMuseum();
bool ImproveFishing();
bool UnlockAura();

inline bool GetRewards = false;
inline unsigned int SelectedRewardAura = 0;
inline bool GetRewardBadges = false;

inline char* SteamID[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

typedef void(__fastcall* SetRewardAvatarAura_t)(__int64 UREDPlayerData, int itemID);
inline SetRewardAvatarAura_t Orig_SetRewardAvatarAura;

typedef void(__fastcall* SetRewardNameAura_t)(__int64 UREDPlayerData, int itemID);
inline SetRewardNameAura_t Orig_SetRewardNameAura;

typedef __int64(__fastcall* SetRewardBadge_t)(__int64 UREDPlayerData, __int64 badgeID, unsigned int add);
inline SetRewardBadge_t Orig_SetRewardBadge;

char __fastcall hk_IsSelectableCharaColorID(unsigned int charaID, unsigned int colorID);
__int64 __fastcall hk_CheckRewardAura(__int64 UREDPlayerData);