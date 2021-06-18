using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Security;
using System.Threading;
using Memory;

namespace GGST_Color_Unlocker
{


    /*
     * Guilty Gear Strive Color Unlocker for PSN & SP Colors
     * Reversed game with Cheat Engine with VEH Debugger, otherwise the game will crash
     * Created by HAWGT
     */

    class Program
    {

        [DllImport("user32.dll")]
        public static extern int GetAsyncKeyState(Int32 i);

        [SuppressUnmanagedCodeSecurity]
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate int AssemblyAddFunction(int x, int y);

        [DllImport("kernel32.dll")]
        private static extern bool VirtualProtectEx(IntPtr hProcess, IntPtr lpAddress, UIntPtr dwSize, uint flNewProtect, out uint lpflOldProtect);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, uint nSize, out int lpNumberOfBytesWritten);

        const string procName = "GGST-Win64-Shipping";

        const string colorPSNPattern = "74 08 48 8B CF E8 22 E9 22 00 0F B6 C3";
        const string colorSPPattern1 = "E8 BA 7B F8 FF 48 63 8F 28 04 00 00 0F BE 44 08 03";
        const string colorSPPattern2 = "74 24 83 BA 2C 04 00 00 00 74 1B C7 82 2C 04 00 00 00 00 00 00";

        static byte[] patchPSN = {
            0x74, 0x80,                     //  je current+8
            0x48, 0x8B, 0xCF,               //  mov rcx,rdi
            0xE8, 0x22, 0xE9, 0x22, 0x00,   //  call 
            0x90, 0x90, 0x90,               //  NOP NOP NOP - Replacing movzx eax,bl
        };

        static byte[] patchSP1 =
        {
            0xE8, 0xBA, 0x7B, 0xF8, 0xFF,               //  call func
            0x48, 0x63, 0x8F, 0x28, 0x04, 0x00, 0x00,   //  movsxd  rcx,dword ptr [rdi+00000428]
            0xB8, 0x59, 0x00, 0x00, 0x00,               //  mov eax,59 - Replacing movsx eax,byte ptr [rax+rcx+03] SP COLOR OPEN SELECT
        };

        static byte[] patchSP2 =
        {
            0x74, 0x24,                                                 //  je current+24
            0x83, 0xBA, 0x2C, 0x04, 0x00, 0x00, 0x00,                   //  cmp cmp dword ptr [rdx+0000042C],00
            0x74, 0x1B,                                                 //  je current+1B
            0xC7, 0x82, 0x2C, 0x04, 0x00, 0x00, 0x59, 0x00, 0x00, 0x00  //  mov [rdx+0000042C],00000059 - Replacing mov [rdx+0000042C],00000000 SP COLOR CHANGE CHARACTER
        };

        static Mem m = new Mem();
        static Thread thread;
        static int pID = 0;
        static bool bProcOpen = false;
        static bool bOldState = false;
        static bool bQuit = false;
        static bool bPatched = false;
        static bool bIsPatching = false;
        static bool bFirstPatch = true;

        static void Main(string[] args)
        {
            Console.WriteLine("+++++++++++++++++++++++++++++++++");
            Console.WriteLine("+------GGST COLOR UNLOCKER------+");
            Console.WriteLine("+++++++++++++++++++++++++++++++++\n");
            Console.WriteLine("Expected game version: 1.03\n");
            Console.WriteLine("You may close this window after the patch is applied.");
            Console.WriteLine("You must reapply this patch on each launch.");
            Console.WriteLine("Waiting for the game process...\n");
            Console.WriteLine("Log:\n");
            thread = new Thread(Routine);
            thread.Start();
        }

        static void Routine()
        {
            while (!bQuit)
            {
                bOldState = bProcOpen;
                pID = m.GetProcIdFromName(procName);
                if (pID > 0)
                {
                    bProcOpen = m.OpenProcess(pID);
                    if (!bOldState && bProcOpen)
                    {
                        bPatched = false;
                        bIsPatching = false;
                        bFirstPatch = true;
                        Console.WriteLine("Game process found! PID: " + pID + "");
                    }
                    if (!bPatched && !bIsPatching &&bProcOpen) CreatePatch();
                }
                else
                {
                    bProcOpen = false;
                    bPatched = false;
                    bIsPatching = false;
                    bFirstPatch = true;
                    if (bOldState && !bProcOpen) Console.WriteLine("Game process closed!");
                }
            }
        }

        private static async void CreatePatch()
        {
            bIsPatching = true;

            IEnumerable<long> addresses1 = await m.AoBScan(colorPSNPattern, true, true);
            IEnumerable<long> addresses2 = await m.AoBScan(colorSPPattern1, true, true);
            IEnumerable<long> addresses3 = await m.AoBScan(colorSPPattern2, true, true);

            bool bFound1 = false;
            bool bFound2 = false;
            bool bFound3 = false;

            long psnCheckAddr = 0x0;
            long spCheckAddr1 = 0x0;
            long spCheckAddr2 = 0x0;

            foreach (long result in addresses1)
            {
                psnCheckAddr = result;
                Console.WriteLine("Address from PSN color check: {0:X}", psnCheckAddr);
                bFound1 = true;
                break;
            }

            foreach (long result in addresses2)
            {
                spCheckAddr1 = result;
                Console.WriteLine("Address from menu color load: {0:X}", spCheckAddr1);
                bFound2 = true;
                break;
            }

            foreach (long result in addresses3)
            {
                spCheckAddr2 = result;
                Console.WriteLine("Address from character change color load: {0:X}", spCheckAddr2);
                bFound3 = true;
                break;
            }

            if (!bFound1 || !bFound2 || !bFound3)
            {
                if (!bFirstPatch) Console.WriteLine("Warning: Couldn't find all patterns! Already patched or different game version?");
                bFirstPatch = false;
                bIsPatching = false;
                return;
            }

            PatchAddr(m.pHandle, psnCheckAddr, patchPSN);
            PatchAddr(m.pHandle, spCheckAddr1, patchSP1);
            PatchAddr(m.pHandle, spCheckAddr2, patchSP2);

            bPatched = true;
            bIsPatching = false;

            Console.WriteLine("Game process patched!");

        }

        private static void PatchAddr(IntPtr handle, long addr, byte[] bytes)
        {
            VirtualProtectEx(handle, (IntPtr)addr, (UIntPtr)bytes.Length, 0x40 /* EXECUTE_READWRITE */, out uint oldProtect);

            WriteProcessMemory(handle, (IntPtr)addr, bytes, (uint)bytes.Length, out _);

            VirtualProtectEx(handle, (IntPtr)addr, (UIntPtr)bytes.Length, oldProtect, out _);
        }

    }
}
