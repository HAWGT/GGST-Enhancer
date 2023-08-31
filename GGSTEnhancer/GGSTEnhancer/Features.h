#pragma once

inline const char ConfigFileName[] = ".\\RED\\Binaries\\Win64\\GGSTEnhancer.ini";
inline const char AvatarFileName[] = ".\\Avatar.png"; //Context changed
inline const char OriginalAvatarFileName[] = ".\\Avatar_Original.png"; //Context changed

inline const unsigned int UPLOAD_AVATAR_IMAGE_FLAG_OFFSET = 0x469;
inline const unsigned int AVATAR_IMAGE_DATA_MAX_SIZE = 262400;
inline const unsigned int AVATAR_IMAGE_DATA_OFFSET = 0x7ED348;
inline const unsigned int AVATAR_IMAGE_DATA_SIZE_OFFSET = 0x40100;
inline const unsigned int AVATAR_IMAGE_DATA_VALID_OFFSET = 0x40104;

bool UncensorMuseum();
bool ImproveFishing();
bool UnlockRewards();
bool CustomAvatarImage();
bool ReplaceAvatarImage();

inline bool bGetRewardAuras = false;
inline unsigned int SelectedRewardAvatarAura = 0;
inline unsigned int SelectedRewardNameAura = 0;
inline bool bGetRewardBadges = false;
inline bool bAntiRQFlag = false;

inline __int64 CSaveDataManagerInstance = 0;

typedef void(__fastcall* AddInGameCash_t)(__int64 CSaveDataManager, int add);
inline AddInGameCash_t Orig_AddInGameCash;
void __fastcall hk_AddInGameCash(__int64 CSaveDataManager, int add);

typedef void(__fastcall* SetRewardAvatarAura_t)(__int64 UREDPlayerData, int itemID);
inline SetRewardAvatarAura_t Orig_SetRewardAvatarAura;

typedef void(__fastcall* SetRewardNameAura_t)(__int64 UREDPlayerData, int itemID);
inline SetRewardNameAura_t Orig_SetRewardNameAura;

typedef __int64(__fastcall* SetRewardBadge_t)(__int64 UREDPlayerData, __int64 badgeID, unsigned int add);
inline SetRewardBadge_t Orig_SetRewardBadge;

__int64 __fastcall hk_CheckRewardAura(__int64 UREDPlayerData);

typedef void(__fastcall* UpdateOnlineCheatPt_t)(__int64, char);
inline UpdateOnlineCheatPt_t Orig_UpdateOnlineCheatPt;
void __fastcall hk_UpdateOnlineCheatPt(__int64 UREDPlayerData, char isMatchEnd);

typedef __int64(__fastcall* SetToDefault_All_t)(__int64 CSaveDataManager);
inline SetToDefault_All_t Orig_SetToDefault_All;
__int64 __fastcall hk_SetToDefault_All(__int64 CSaveDataManager);

typedef __int64(__fastcall* ExportAvatarImage_t)(__int64 UREDWidgetLobbyAvatarEditor);
inline ExportAvatarImage_t Orig_ExportAvatarImage;
__int64 __fastcall hk_ExportAvatarImage(__int64 UREDWidgetLobbyAvatarEditor);
