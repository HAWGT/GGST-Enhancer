#include "Features.h"
#include "Helpers.h"

bool PatchIsSelectableCharaColorID()
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

	Detour64(Orig_IsSelectableCharaColorID, (BYTE*)&hk_IsUnlocked, 12);

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

bool PatchIsUnlocked()
{
	BYTE* Orig_IsUnlocked = PatternScan("40 53 55 56 57 48 83 EC ? 48 8B E9 48 8B D1");
	if (!Orig_IsUnlocked) return false;

	Detour64(Orig_IsUnlocked, (BYTE*)&hk_IsUnlocked, 12);

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

char __fastcall hk_IsSelectableCharaColorID(unsigned int charaID, unsigned int colorID)
{
	return (colorID >= 0 && colorID < 99);
}

__int64 __fastcall hk_IsUnlocked(__int64 pDLCName)
{
	return 1;
}
