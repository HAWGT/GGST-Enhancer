#pragma once

bool PatchIsSelectableCharaColorID();
bool UncensorMuseum();
bool PatchIsUnlocked();
bool ImproveFishing();

char __fastcall hk_IsSelectableCharaColorID(unsigned int charaID, unsigned int colorID);
__int64 __fastcall hk_IsUnlocked(__int64 pDLCName);
