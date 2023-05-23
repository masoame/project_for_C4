using System.Text;

foreach (string str in System.IO.File.ReadAllLines(@"D:\小说\无职转生\[理不尽な孫の手] 无职转生~到了異世界就拿出真本事~ [25][青年期 决战篇 下]\chapter3.html.txt", Encoding.Default))
{
    Console.WriteLine(str);                         // str就是每一行数据
}