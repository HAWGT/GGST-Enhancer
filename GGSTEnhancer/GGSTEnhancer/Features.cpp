#include "Features.h"
#include "Helpers.h"

bool PatchOnlineMultiplayerColorSelect()
{
	BYTE* ColorSetInitWidget = PatternScan("0F BE 44 08 03 89 83 3C 04 00 00 E8 ? ? ? ? 0F BE 88 83 00 00 00 89 8B 44 04 00 00 E8 ? ? ? ? 0F B7 88 84 00 00 00");
	if (!ColorSetInitWidget) return false;

	BYTE* ColorSetSelectCharacter = PatternScan("C7 82 3C 04 00 00 ? ? ? ? 33 D2");
	if (!ColorSetSelectCharacter) return false;

	BYTE PatchColorSetInitWidget[] = { 0xB8, 0x59, 0x00, 0x00, 0x00 }; //mov eax, 59h
	Patch(PatchColorSetInitWidget, ColorSetInitWidget, 5);

	BYTE PatchColorSetSelectCharacter[] = { 0xC7, 0x82, 0x3C, 0x04, 0x00, 0x00, 0x59, 0x00, 0x00, 0x00 }; //mov [rdx+0000043C],00000059h
	Patch(PatchColorSetSelectCharacter, ColorSetSelectCharacter, 10);

	return true;
}

bool UncensorMuseum()
{
	BYTE* MuseumFigureNSFWFlagSetter = PatternScan("0F B6 43 61 48 8B 5C 24 30");
	if (!MuseumFigureNSFWFlagSetter) return false;

	BYTE MuseumFigureNSFWFlagSetterPatch[] = { 0x30, 0xC0, 0x90, 0x90 }; //xor al, al (nop nop)
	Patch(MuseumFigureNSFWFlagSetterPatch, MuseumFigureNSFWFlagSetter, 4);

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

	BYTE PatchHasRareFish[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	Patch(PatchHasRareFish, HasRareFish, 10);

	BYTE PatchDecreaseMoney[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	Patch(PatchDecreaseMoney, DecreaseMoney, 7);

	return true;
}

__int64 __fastcall hk_IsUnlocked(__int64 a1)
{
	return 1;
}
