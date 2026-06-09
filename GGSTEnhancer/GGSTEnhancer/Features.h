#pragma once

inline const char ConfigFileName[] = ".\\RED\\Binaries\\Win64\\GGSTEnhancer.ini";
inline const char AvatarFileName[] = ".\\Avatar.png"; //Context changed
inline const char AvatarBackupFileName[] = ".\\Avatar_Backup.png"; //Context changed
inline const char OriginalAvatarFileName[] = ".\\Avatar_Original.png"; //Context changed
inline const char ThumbnailFileName[] = ".\\Thumbnail.jpg"; //Context changed
inline const char OriginalThumbnailFileName[] = ".\\Thumbnail_Original.jpg"; //Context changed

//40 55 48 8D 6C 24 A9 48 81 EC ? ? ? ? 48 8B 01

inline const unsigned int UPLOAD_AVATAR_IMAGE_FLAG_OFFSET = 0x469;
inline const unsigned int AVATAR_IMAGE_DATA_MAX_SIZE = 262400;
inline const unsigned int AVATAR_IMAGE_DATA_OFFSET = 0x7ED348;
inline const unsigned int AVATAR_IMAGE_DATA_SIZE_OFFSET = 0x40100;
inline const unsigned int AVATAR_IMAGE_DATA_VALID_OFFSET = 0x40104;

inline const unsigned int DIGITAL_FIGURE_THUMBNAIL_OFFSET = 0x40;
inline const unsigned int THUMBNAIL_IMAGE_DATA_MAX_SIZE = 290816;
inline const unsigned int THUMBNAIL_IMAGE_DATA_OFFSET = 0x340;
inline const unsigned int THUMBNAIL_IMAGE_DATA_SIZE_OFFSET = 0x348;

inline const unsigned int MAX_TEXTURE_AXIS_DIMENSION = 4096;

inline __int64 OriginalThumbnail = 0;
inline void* ThumbnailAlloc = nullptr;

int UncensorMuseum();
int ImproveFishing();
int UnlockRewards();
int CustomAvatarImage();
int CustomThumbnail();
bool ReplaceAvatarImage();
int AntiPNGBomb();
#ifdef _DEBUG
int DLCUnlocker();
#endif
int ColorUnlocker();

inline bool bGetRewardAuras = false;
inline unsigned int SelectedRewardAvatarAura = 0;
inline unsigned int SelectedRewardNameAura = 0;
inline bool bGetRewardBadges = false;
inline bool bAntiPNGBomb = false;

inline __int64 CSaveDataManagerInstance = 0;

using Free_t = __int64(__fastcall*)(__int64 Object);
inline Free_t Orig_Free;

using AddInGameCash_t = void(__fastcall*)(__int64 CSaveDataManager, int add);
inline AddInGameCash_t Orig_AddInGameCash;
void __fastcall hk_AddInGameCash(__int64 CSaveDataManager, int add);

using SetRewardAvatarAura_t = void(__fastcall*)(__int64 UREDPlayerData, int itemID);
inline SetRewardAvatarAura_t Orig_SetRewardAvatarAura;

using SetRewardNameAura_t = void(__fastcall*)(__int64 UREDPlayerData, int itemID);
inline SetRewardNameAura_t Orig_SetRewardNameAura;

using SetRewardBadge_t = __int64(__fastcall*)(__int64 UREDPlayerData, __int64 badgeID, unsigned int add);
inline SetRewardBadge_t Orig_SetRewardBadge;

__int64 __fastcall hk_CheckRewardAura(__int64 UREDPlayerData);

using UpdateOnlineCheatPt_t = void(__fastcall*)(__int64, char);
inline UpdateOnlineCheatPt_t Orig_UpdateOnlineCheatPt;
void __fastcall hk_UpdateOnlineCheatPt(__int64 UREDPlayerData, char isMatchEnd);

using SetToDefault_All_t = __int64(__fastcall*)(__int64 CSaveDataManager);
inline SetToDefault_All_t Orig_SetToDefault_All;
__int64 __fastcall hk_SetToDefault_All(__int64 CSaveDataManager);

using ExportAvatarImage_t = __int64(__fastcall*)(__int64 UREDWidgetLobbyAvatarEditor);
inline ExportAvatarImage_t Orig_ExportAvatarImage;
__int64 __fastcall hk_ExportAvatarImage(__int64 UREDWidgetLobbyAvatarEditor);

using CreateTransient_t = __int64(__fastcall*)(int InSizeX, int inSizeY, __int64 InFormat, __int64 InName);
inline CreateTransient_t Orig_CreateTransient;
__int64 __fastcall hk_CreateTransient(int InSizeX, int InSizeY, __int64 InFormat, __int64 InName);

using GenerateThumbnail_t = __int64(__fastcall*)(__int64 Instance);
inline GenerateThumbnail_t Orig_GenerateThumbnail;
__int64 __fastcall hk_GenerateThumbnail(__int64 Instance);

using FigureCleanup_t = __int64(__fastcall*)(__int64 Instance, char a2);
inline FigureCleanup_t Orig_FigureCleanup;
__int64 __fastcall hk_FigureCleanup(__int64 Instance, char a2);

using IsSelectableCharaColorID_t = char(__fastcall*)(unsigned int charaID, unsigned int colorID, int costumeID);
inline IsSelectableCharaColorID_t Orig_IsSelectableCharaColorID;
char __fastcall hk_IsSelectableCharaColorID(unsigned int charaID, unsigned int colorID, int costumeID);

#ifdef _DEBUG
__int64 __fastcall hk_IsUnlocked(const wchar_t* pDLCName);
#endif
