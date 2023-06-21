// See https://aka.ms/new-console-template for more information


using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Threading;


Thread thread1 = new Thread(new ThreadStart(server_start));
thread1.Start();


Console.ReadLine();
string str = ".wav";

for(int i=0;i!=23 ;i++)
{
    sendFile(i + str, 0);
    Thread.Sleep(1000);
}


thread1.Join();

[DllImport("Box_Server.dll", EntryPoint= "server_start")]
extern static void  server_start();

[DllImport("Box_Server.dll", EntryPoint = "sendFile")]
extern static void sendFile([MarshalAs(UnmanagedType.LPStr)] string str, Int64 id);