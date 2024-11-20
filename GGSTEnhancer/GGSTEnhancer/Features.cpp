#include "Features.h"
#include "Helpers.h"
#include "Badges.h"

#include <fstream>
#include <filesystem>
#include <iostream>

inline BYTE* RefreshSetLocal = nullptr;
inline BYTE* RefreshSetOnline = nullptr;

inline BYTE Orig_RefreshSetLocal[] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
inline BYTE Orig_RefreshSetOnline[] = { 0x00, 0x00, 0x00, 0x00, 0x00 };

bool UncensorMuseum()
{
	BYTE* MuseumFigureNSFWFlagSetterList = PatternScan("0F B6 43 61 48 8B 5C 24 30");
	if (!MuseumFigureNSFWFlagSetterList) return false;

	BYTE* MuseumFigureNSFWFlagSetterModal = PatternScan("0F B6 43 61 88 47 61 0F B6 43 62");
	if (!MuseumFigureNSFWFlagSetterModal) return false;

	BYTE MuseumFigureNSFWFlagSetterPatch[] = { 0x30, 0xC0, 0x90, 0x90 }; //xor al, al (nop nop)

	Patch(MuseumFigureNSFWFlagSetterPatch, MuseumFigureNSFWFlagSetterList, sizeof(MuseumFigureNSFWFlagSetterPatch));
	Patch(MuseumFigureNSFWFlagSetterPatch, MuseumFigureNSFWFlagSetterModal, sizeof(MuseumFigureNSFWFlagSetterPatch));

	return true;
}

bool ImproveFishing()
{
	BYTE* HasRareFish = PatternScan("45 85 F6 75 ? 80 FA 0A");
	if (!HasRareFish) return false;

	BYTE HasRareFishPatch[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	Patch(HasRareFishPatch, HasRareFish, sizeof(HasRareFishPatch));

	Orig_AddInGameCash = reinterpret_cast<AddInGameCash_t>(PatternScan("40 53 48 83 EC ? 48 63 81 24 35 03 00"));
	if (!Orig_AddInGameCash) return false;
	Orig_AddInGameCash = reinterpret_cast<AddInGameCash_t>(TrampHook64((BYTE*)Orig_AddInGameCash, (BYTE*)hk_AddInGameCash, 19)); //No need for this many bytes, but it makes it easier to visualize and debug
	if (!Orig_AddInGameCash) return false;

	return true;
}

bool UnlockRewards()
{
	BYTE* Orig_CheckRewardAura = PatternScan("48 89 5C 24 20 55 56 57 48 83 EC ? 48 8B D9 48 8D 4C 24 40");
	if (!Orig_CheckRewardAura) return false;

	//Find them with: 48 83 EC ? 8D 42 FF 45 8B D0 (NetworkGiftManager::AddSaveDataParam)

	//Case 2:
	Orig_SetRewardAvatarAura = reinterpret_cast<SetRewardAvatarAura_t>(GetAddressFromInstruction((uintptr_t)PatternScan("E8 ? ? ? ? B0 ? 48 83 C4 ? C3 48 83 3D ? ? ? ? ? 0F 84 ? ? ? ? 41 8D 42 FF 83 F8 ? 0F 87 ? ? ? ? 48 8B 0D ? ? ? ? 41 8B D2 48 8B 89 18 0B 00 00 E8 ? ? ? ?"), 5));
	if (!Orig_SetRewardAvatarAura) return false;

	//Case 3:
	Orig_SetRewardNameAura = reinterpret_cast<SetRewardNameAura_t>(GetAddressFromInstruction((uintptr_t)PatternScan("E8 ? ? ? ? B0 ? 48 83 C4 ? C3 48 83 3D ? ? ? ? ? 0F 84 ? ? ? ? 45 85 D2"), 5));
	if (!Orig_SetRewardNameAura) return false;

	//Case 4:
	Orig_SetRewardBadge = reinterpret_cast<SetRewardBadge_t>(PatternScan("48 89 5C 24 10 55 56 41 54 41 56 41 57 48 83 EC ?"));
	if (!Orig_SetRewardBadge) return false;

	Orig_UpdateOnlineCheatPt = reinterpret_cast<UpdateOnlineCheatPt_t>(PatternScan("48 89 5C 24 18 57 48 83 EC ? 48 83 B9 40 01 00 00 ?"));
	if (!Orig_UpdateOnlineCheatPt) return false;

	Detour64(Orig_CheckRewardAura, (BYTE*)hk_CheckRewardAura, 12);

	return true;
}

bool CustomAvatarImage()
{
	Orig_SetToDefault_All = reinterpret_cast<SetToDefault_All_t>(PatternScan("48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 41 56 41 57 48 83 EC ? 45 33 FF 48 8D B9 D4 00 00 00"));
	if (!Orig_SetToDefault_All) return false;

	Orig_ExportAvatarImage = reinterpret_cast<ExportAvatarImage_t>(PatternScan("40 55 48 8D 6C 24 A9 48 81 EC ? ? ? ? 48 8B 01"));
	if (!Orig_ExportAvatarImage) return false;

	Orig_ExportAvatarImage = reinterpret_cast<ExportAvatarImage_t>(TrampHook64((BYTE*)Orig_ExportAvatarImage, (BYTE*)hk_ExportAvatarImage, 14));
	if (!Orig_ExportAvatarImage) return false;

	Orig_SetToDefault_All = reinterpret_cast<SetToDefault_All_t>(TrampHook64((BYTE*)Orig_SetToDefault_All, (BYTE*)hk_SetToDefault_All, 15));
	if (!Orig_SetToDefault_All) return false;

	return true;
}

bool CustomThumbnail()
{
	Orig_GenerateThumbnail = reinterpret_cast<GenerateThumbnail_t>(PatternScan("40 56 48 83 EC ? 48 89 5C 24 58 0F 57 C9"));
	if (!Orig_GenerateThumbnail) return false;

	Orig_GenerateThumbnail = reinterpret_cast<GenerateThumbnail_t>(TrampHook64((BYTE*)Orig_GenerateThumbnail, (BYTE*)hk_GenerateThumbnail, 14));
	if (!Orig_GenerateThumbnail) return false;

	Orig_Free = reinterpret_cast<Free_t>(PatternScan("48 85 C9 74 ? 53 48 83 EC ? 48 8B D9 48 8B 0D ? ? ? ?"));
	if (!Orig_Free) return false;

	Orig_FigureCleanup = reinterpret_cast<FigureCleanup_t>(PatternScan("48 89 5C 24 08 57 48 83 EC ? 48 8B D9 8B FA 48 8B 89 40 03 00 00 48 85 C9 74 ? E8 ? ? ? ? 48 8D 8B F8 02 00 00"));
	if (!Orig_FigureCleanup) return false;

	Orig_FigureCleanup = reinterpret_cast<FigureCleanup_t>(TrampHook64((BYTE*)Orig_FigureCleanup, (BYTE*)hk_FigureCleanup, 13));
	if (!Orig_FigureCleanup) return false;

	ThumbnailAlloc = VirtualAlloc(nullptr, THUMBNAIL_IMAGE_DATA_MAX_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	return true;
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

bool AntiPNGBomb()
{
	Orig_CreateTransient = reinterpret_cast<CreateTransient_t>(PatternScan("4C 8B DC 53 55 56 41 55"));
	if (!Orig_CreateTransient) return false;

	Orig_CreateTransient = reinterpret_cast<CreateTransient_t>(TrampHook64((BYTE*)Orig_CreateTransient, (BYTE*)hk_CreateTransient, 14));
	if (!Orig_CreateTransient) return false;

	RefreshSetLocal = PatternScan("E8 ? ? ? ? EB ? 49 8B 06 48 8D 55 C7");
	if (!RefreshSetLocal) return false;

	memcpy_s(Orig_RefreshSetLocal, 5, RefreshSetLocal, 5);

	RefreshSetOnline = PatternScan("E8 ? ? ? ? E8 ? ? ? ? 48 8B 4B 10 48 8B D0 E8 ? ? ? ? 84 C0 0F 84 ? ? ? ?");
	if (!RefreshSetOnline) return false;

	memcpy_s(Orig_RefreshSetOnline, 5, RefreshSetOnline, 5);

	return true;
}

void __fastcall hk_AddInGameCash(__int64 CSaveDataManager, int add)
{
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
