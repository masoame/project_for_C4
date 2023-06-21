using Microsoft.AspNetCore.Mvc;
using System.Diagnostics;

namespace C4_Web.Models
{

    public class startexe
    {
        public string? filePath { get; set; }
        public void start(string filePath1)
        {
            if (filePath1 != null)
            {
                string filePath = filePath1;
                ProcessStartInfo startInfo = new ProcessStartInfo();
                startInfo.FileName = "D:\\c4\\MoeGoe\\MoeGoe\\MoeGoe.exe";
                startInfo.RedirectStandardInput = true;
                startInfo.RedirectStandardOutput = true;
                startInfo.CreateNoWindow = true;
                startInfo.UseShellExecute = false;

                Process proc = new Process();
                proc.StartInfo = startInfo;

                proc.Start();

                const int speak_id = 141;
                int i = 0;
                string[] commands1 = new string[] { "D:\\c4\\Mahiro-VITS\\G_latest.pth", "D:\\c4\\Mahiro-VITS\\finetune_speaker.json", "t", "[ZH]欢迎收听[ZH]", speak_id.ToString(), "D:\\c4\\Mahiro-VITS\\" + i + ".wav", "y" }; // 指令参数列表
                i++;
                foreach (string command in commands1)
                {
                    proc.StandardInput.WriteLine(command);
                }
                using (StreamReader sr = File.OpenText(filePath)) // 打开指令文件进行读取
                {
                    string line;
                    while ((line = sr.ReadLine()) != null)
                    { // 循环读取每一行文本，并逐条发送到外部应用程序中执行命令
                        string[] commands2 = new string[] { "t", "[ZH]" + line + "[ZH]", speak_id.ToString(), "D:\\c4\\Mahiro-VITS\\" + i + ".wav", "y" }; // 指令参数列表
                        i++;
                        foreach (string command in commands2)
                        {
                            proc.StandardInput.WriteLine(command);
                        }
                    }
                }
                string[] commands3 = new string[] { "t", "[ZH]收听结束[ZH]", speak_id.ToString(), "D:\\c4\\Mahiro-VITS\\" + i + ".wav", "n" }; // 指令参数列表
                i++;
                foreach (string command in commands3)
                {
                    proc.StandardInput.WriteLine(command);
                }
                
                string output = proc.StandardOutput.ReadToEnd();

                proc.WaitForExit();

                int exitCode = proc.ExitCode;
                Console.WriteLine("External process exited with code: " + exitCode);
                Console.WriteLine("Output: " + output);

            }
        }
    }
}
