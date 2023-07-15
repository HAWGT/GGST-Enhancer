#pragma once

bool UnlockColorSelection();
bool UncensorMuseum();
bool ImproveFishing();
bool UnlockRewards();

inline bool bGetRewardAuras = false;
inline unsigned int SelectedRewardAvatarAura = 0;
inline unsigned int SelectedRewardNameAura = 0;
inline bool bGetRewardBadges = false;
inline bool bAntiRQFlag = false;

typedef void(__fastcall* AddInGameCash_t)(__int64 CSaveDataManager, int add);
inline AddInGameCash_t Orig_AddInGameCash;
void __fastcall hk_AddInGameCash(__int64 CSaveDataManager, int add);

typedef void(__fastcall* SetRewardAvatarAura_t)(__int64 UREDPlayerData, int itemID);
inline SetRewardAvatarAura_t Orig_SetRewardAvatarAura;

typedef void(__fastcall* SetRewardNameAura_t)(__int64 UREDPlayerData, int itemID);
inline SetRewardNameAura_t Orig_SetRewardNameAura;

typedef __int64(__fastcall* SetRewardBadge_t)(__int64 UREDPlayerData, __int64 badgeID, unsigned int add);
inline SetRewardBadge_t Orig_SetRewardBadge;

char __fastcall hk_IsSelectableCharaColorID(unsigned int charaID, unsigned int colorID);
__int64 __fastcall hk_CheckRewardAura(__int64 UREDPlayerData);

typedef void(__fastcall* UpdateOnlineCheatPt_t)(__int64, char);
inline UpdateOnlineCheatPt_t Orig_UpdateOnlineCheatPt;
void __fastcall hk_UpdateOnlineCheatPt(__int64 UREDPlayerData, char isMatchEnd);
