#include "Features.h"
#include "Helpers.h"
#include "Constants.h"

bool UnlockColorSelection()
{
	BYTE* SelectionLimitIncrement = PatternScan("83 78 0C 4F 7E ? 89 58 0C");
	if (!SelectionLimitIncrement) return false;

	BYTE* SelectionLimitDecrement = PatternScan("C7 40 0C 4F ? ? ? 48 8B 45 30");
	if (!SelectionLimitIncrement) return false;

	BYTE* MiniSelectionLimitDecrement = PatternScan("C7 83 3C 04 00 00 4F ? ? ? BA ? ? ? ? EB");
	if (!MiniSelectionLimitDecrement) return false;

	BYTE* MiniSelectionLimitIncrement = PatternScan("83 FA 4F 7E ? 89 B3 3C 04 00 00");
	if (!MiniSelectionLimitIncrement) return false;

	BYTE* Orig_IsSelectableCharaColorID = PatternScan("48 89 5C 24 08 48 89 74 24 10 55 57 41 54 41 56 41 57 48 8B EC 48 83 EC ? 33 DB");
	if (!Orig_IsSelectableCharaColorID) return false;

	BYTE SelectionLimitIncrementPatch[] = { 0x83, 0x78, 0x0C, 0x62 }; //cmp dword ptr [rax+0Ch], 62h
	Patch(SelectionLimitIncrementPatch, SelectionLimitIncrement, sizeof(SelectionLimitIncrementPatch));

	BYTE SelectionLimitDecrementPatch[] = { 0xC7, 0x40, 0x0C, 0x62, 0x00, 0x00, 0x00 }; //mov dword ptr [rax+0Ch], 62h
	Patch(SelectionLimitDecrementPatch, SelectionLimitDecrement, sizeof(SelectionLimitDecrementPatch));

	BYTE MiniSelectionLimitDecrementPatch[] =
	{
		0xC7, 0x83, 0x3C, 0x04, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00, //mov dword ptr [rbx+43Ch], 62h
		0xBA, 0x62, 0x00, 0x00, 0x00								//mov edx, 62h
	};
	Patch(MiniSelectionLimitDecrementPatch, MiniSelectionLimitDecrement, sizeof(MiniSelectionLimitDecrementPatch));

	BYTE MiniSelectionLimitIncrementPatch[] = { 0x83, 0xFA, 0x62 }; //cmp edx, 62h
	Patch(MiniSelectionLimitIncrementPatch, MiniSelectionLimitIncrement, sizeof(MiniSelectionLimitIncrementPatch));

	Detour64(Orig_IsSelectableCharaColorID, (BYTE*)hk_IsSelectableCharaColorID, 12);

	return true;
}

bool UncensorMuseum()
{
	BYTE* MuseumFigureNSFWFlagSetter = PatternScan("0F B6 43 61 48 8B 5C 24 30");
	if (!MuseumFigureNSFWFlagSetter) return false;

	BYTE MuseumFigureNSFWFlagSetterPatch[] = { 0x30, 0xC0, 0x90, 0x90 }; //xor al, al (nop nop)
	Patch(MuseumFigureNSFWFlagSetterPatch, MuseumFigureNSFWFlagSetter, sizeof(MuseumFigureNSFWFlagSetterPatch));

	return true;
}

bool ImproveFishing()
{
	BYTE* HasRareFish = PatternScan("45 85 F6 75 ? 80 FA 0A");
	if (!HasRareFish) return false;

	BYTE* DecreaseMoney = PatternScan("44 89 81 24 35 03 00");
	if (!DecreaseMoney) return false;

	BYTE HasRareFishPatch[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	Patch(HasRareFishPatch, HasRareFish, sizeof(HasRareFishPatch));

	BYTE DecreaseMoneyPatch[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	Patch(DecreaseMoneyPatch, DecreaseMoney, sizeof(DecreaseMoneyPatch));

	return true;
}

bool UnlockAura()
{

	BYTE* Orig_CheckRewardAura = PatternScan("48 89 5C 24 20 55 56 57 48 83 EC ? 48 8B D9 48 8D 4C 24 40");
	if (!Orig_CheckRewardAura) return false;

	//Find them with: 48 83 EC ? 8D 42 FF 45 8B D0
	Orig_SetRewardAvatarAura = reinterpret_cast<SetRewardAvatarAura_t>(PatternScan("48 89 5C 24 10 57 48 83 EC ? 8B FA 48 8B D9 85 D2 0F 8E ? ? ? ? 48 8D 4C 24 40 48 89 74 24 30 E8 ? ? ? ? 48 8D 4C 24 40 E8 ? ? ? ? 8B F0 48 8D 4C 24 40 C1 E6 ? E8 ? ? ? ? 0B F0 48 8B 83 28 01 00 00 48 85 C0 74 ? 48 8B 15 ? ? ? ? 48 8D 4C 24 48 41 B8 ? ? ? ? E8 ? ? ? ? 48 8B 8B 28 01 00 00 44 8B C7 48 8B 10 E8 ? ? ? ? 48 8B 83 28 01 00 00 48 85 C0 74 ? 48 8B 15 ? ? ? ? 48 8D 4C 24 48 41 B8 ? ? ? ? E8 ? ? ? ? 48 8B 8B 28 01 00 00 44 8B C6 48 8B 10 E8 ? ? ? ? 48 8B 74 24 30 48 8B 5C 24 38 48 83 C4 ? 5F C3 48 8B 81 28 01 00 00 48 85 C0 74 ? 48 8B 15 ? ? ? ? 48 8D 4C 24 40 41 B8 ? ? ? ? E8 ? ? ? ? 48 8B 8B 28 01 00 00 45 33 C0 48 8B 10 E8 ? ? ? ? 48 8B 83 28 01 00 00 48 85 C0 74 ? 48 8B 15 ? ? ? ? 48 8D 4C 24 40 41 B8 ? ? ? ? E8 ? ? ? ? 48 8B 8B 28 01 00 00 45 33 C0 48 8B 10 E8 ? ? ? ? 48 8B 5C 24 38 48 83 C4 ? 5F C3 48 89 5C 24 10"));
	if (!Orig_SetRewardAvatarAura) return false;

	Orig_SetRewardNameAura = reinterpret_cast<SetRewardNameAura_t>(PatternScan("48 89 5C 24 10 57 48 83 EC ? 8B FA 48 8B D9 85 D2 0F 8E ? ? ? ? 48 8D 4C 24 40 48 89 74 24 30 E8 ? ? ? ? 48 8D 4C 24 40 E8 ? ? ? ? 8B F0 48 8D 4C 24 40 C1 E6 ? E8 ? ? ? ? 0B F0 48 8B 83 28 01 00 00 48 85 C0 74 ? 48 8B 15 ? ? ? ? 48 8D 4C 24 48 41 B8 ? ? ? ? E8 ? ? ? ? 48 8B 8B 28 01 00 00 44 8B C7 48 8B 10 E8 ? ? ? ? 48 8B 83 28 01 00 00 48 85 C0 74 ? 48 8B 15 ? ? ? ? 48 8D 4C 24 48 41 B8 ? ? ? ? E8 ? ? ? ? 48 8B 8B 28 01 00 00 44 8B C6 48 8B 10 E8 ? ? ? ? 48 8B 74 24 30 48 8B 5C 24 38 48 83 C4 ? 5F C3 48 8B 81 28 01 00 00 48 85 C0 74 ? 48 8B 15 ? ? ? ? 48 8D 4C 24 40 41 B8 ? ? ? ? E8 ? ? ? ? 48 8B 8B 28 01 00 00 45 33 C0 48 8B 10 E8 ? ? ? ? 48 8B 83 28 01 00 00 48 85 C0 74 ? 48 8B 15 ? ? ? ? 48 8D 4C 24 40 41 B8 ? ? ? ? E8 ? ? ? ? 48 8B 8B 28 01 00 00 45 33 C0 48 8B 10 E8 ? ? ? ? 48 8B 5C 24 38 48 83 C4 ? 5F C3 48 89 5C 24 08"));
	if (!Orig_SetRewardNameAura) return false;

	Orig_SetRewardBadge = reinterpret_cast<SetRewardBadge_t>(PatternScan("48 89 5C 24 10 55 56 41 54 41 56 41 57 48 83 EC"));
	if (!Orig_SetRewardBadge) return false;

	Detour64(Orig_CheckRewardAura, (BYTE*)hk_CheckRewardAura, 12);

	return true;
}

char __fastcall hk_IsSelectableCharaColorID(unsigned int charaID, unsigned int colorID)
{
	return (colorID >= 0 && colorID < 99);
}

__int64 __fastcall hk_CheckRewardAura(__int64 UREDPlayerData)
{
	__int64 BasicData = *(__int64*)((BYTE*)UREDPlayerData + 0x128);

	if (!BasicData) return 0;

	memcpy_s(SteamID, 16, (char*)(UREDPlayerData + 0x140), 16);

	Orig_SetRewardAvatarAura(UREDPlayerData, SelectedRewardAura);
	Orig_SetRewardNameAura(UREDPlayerData, SelectedRewardAura);
	
	if (GetRewardBadges)
	{
		Orig_SetRewardBadge(UREDPlayerData, 3009, 1); //Times in top badge
		Orig_SetRewardBadge(UREDPlayerData, 503009, 1); //Wins badge
	}

	return 0;
}
