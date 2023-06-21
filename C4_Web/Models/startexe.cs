using Microsoft.AspNetCore.Mvc;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace C4_Web.Models
{

    public class TTS
    {
        private static ProcessStartInfo startInfo;
        private static Process proc;

        public static int num;

        public static void init()
        {
            startInfo = new ProcessStartInfo();
            startInfo.FileName = "D:\\c4\\MoeGoe\\MoeGoe\\MoeGoe.exe";
            startInfo.RedirectStandardInput = true;
            startInfo.RedirectStandardOutput = true;
            startInfo.CreateNoWindow = true;
            startInfo.UseShellExecute = false;

            proc = new Process();
            proc.StartInfo = startInfo;
            proc.Start();

            string[] comm = new string[] { "D:\\c4\\Mahiro-VITS\\G_latest.pth", "D:\\c4\\Mahiro-VITS\\finetune_speaker.json" };
            foreach (string str in comm)
            {
                proc.StandardInput.WriteLine(str);
            }
        }

        public static void start(string filePath)
        {
            if (filePath != null)
            {

                const int speak_id = 803;

                num = 0;
                string[] commands1 = new string[] { "t", "[ZH]欢迎收听[ZH]", speak_id.ToString(), "D:\\c4\\Mahiro-VITS\\" + num + ".wav", "y" };// 指令参数列表

                foreach (string command in commands1)
                {
                    proc.StandardInput.WriteLine(command);
                }
                num++;
                using (StreamReader sr = File.OpenText(filePath)) // 打开指令文件进行读取
                {
                    string line;
                    while ((line = sr.ReadLine()) != null)
                    { // 循环读取每一行文本，并逐条发送到外部应用程序中执行命令
                        string[] commands2 = new string[] { "t", "[ZH]" + line + "[ZH]", speak_id.ToString(), "D:\\c4\\Mahiro-VITS\\" + num + ".wav", "y" }; // 指令参数列表
                        num++;
                        foreach (string command in commands2)
                        {
                            proc.StandardInput.WriteLine(command);
                        }
                    }
                }

                string[] commands3 = new string[] { "t", "[ZH]收听结束[ZH]", speak_id.ToString(), "D:\\c4\\Mahiro-VITS\\" + num + ".wav", "y" }; // 指令参数列表
                num++;
                foreach (string command in commands3)
                {
                    proc.StandardInput.WriteLine(command);
                }
                
                string output = proc.StandardOutput.ReadToEnd();

                Console.WriteLine("Output: " + output);

            }
        }
    }
    public class server_dll
    {

        [DllImport("Box_Server.dll", EntryPoint = "server_start")]
        public extern static void server_start();

        [DllImport("Box_Server.dll", EntryPoint = "sendFile")]
        public extern static void sendFile([MarshalAs(UnmanagedType.LPStr)] string str, Int64 id);
    }
}
