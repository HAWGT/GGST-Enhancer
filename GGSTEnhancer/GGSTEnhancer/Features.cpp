#include "Features.h"
#include "Helpers.h"
#include "Badges.h"
#include "Character.h"
#include "Color.h"

#include <fstream>
#include <filesystem>
#include <iostream>

inline BYTE* RefreshSetLocal = nullptr;
inline BYTE* RefreshSetOnline = nullptr;

inline BYTE Orig_RefreshSetLocal[] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
inline BYTE Orig_RefreshSetOnline[] = { 0x00, 0x00, 0x00, 0x00, 0x00 };

int UncensorMuseum()
{
	BYTE* MuseumFigureNSFWFlagSetterList = PatternScan("0F B6 43 61 48 8B 5C 24 30");
	if (!MuseumFigureNSFWFlagSetterList) return 1;

	BYTE* MuseumFigureNSFWFlagSetterModal = PatternScan("0F B6 43 61 88 47 61 0F B6 43 62");
	if (!MuseumFigureNSFWFlagSetterModal) return 2;

	BYTE MuseumFigureNSFWFlagSetterPatch[] = { 0x30, 0xC0, 0x90, 0x90 }; //xor al, al (nop nop)

	Patch(MuseumFigureNSFWFlagSetterPatch, MuseumFigureNSFWFlagSetterList, sizeof(MuseumFigureNSFWFlagSetterPatch));
	Patch(MuseumFigureNSFWFlagSetterPatch, MuseumFigureNSFWFlagSetterModal, sizeof(MuseumFigureNSFWFlagSetterPatch));

	return 0;
}

int ImproveFishing()
{
	BYTE* HasRareFish = PatternScan("72 ? 45 33 C0 44 88 4D E0");
	if (!HasRareFish) return 1;

	BYTE HasRareFishPatch[] = { 0x90, 0x90 };
	Patch(HasRareFishPatch, HasRareFish, sizeof(HasRareFishPatch));

	Orig_AddInGameCash = reinterpret_cast<AddInGameCash_t>(PatternScan("40 53 48 83 EC ? 48 63 81 24 35 03 00"));
	if (!Orig_AddInGameCash) return 2;
	Orig_AddInGameCash = reinterpret_cast<AddInGameCash_t>(TrampHook64((BYTE*)Orig_AddInGameCash, (BYTE*)hk_AddInGameCash, 16));
	if (!Orig_AddInGameCash) return 3;

	BYTE* ForceValidRoll = PatternScan("83 3D ? ? ? ? ? 0F 84 ? ? ? ? 44 8B 54 24 70");
	if (!ForceValidRoll) return 4;

	BYTE* DBM_ForceValidRollWeight = (BYTE*)(GetAddressFromInstruction((uintptr_t)ForceValidRoll, 6) + 1); //7th byte is the value of the comparison, not part of the address, but it needs to be accounted for

	if (!DBM_ForceValidRollWeight) return 5;

	*DBM_ForceValidRollWeight = 0x1;

	return 0;
}

int UnlockRewards()
{
	BYTE* Orig_CheckRewardAura = PatternScan("48 89 5C 24 20 55 56 57 48 83 EC ? 48 8B D9 48 8D 4C 24 40");
	if (!Orig_CheckRewardAura) return 1;

	//Find them with: 48 83 EC ? 8D 42 FF 45 8B D0 (NetworkGiftManager::AddSaveDataParam)

	//Case 2:
	BYTE* FindSetRewardAvatarAura = PatternScan("E8 ? ? ? ? B0 ? 48 83 C4 ? C3 48 83 3D ? ? ? ? ? 0F 84 ? ? ? ? 41 8D 42 FF 83 F8 ? 0F 87 ? ? ? ? 48 8B 0D ? ? ? ? 41 8B D2 48 8B 89 30 0B 00 00 E8 ? ? ? ?");
	if (!FindSetRewardAvatarAura) return 2;

	Orig_SetRewardAvatarAura = reinterpret_cast<SetRewardAvatarAura_t>(GetAddressFromInstruction((uintptr_t)FindSetRewardAvatarAura + 0x38, 5));
	if (!Orig_SetRewardAvatarAura) return 3;

	//Case 3:
	BYTE* FindSetRewardNameAura = PatternScan("E8 ? ? ? ? B0 ? 48 83 C4 ? C3 48 83 3D ? ? ? ? ? 0F 84 ? ? ? ? 45 85 D2");
	if (!FindSetRewardNameAura) return 4;

	Orig_SetRewardNameAura = reinterpret_cast<SetRewardNameAura_t>(GetAddressFromInstruction((uintptr_t)FindSetRewardNameAura, 5));
	if (!Orig_SetRewardNameAura) return 5;

	//Case 4:
	Orig_SetRewardBadge = reinterpret_cast<SetRewardBadge_t>(PatternScan("48 89 5C 24 10 55 56 41 54 41 56 41 57 48 83 EC ?"));
	if (!Orig_SetRewardBadge) return 6;

	//RequestUploadBasic() Error

	Orig_UpdateOnlineCheatPt = reinterpret_cast<UpdateOnlineCheatPt_t>(PatternScan("48 89 5C 24 18 57 48 83 EC ? 48 83 B9 F0 01 00 00 ?"));
	if (!Orig_UpdateOnlineCheatPt) return 7;

	Detour64(Orig_CheckRewardAura, (BYTE*)hk_CheckRewardAura, 15);

	return 0;
}

int CustomAvatarImage()
{
	Orig_SetToDefault_All = reinterpret_cast<SetToDefault_All_t>(PatternScan("48 89 5C 24 10 48 89 6C 24 18 56 57 41 54 41 56 41 57 48 83 EC ? 45 33 E4 48 8D B9 D4 00 00 00"));
	if (!Orig_SetToDefault_All) return 1;

	Orig_ExportAvatarImage = reinterpret_cast<ExportAvatarImage_t>(PatternScan("40 55 48 8D 6C 24 A9 48 81 EC ? ? ? ? 48 8B 01"));
	if (!Orig_ExportAvatarImage) return 2;

	Orig_ExportAvatarImage = reinterpret_cast<ExportAvatarImage_t>(TrampHook64((BYTE*)Orig_ExportAvatarImage, (BYTE*)hk_ExportAvatarImage, 14));
	if (!Orig_ExportAvatarImage) return 3;

	Orig_SetToDefault_All = reinterpret_cast<SetToDefault_All_t>(TrampHook64((BYTE*)Orig_SetToDefault_All, (BYTE*)hk_SetToDefault_All, 15));
	if (!Orig_SetToDefault_All) return 4;

	return 0;
}

int CustomThumbnail()
{
	Orig_GenerateThumbnail = reinterpret_cast<GenerateThumbnail_t>(PatternScan("40 56 48 83 EC ? 48 89 5C 24 58 0F 57 C9"));
	if (!Orig_GenerateThumbnail) return 1;

	Orig_GenerateThumbnail = reinterpret_cast<GenerateThumbnail_t>(TrampHook64((BYTE*)Orig_GenerateThumbnail, (BYTE*)hk_GenerateThumbnail, 14));
	if (!Orig_GenerateThumbnail) return 2;

	Orig_Free = reinterpret_cast<Free_t>(PatternScan("48 85 C9 74 ? 53 48 83 EC ? 48 8B D9 48 8B 0D ? ? ? ?"));
	if (!Orig_Free) return 3;

	Orig_FigureCleanup = reinterpret_cast<FigureCleanup_t>(PatternScan("48 89 5C 24 08 57 48 83 EC ? 48 8B D9 8B FA 48 8B 89 40 03 00 00 48 85 C9 74 ? E8 ? ? ? ? 48 8D 8B F8 02 00 00"));
	if (!Orig_FigureCleanup) return 4;

	Orig_FigureCleanup = reinterpret_cast<FigureCleanup_t>(TrampHook64((BYTE*)Orig_FigureCleanup, (BYTE*)hk_FigureCleanup, 15));
	if (!Orig_FigureCleanup) return 5;

	ThumbnailAlloc = VirtualAlloc(nullptr, THUMBNAIL_IMAGE_DATA_MAX_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	return 0;
}

bool ReplaceAvatarImage()
{
	if (CSaveDataManagerInstance == 0) return false;

	std::ofstream original(OriginalAvatarFileName, std::ios::out | std::ios::binary);
	if (original.is_open() && *(int*)(CSaveDataManagerInstance + AVATAR_IMAGE_DATA_OFFSET + AVATAR_IMAGE_DATA_SIZE_OFFSET) > 0)
	{
		original.write(reinterpret_cast<const char*>((BYTE*)(CSaveDataManagerInstance + AVATAR_IMAGE_DATA_OFFSET)), *(int*)(CSaveDataManagerInstance + AVATAR_IMAGE_DATA_OFFSET + AVATAR_IMAGE_DATA_SIZE_OFFSET));
	}
	original.close();

	std::ifstream image(AvatarFileName, std::ios::in | std::ios::binary | std::ios::ate);
	std::streamsize size = image.tellg();

	if (size <= AVATAR_IMAGE_DATA_MAX_SIZE && size > 0)
	{
		image.seekg(0, std::ios::beg);

		std::vector<char> buffer(size);
		if (image.read(buffer.data(), size))
		{
			memcpy_s((BYTE*)(CSaveDataManagerInstance + AVATAR_IMAGE_DATA_OFFSET), size, buffer.data(), size);
			*(int*)(CSaveDataManagerInstance + AVATAR_IMAGE_DATA_OFFSET + AVATAR_IMAGE_DATA_SIZE_OFFSET) = (int)size;
			*(BYTE*)(CSaveDataManagerInstance + AVATAR_IMAGE_DATA_OFFSET + AVATAR_IMAGE_DATA_VALID_OFFSET) = 1;

			return true;
		}
	}

	return false;
}

int AntiPNGBomb()
{
	Orig_CreateTransient = reinterpret_cast<CreateTransient_t>(PatternScan("4C 8B DC 53 55 56 41 55"));
	if (!Orig_CreateTransient) return 1;

	Orig_CreateTransient = reinterpret_cast<CreateTransient_t>(TrampHook64((BYTE*)Orig_CreateTransient, (BYTE*)hk_CreateTransient, 14));
	if (!Orig_CreateTransient) return 2;

	//UREDWidgetRCodePageOne::RefreshStatic - 48 85 C9 0F 84 ? ? ? ? 48 8B C4 44 89 48 20
	//UREDWidgetCommonAvatarFace::SetData_LocalUser
	//UREDWidgetCommonAvatarFace::SetData_OnlineUser


	RefreshSetLocal = PatternScan("E8 ? ? ? ? EB ? 49 8B 04 24 48 8D 54 24 40");
	if (!RefreshSetLocal) return 3;

	memcpy_s(Orig_RefreshSetLocal, 5, RefreshSetLocal, 5);

	RefreshSetOnline = PatternScan("E8 ? ? ? ? E8 ? ? ? ? 48 8B 4B 10 48 8B D0 E8 ? ? ? ? 84 C0 0F 84 ? ? ? ?");
	if (!RefreshSetOnline) return 4;

	memcpy_s(Orig_RefreshSetOnline, 5, RefreshSetOnline, 5);

	return 0;
}

#ifdef _DEBUG
int DLCUnlocker()
{
	void* Orig_IsUnlocked = PatternScan("40 53 55 56 57 48 83 EC ? 48 8B E9 48 8B D1");
	if (!Orig_IsUnlocked) return 1;

	bool bHooked = Detour64((BYTE*)Orig_IsUnlocked, (BYTE*)hk_IsUnlocked, 15);
	if (!bHooked) return 2;

	return 0;
}
#endif


int ColorUnlocker()
{
	Orig_IsSelectableCharaColorID = reinterpret_cast<IsSelectableCharaColorID_t>(PatternScan("48 89 5C 24 08 48 89 74 24 10 48 89 7C 24 18 55 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC ? 45 33 ED 8B FA"));
	if (!Orig_IsSelectableCharaColorID) return 1;
	
	Orig_IsSelectableCharaColorID = reinterpret_cast<IsSelectableCharaColorID_t>(TrampHook64((BYTE*)Orig_IsSelectableCharaColorID, (BYTE*)hk_IsSelectableCharaColorID, 15));
	if (!Orig_IsSelectableCharaColorID) return 2;

	//40 53 48 83 EC ? 48 8B D9 0F 29 74 24 30 48 8B 89 C0 08 00 00
	//AREDGameState::UpdateChangeScene()
	//48 89 4C 24 08 55 53 56 57 41 55 41 56 41 57 48 8B EC 48 83 EC ? 4C 8B F9 E8 ? ? ? ? 84 C0
	void* bArcadeEnemyOrbSurvivalEnemy = PatternScan("0F 85 ? ? ? ? 40 84 FF 0F 85 ? ? ? ? 45 84 F6");
	if (!bArcadeEnemyOrbSurvivalEnemy) return 3;

	BYTE bArcadeEnemyOrbSurvivalEnemyPatch[] = { 0xE9, 0x12, 0x01, 0x00, 0x00, 0x90 }; //jmp 0x117 (nop)

	Patch(bArcadeEnemyOrbSurvivalEnemyPatch, (BYTE*)bArcadeEnemyOrbSurvivalEnemy, sizeof(bArcadeEnemyOrbSurvivalEnemy));

	return 0;
}

void __fastcall hk_AddInGameCash(__int64 CSaveDataManager, int add)
{
	CSaveDataManagerInstance = CSaveDataManager;

	std::ofstream original(AvatarBackupFileName, std::ios::out | std::ios::binary);

	if (original.is_open() && *(int*)(CSaveDataManagerInstance + AVATAR_IMAGE_DATA_OFFSET + AVATAR_IMAGE_DATA_SIZE_OFFSET) > 0)
	{
		original.write(reinterpret_cast<const char*>((BYTE*)(CSaveDataManagerInstance + AVATAR_IMAGE_DATA_OFFSET)), *(int*)(CSaveDataManagerInstance + AVATAR_IMAGE_DATA_OFFSET + AVATAR_IMAGE_DATA_SIZE_OFFSET));
	}

	original.close();

	if (add < 0) add = 0;
	Orig_AddInGameCash(CSaveDataManager, add);
}

__int64 __fastcall hk_CheckRewardAura(__int64 UREDPlayerData)
{
	if (bGetRewardAuras)
	{
		Orig_SetRewardAvatarAura(UREDPlayerData, SelectedRewardAvatarAura);
		Orig_SetRewardNameAura(UREDPlayerData, SelectedRewardNameAura);
	}

	if (bGetRewardBadges)
	{
		for (int badgeID : BADGES)
		{
			Orig_SetRewardBadge(UREDPlayerData, badgeID, 1);
		}
	}

	//Min: -10, Max: 50, Negative Step: 8, Positive Step: 9, 9 * 7 = 63
	for (int i = 0; i < 7; i++)
	{
		Orig_UpdateOnlineCheatPt(UREDPlayerData, 1);
	}

	//Neutralize this function
	Detour64((BYTE*)Orig_UpdateOnlineCheatPt, (BYTE*)hk_UpdateOnlineCheatPt, 12);

	return 0;
}

void __fastcall hk_UpdateOnlineCheatPt(__int64 UREDPlayerData, char isMatchEnd)
{
	return;
}

__int64 __fastcall hk_SetToDefault_All(__int64 CSaveDataManager)
{
	CSaveDataManagerInstance = CSaveDataManager;
	__int64 returnVal = Orig_SetToDefault_All(CSaveDataManager);
	ReplaceAvatarImage();
	return returnVal;
}

__int64 __fastcall hk_ExportAvatarImage(__int64 UREDWidgetLobbyAvatarEditor)
{
	__int64 returnVal = Orig_ExportAvatarImage(UREDWidgetLobbyAvatarEditor);

	if (ReplaceAvatarImage())
	{
		*(BYTE*)(UREDWidgetLobbyAvatarEditor + UPLOAD_AVATAR_IMAGE_FLAG_OFFSET) = 1;
	}

	return returnVal;
}

__int64 __fastcall hk_CreateTransient(int InSizeX, int InSizeY, __int64 InFormat, __int64 InName)
{
	BYTE NOPPatch[] = { 0x90, 0x90, 0x90, 0x90, 0x90 };

	if (InSizeX > MAX_TEXTURE_AXIS_DIMENSION || InSizeY > MAX_TEXTURE_AXIS_DIMENSION)
	{
		Patch(NOPPatch, RefreshSetLocal, sizeof(NOPPatch));
		Patch(NOPPatch, RefreshSetOnline, sizeof(NOPPatch));
		return 0;
	}

	Patch(Orig_RefreshSetLocal, RefreshSetLocal, sizeof(Orig_RefreshSetLocal));
	Patch(Orig_RefreshSetOnline, RefreshSetOnline, sizeof(Orig_RefreshSetOnline));

	return Orig_CreateTransient(InSizeX, InSizeY, InFormat, InName);
}

__int64 __fastcall hk_GenerateThumbnail(__int64 Instance)
{
	__int64 retval = Orig_GenerateThumbnail(Instance);

	std::ifstream image(ThumbnailFileName, std::ios::in | std::ios::binary | std::ios::ate);
	std::streamsize size = image.tellg();

	if (size <= THUMBNAIL_IMAGE_DATA_MAX_SIZE && size > 0 && ThumbnailAlloc)
	{
		image.seekg(0, std::ios::beg);

		std::vector<char> buffer(size);
		if (image.read(buffer.data(), size))
		{
			OriginalThumbnail = *(__int64*)(Instance + THUMBNAIL_IMAGE_DATA_OFFSET);
			int OriginalThumbnailSize = *(int*)(Instance + THUMBNAIL_IMAGE_DATA_SIZE_OFFSET);

			std::ofstream original(OriginalThumbnailFileName, std::ios::out | std::ios::binary);

			if (original.is_open() && *(int*)(Instance + THUMBNAIL_IMAGE_DATA_SIZE_OFFSET) > 0)
			{
				original.write(reinterpret_cast<const char*>(OriginalThumbnail), OriginalThumbnailSize);
			}

			original.close();

			memcpy_s(ThumbnailAlloc, size, buffer.data(), size);

			*(__int64*)(Instance + THUMBNAIL_IMAGE_DATA_OFFSET) = (__int64)ThumbnailAlloc;
			*(int*)(Instance + THUMBNAIL_IMAGE_DATA_SIZE_OFFSET) = (int)size;
		}
	}

	return retval;
}

__int64 __fastcall hk_FigureCleanup(__int64 Instance, char a2)
{
	if (*(__int64*)(Instance + THUMBNAIL_IMAGE_DATA_OFFSET) == (__int64)ThumbnailAlloc && OriginalThumbnail != 0)
	{
		Orig_Free(OriginalThumbnail);
		OriginalThumbnail = 0;
		*(__int64*)(Instance + THUMBNAIL_IMAGE_DATA_OFFSET) = 0;
	}

	return Orig_FigureCleanup(Instance, a2);
}

char __fastcall hk_IsSelectableCharaColorID(unsigned int charaID, unsigned int colorID)
{
	bool bCustomUnlock = (colorID == SPCOLOR) ||
		(charaID == BAIKEN && colorID == ALTCOLOR) ||
		((charaID == SOL || charaID == KY) && colorID == EXCOLOR) ||
		((charaID == NAGO || charaID == INO || charaID == JACKO || charaID == ASUKA) && colorID == STORYCOLOR) ||
		((charaID == SOL || charaID == KY || charaID == AXL || charaID == SIN || charaID == UNIKA || charaID == HC ||
			charaID == BRIDGET || charaID == RAM || charaID == ELPHELT || charaID == ABA || charaID == MAY) && colorID == COLLABCOLOR) ||
		((colorID == HAUNTEDCOLOR) && (charaID == SOL || charaID == KY || charaID == MILLIA || charaID == GOLDLEWIS || charaID == HC || charaID == BAIKEN || charaID == ASUKA || charaID == JAM));

#ifdef _DEBUG
	bool bBaseUnlocked = (colorID >= COLORMIN && colorID <= COLORLIMIT);
#else
	bool bBaseUnlocked = Orig_IsSelectableCharaColorID(charaID, colorID);
#endif

	return bBaseUnlocked || bCustomUnlock;
}

#ifdef _DEBUG
__int64 __fastcall hk_IsUnlocked(const wchar_t* pDLCName)
{
	return true;
}
#endif
