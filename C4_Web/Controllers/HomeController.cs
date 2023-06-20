using C4_Web.Models;
using Microsoft.AspNetCore.Hosting.Server;
using Microsoft.AspNetCore.Mvc;
using System.Diagnostics;
using System.IO.Compression;
using System.Text;
using System.Text.RegularExpressions;
using WebApplication2.Models;

namespace C4_Web.Controllers
{
    public class HomeController : Controller
    {
        private readonly IHostEnvironment _hostEnvironment;
        private readonly ILogger<HomeController> _logger;
        private mydbContext _context;
        public HomeController(IHostEnvironment hostEnvironment,ILogger<HomeController> logger, mydbContext context)
        {
            _hostEnvironment = hostEnvironment;
            _logger = logger;
            //添加服务
            _context = context;
        }
        [HttpPost]
        public IActionResult Login(UserInfo user)
        {
            if (user.Username==null)
            {
                ViewBag.AlertMsg="账号不能为空";
            }
            else if (user.Password==null)
            {
                ViewBag.AlertMsg="密码不能为空";
            }
            else
            {
                var u = _context.UserInfos.FirstOrDefault(p => p.Username==user.Username&&p.Password==user.Password);
                if (u!=null)
                {
                    return RedirectToAction("index1");
                }
            }

            return View();
        }
        public IActionResult index1()
        {
         
            return View();
        }
        //调用exe按钮
        [HttpPost]
        public IActionResult zhuanhuan(IFormFile fileInput)
        {
            var filePath = Path.GetFullPath("Books\\"+fileInput.FileName);
            startexe model = new startexe();
            if (filePath!=null)
            {
                model.start(filePath);
                ViewBag.AlertMsg="生成成功";
            }
           
            return View();
       
        }
        //转化
        public IActionResult zhuanhuan()
        {
          
            return View();

        }
        public IActionResult buttun()
        {
          
            return View();

        }
        public IActionResult Index()
        {
            ViewData["test"] = "deafult";
            return View();
        }

        [HttpPost]
        public IActionResult Index([FromForm] IFormCollection fromData)
        {
            string fileNew = "";
            IFormFileCollection files = fromData.Files;
            foreach (var formFile in files)
            {
                //通过_hostEnvironment.ContentRootPath来获取根目录
                string webContentPath = _hostEnvironment.ContentRootPath;//获取项目根目录
                var fileExt = formFile.FileName.Substring(formFile.FileName.LastIndexOf('.'));//获取文件扩展名
                fileNew = DateTime.Now.ToString("yyyyMMddHHmmss") + fileExt; //给文件重新命名

                string upLoadPath = webContentPath + $@"\Books";

                var fileUrl = upLoadPath + $@"\{fileNew}"; //文件重命名防止重复

                if (formFile.Length > 0)
                {
                    using (var stream = new FileStream(fileUrl, FileMode.Create))
                    {
                        formFile.CopyTo(stream);
                    }
                }
            }
            System.Text.Encoding.RegisterProvider(System.Text.CodePagesEncodingProvider.Instance);
            ZipFile.ExtractToDirectory($@"./Books/{fileNew}", $@"./Books", Encoding.GetEncoding("gb2312"));
            // 返回与指定虚拟路径相对应的物理路径即绝对路径

            // 删除该文件\
            System.IO.File.Delete(Path.Combine($@"./Books/{fileNew}"));
            string rootPath = Path.Combine($@"./Books/OEBPS/Text");
            string[] files1 = Directory.GetFiles(rootPath, "*chapter*", SearchOption.AllDirectories);

            // 创建test文件夹
            string testPath = Path.Combine( $@"./Books/OEBPS/test");
            if (!Directory.Exists(testPath))
            {
                Directory.CreateDirectory(testPath);
            }

            // 将符合条件的文件移动到test文件夹中
            foreach (string file in files1)
            {
                string fileName = Path.GetFileName(file);
                if (fileName.EndsWith(".cshtml"))
                {
                    System.IO.File.Move(file, Path.Combine(testPath, fileName));
                }
                else
                {
                    System.IO.File.Move(file, Path.Combine(testPath, Path.ChangeExtension(fileName, ".cshtml")));
                }
            }




            return View("Views\\Home\\chapter10.cshtml");
        }


        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }
    }
    class vits
    {

        private decimal LENGTHSCALE;
        private decimal NOISESCALE;
        private decimal NOISESCALEW;

        CommandLine cmd;

        public class CommandLine
        {
            private readonly Process process;

            public delegate void onOutputHandler(CommandLine sender, string e);
            public event onOutputHandler OutputHandler;

            public CommandLine()
            {
                try
                {
                    process = new Process()
                    {
                        StartInfo = new ProcessStartInfo("cmd.exe")
                        {
                            Arguments = "/k",
                            RedirectStandardInput = true,
                            RedirectStandardOutput = true,
                            RedirectStandardError = true,
                            UseShellExecute = false,
                            CreateNoWindow = true
                        }
                    };
                    process.OutputDataReceived += Command_OutputDataReceived;
                    process.ErrorDataReceived += Command_ErrorDataReceived;
                    process.Start();
                    process.BeginOutputReadLine();
                    process.BeginErrorReadLine();
                }
                catch (Exception e)
                {
                    Trace.WriteLine(e.Message);
                }
            }

            void Command_ErrorDataReceived(object sender, DataReceivedEventArgs e)
            {
                OnOutput(e.Data);
            }

            void Command_OutputDataReceived(object sender, DataReceivedEventArgs e)
            {
                OnOutput(e.Data);
            }

            private void OnOutput(string data)
            {
                OutputHandler?.Invoke(this, data);
            }

            public void Write(string data)
            {
                try
                {
                    process.StandardInput.WriteLine(data);
                }
                catch (Exception e)
                {
                    Trace.WriteLine(e.Message);
                }
            }
        }





        private void TTS(string text, int speaker)
        {
            cmd.Write("t");
            cmd.Write($"[LENGTH={LENGTHSCALE}][NOISE={NOISESCALE}][NOISEW={NOISESCALEW}]{Regex.Replace(text, @"\r?\n", " ")}");
            cmd.Write(speaker.ToString());
        }
    }

}