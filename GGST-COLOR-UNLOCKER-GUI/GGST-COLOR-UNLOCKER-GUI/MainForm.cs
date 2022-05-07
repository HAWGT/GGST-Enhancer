using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Diagnostics;
using Memory;
using System.Runtime.InteropServices;

namespace GGST_COLOR_UNLOCKER_GUI
{
    public partial class MainForm : Form
    {
        [DllImport("kernel32.dll")]
        private static extern bool VirtualProtectEx(IntPtr hProcess, IntPtr lpAddress, UIntPtr dwSize, uint flNewProtect, out uint lpflOldProtect);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, uint nSize, out int lpNumberOfBytesWritten);

        const string procName = "GGST-Win64-Shipping";

        const string colorPSNPatternOffset = "GGST-Win64-Shipping.exe+0xEA55A4";
        static byte[] colorPSNPattern = { 0x74, 0x08, 0x48, 0x8B, 0xCF, 0xE8, 0x02, 0x46, 0x25, 0x00, 0x0F, 0xB6, 0xC3 };

        const string colorSPPattern1Offset = "GGST-Win64-Shipping.exe+0xC17D01";
        static byte[] colorSPPattern1 = { 0xE8, 0xBA, 0x97, 0xF7, 0xFF, 0x48, 0x63, 0x8B, 0x38, 0x04, 0x00, 0x00, 0x0F, 0xBE, 0x44, 0x08, 0x03 };

        const string colorSPPattern2Offset = "GGST-Win64-Shipping.exe+0xC0A6CE";
        static byte[] colorSPPattern2 = { 0x74, 0x24, 0x83, 0xBA, 0x3C, 0x04, 0x00, 0x00, 0x00, 0x74, 0x1B, 0xC7, 0x82, 0x3C, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

        static long psnCheckAddr = 0x0;
        static long spCheckAddr1 = 0x0;
        static long spCheckAddr2 = 0x0;


        static byte[] patchPSN = {
            0x74, 0x08,                     //  je current+8
            0x48, 0x8B, 0xCF,               //  mov rcx,rdi
            0xE8, 0x02, 0x46, 0x25, 0x00,   //  call func
            0x90, 0x90, 0x90,               //  NOP NOP NOP - Replacing movzx eax,bl - PSN COLOR CHECK
        };

        static byte[] patchSP1 =
        {
            0xE8, 0xBA, 0x97, 0xF7, 0xFF,               //  call func
            0x48, 0x63, 0x8B, 0x38, 0x04, 0x00, 0x00,   //  movsxd  rcx,dword ptr [rbx+00000438]
            0xB8, 0x59, 0x00, 0x00, 0x00,               //  mov eax,59 - Replacing movsx eax,byte ptr [rax+rcx+03] - SP COLOR OPEN SELECT
        };

        static byte[] patchSP2 =
        {
            0x74, 0x24,                                                 //  je current+24
            0x83, 0xBA, 0x3C, 0x04, 0x00, 0x00, 0x00,                   //  cmp cmp dword ptr [rdx+0000042C],00
            0x74, 0x1B,                                                 //  je current+1B
            0xC7, 0x82, 0x3C, 0x04, 0x00, 0x00, 0x59, 0x00, 0x00, 0x00  //  mov [rdx+0000042C],00000059 - Replacing mov [rdx+0000042C],00000000 - SP COLOR CHANGE CHARACTER
        };


        static Mem m = new Mem();
        static int pID = 0;
        static bool bProcOpen = false;
        static bool bOldState = false;
        static bool bPatched = false;
        static bool bIsPatching = false;


        public MainForm()
        {
            InitializeComponent();
        }

        private void Main_Load(object sender, EventArgs e)
        {
            background_find_game_process.DoWork += new DoWorkEventHandler(background_find_game_process_DoWork);
            if (!background_find_game_process.IsBusy) background_find_game_process.RunWorkerAsync();
        }

        private void background_find_game_process_DoWork(object sender, DoWorkEventArgs e)
        {
            while (true)
            {

                btn_force.Invoke((MethodInvoker)delegate
                {
                    btn_force.Enabled = bProcOpen;
                });

                bOldState = bProcOpen;
                pID = m.GetProcIdFromName(procName);

                lbl_pid.Invoke((MethodInvoker)delegate
                {
                    lbl_pid.Text = pID.ToString();
                });
                if (pID > 0)
                {
                    bProcOpen = m.OpenProcess(pID);

                    lbl_status.Invoke((MethodInvoker)delegate
                    {
                        lbl_status.Text = "RUNNING";
                        lbl_status.ForeColor = Color.Green;
                    });
                    if (!bOldState && bProcOpen)
                    {
                        bPatched = false;
                        bIsPatching = false;
                    }
                    if (!bPatched && !bIsPatching && bProcOpen) CreatePatch();
                }
                else
                {

                    lbl_status.Invoke((MethodInvoker)delegate
                    {
                        lbl_status.Text = "NOT RUNNING";
                        lbl_status.ForeColor = Color.Red;
                    });
                    bProcOpen = false;
                    bPatched = false;
                    bIsPatching = false;
                }
            }
        }

        private static async void CreatePatch()
        {
            Stopwatch s = new Stopwatch();
            s.Start();

            bIsPatching = true;

            bool bFound1 = false;
            bool bFound2 = false;
            bool bFound3 = false;

            // PSN Color
            // Check if offset is correct

            if (BytesEqual(m.ReadBytes(colorPSNPatternOffset, colorPSNPattern.Length), colorPSNPattern))
            {
                psnCheckAddr = (long)m.Get64BitCode(colorPSNPatternOffset);
                bFound1 = true;
                //MessageBox.Show("Found PSN");
            }
            // Otherwise AOB Scan for bytes
            else
            {
                IEnumerable<long> addresses1 = await m.AoBScan(BytesToString(colorPSNPattern), true, true);
                foreach (long result in addresses1)
                {
                    psnCheckAddr = result;
                    bFound1 = true;
                    //MessageBox.Show("Found PSN");
                    break;
                }
            }

            // Special Color 1
            // Check if offset is correct
            if (BytesEqual(m.ReadBytes(colorSPPattern1Offset, colorSPPattern1.Length), colorSPPattern1))
            {
                spCheckAddr1 = (long)m.Get64BitCode(colorSPPattern1Offset);
                bFound2 = true;
                //MessageBox.Show("Found SP1");
            }
            // Otherwise AOB Scan for bytes
            else
            {
                IEnumerable<long> addresses2 = await m.AoBScan(BytesToString(colorSPPattern1), true, true);
                foreach (long result in addresses2)
                {
                    spCheckAddr1 = result;
                    bFound2 = true;
                    //MessageBox.Show("Found SP1");
                    break;
                }
            }

            // Special Color 2
            // Check if offset is correct
            if (BytesEqual(m.ReadBytes(colorSPPattern2Offset, colorSPPattern2.Length), colorSPPattern2))
            {
                spCheckAddr2 = (long)m.Get64BitCode(colorSPPattern2Offset);
                bFound3 = true;
                //MessageBox.Show("Found SP2");
            }
            // Otherwise AOB Scan for bytes
            else
            {
                IEnumerable<long> addresses3 = await m.AoBScan(BytesToString(colorSPPattern2), true, true);
                foreach (long result in addresses3)
                {
                    spCheckAddr2 = result;
                    bFound3 = true;
                    //MessageBox.Show("Found SP2");
                    break;
                }
            }

            if (!bFound1 || !bFound2 || !bFound3)
            {
                bIsPatching = false;
                return;
            }

            PatchAddr(m.mProc.Handle, psnCheckAddr, patchPSN);
            PatchAddr(m.mProc.Handle, spCheckAddr1, patchSP1);
            PatchAddr(m.mProc.Handle, spCheckAddr2, patchSP2);

            bPatched = true;
            bIsPatching = false;

            s.Stop();

            MessageBox.Show("Game process patched in " + s.Elapsed.TotalSeconds.ToString() + " seconds!");

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

        private void btn_force_Click(object sender, EventArgs e)
        {
            if (bProcOpen && bPatched && spCheckAddr1 > 0 && spCheckAddr2 > 0)
            {
                byte[] patchSP1 =
                {
                    0xE8, 0xBA, 0x97, 0xF7, 0xFF,
                    0x48, 0x63, 0x8B, 0x38, 0x04, 0x00, 0x00,
                    0xB8, (byte)num_color.Value, 0x00, 0x00, 0x00,
                };

                byte[] patchSP2 =
                {
                    0x74, 0x24,
                    0x83, 0xBA, 0x3C, 0x04, 0x00, 0x00, 0x00,
                    0x74, 0x1B,
                    0xC7, 0x82, 0x3C, 0x04, 0x00, 0x00, (byte)num_color.Value, 0x00, 0x00, 0x00
                };

                PatchAddr(m.mProc.Handle, spCheckAddr1, patchSP1);
                PatchAddr(m.mProc.Handle, spCheckAddr2, patchSP2);

                MessageBox.Show("Updated Color!");
            }
        }

        private void btn_info_Click(object sender, EventArgs e)
        {
            MessageBox.Show("This is meant for game version 1.17\nWhen choosing a color, you subtract 1 to get the correct color ingame.");
        }
    }
}
