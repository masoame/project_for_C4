using C4_Web;
using Microsoft.AspNetCore.Hosting;
using C4_Web.Models;


//�������������еķ�����
Thread thread1 = new Thread(new ThreadStart(server_dll.server_start));
thread1.Start();

//��ʼ��TTS
TTS.init();

var builder = WebApplication.CreateBuilder(args);
//builder.WebHost.UseUrls("http://0.0.0.0:5400");

var startup = new Startup(builder.Configuration); 
startup.ConfigureServices(builder.Services);
var app = builder.Build(); 
startup.Configure(app, builder.Environment); 



//������ҳ
app.Run();
