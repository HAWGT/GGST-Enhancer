using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Security;
using System.Threading;
using System.Diagnostics;
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
        [DllImport("kernel32.dll")]
        private static extern bool VirtualProtectEx(IntPtr hProcess, IntPtr lpAddress, UIntPtr dwSize, uint flNewProtect, out uint lpflOldProtect);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, uint nSize, out int lpNumberOfBytesWritten);

        const string procName = "GGST-Win64-Shipping";

        const string colorPSNPatternOffset = "GGST-Win64-Shipping.exe+0xDD7094";
        static byte[] colorPSNPattern = { 0x74, 0x08, 0x48, 0x8B, 0xCF, 0xE8, 0xF2, 0x7A, 0x23, 0x00, 0x0F, 0xB6, 0xC3 };

        const string colorSPPattern1Offset = "GGST-Win64-Shipping.exe+0xBE9F51";
        static byte[] colorSPPattern1 = { 0xE8, 0x0A, 0x75, 0xF8, 0xFF, 0x48, 0x63, 0x8F, 0x28, 0x04, 0x00, 0x00, 0x0F, 0xBE, 0x44, 0x08, 0x03 };

        const string colorSPPattern2Offset = "GGST-Win64-Shipping.exe+0xBE1111";
        static byte[] colorSPPattern2 = { 0x74, 0x24, 0x83, 0xBA, 0x2C, 0x04, 0x00, 0x00, 0x00, 0x74, 0x1B, 0xC7, 0x82, 0x2C, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };


        static byte[] patchPSN = {
            0x74, 0x80,                     //  je current+8
            0x48, 0x8B, 0xCF,               //  mov rcx,rdi
            0xE8, 0xF2, 0x7A, 0x23, 0x00,   //  call 
            0x90, 0x90, 0x90,               //  NOP NOP NOP - Replacing movzx eax,bl - PSN COLOR CHECK
        };

        static byte[] patchSP1 =
        {
            0xE8, 0x0A, 0x75, 0xF8, 0xFF,               //  call func
            0x48, 0x63, 0x8F, 0x28, 0x04, 0x00, 0x00,   //  movsxd  rcx,dword ptr [rdi+00000428]
            0xB8, 0x59, 0x00, 0x00, 0x00,               //  mov eax,59 - Replacing movsx eax,byte ptr [rax+rcx+03] - SP COLOR OPEN SELECT
        };

        static byte[] patchSP2 =
        {
            0x74, 0x24,                                                 //  je current+24
            0x83, 0xBA, 0x2C, 0x04, 0x00, 0x00, 0x00,                   //  cmp cmp dword ptr [rdx+0000042C],00
            0x74, 0x1B,                                                 //  je current+1B
            0xC7, 0x82, 0x2C, 0x04, 0x00, 0x00, 0x59, 0x00, 0x00, 0x00  //  mov [rdx+0000042C],00000059 - Replacing mov [rdx+0000042C],00000000 - SP COLOR CHANGE CHARACTER
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
            Console.WriteLine("Expected game version: 1.09\n");
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
                    if (!bPatched && !bIsPatching && bProcOpen) CreatePatch();
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
            Stopwatch s = new Stopwatch();
            s.Start();

            bIsPatching = true;

            long psnCheckAddr = 0x0;
            long spCheckAddr1 = 0x0;
            long spCheckAddr2 = 0x0;

            bool bFound1 = false;
            bool bFound2 = false;
            bool bFound3 = false;

            // PSN Color
            // Check if offset is correct
            if (BytesEqual(m.ReadBytes(colorPSNPatternOffset, colorPSNPattern.Length), colorPSNPattern))
            {
                psnCheckAddr = (long)m.Get64BitCode(colorPSNPatternOffset);
                Console.WriteLine("Offset Correct, Address from PSN color check: {0:X}", psnCheckAddr);
                bFound1 = true;
            }
            // Otherwise AOB Scan for bytes
            else
            {
                IEnumerable<long> addresses1 = await m.AoBScan(BytesToString(colorPSNPattern), true, true);
                foreach (long result in addresses1)
                {
                    psnCheckAddr = result;
                    Console.WriteLine("Address from PSN color check: {0:X}", psnCheckAddr);
                    bFound1 = true;
                    break;
                }
            }

            // Special Color 1
            // Check if offset is correct
            if (BytesEqual(m.ReadBytes(colorSPPattern1Offset, colorSPPattern1.Length), colorSPPattern1))
            {
                spCheckAddr1 = (long)m.Get64BitCode(colorSPPattern1Offset);
                Console.WriteLine("Offset Correct, Address from menu color check: {0:X}", spCheckAddr1);
                bFound2 = true;
            }
            // Otherwise AOB Scan for bytes
            else
            {
                IEnumerable<long> addresses2 = await m.AoBScan(BytesToString(colorSPPattern1), true, true);
                foreach (long result in addresses2)
                {
                    spCheckAddr1 = result;
                    Console.WriteLine("Address from menu color load: {0:X}", spCheckAddr1);
                    bFound2 = true;
                    break;
                }
            }

            // Special Color 2
            // Check if offset is correct
            if (BytesEqual(m.ReadBytes(colorSPPattern2Offset, colorSPPattern2.Length), colorSPPattern2))
            {
                spCheckAddr2 = (long)m.Get64BitCode(colorSPPattern2Offset);
                Console.WriteLine("Offset Correct, Address from character change color check: {0:X}", spCheckAddr2);
                bFound3 = true;
            }
            // Otherwise AOB Scan for bytes
            else
            {
                IEnumerable<long> addresses3 = await m.AoBScan(BytesToString(colorSPPattern2), true, true);
                foreach (long result in addresses3)
                {
                    spCheckAddr2 = result;
                    Console.WriteLine("Address from character change color load: {0:X}", spCheckAddr2);
                    bFound3 = true;
                    break;
                }
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

            s.Stop();
            Console.WriteLine("Game process patched in {0} seconds",s.Elapsed.TotalSeconds);

        }
        private static bool BytesEqual(byte[] a, byte[] b)
        {
            if (a.Length == b.Length)
            {
                for (int i = 0; i < a.Length; i++)
                {
                    if (a[i] != b[i])
                    {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }
        static string BytesToString(byte[] bytes)
        {
            string r = "";
            for (int i = 0; i < bytes.Length - 1; i++)
            {
                r += bytes[i].ToString("X2") + " ";
            }
            return r + bytes[bytes.Length - 1].ToString("X2");
        }
        private static void PatchAddr(IntPtr handle, long addr, byte[] bytes)
        {
            VirtualProtectEx(handle, (IntPtr)addr, (UIntPtr)bytes.Length, 0x40 /* EXECUTE_READWRITE */, out uint oldProtect);

            WriteProcessMemory(handle, (IntPtr)addr, bytes, (uint)bytes.Length, out _);

            VirtualProtectEx(handle, (IntPtr)addr, (UIntPtr)bytes.Length, oldProtect, out _);
        }

    }
}
