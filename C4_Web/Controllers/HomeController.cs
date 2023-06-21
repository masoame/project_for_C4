using C4_Web.Models;
using Microsoft.AspNetCore.Hosting.Server;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Razor.Compilation;
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
                    return RedirectToAction("zhuanhuan");
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
            if(fileInput == null) return View();

            string webContentPath = _hostEnvironment.ContentRootPath;   //获取项目根目录

            string path = webContentPath + "\\Books\\" + fileInput.FileName;
            using (FileStream fileStream = new FileStream(path, FileMode.Create))
            {
                fileInput.CopyTo(fileStream);
            }

             TTS.start(path);

            ViewBag.AlertMsg = "生成成功";

            return View();
       
        }

        [HttpPost]
        public IActionResult langdu()
        {
            string str1 = "D:\\c4\\Mahiro-VITS\\";
            string str2 = ".wav";
            for (int i = 0; i != TTS.num+1; i++)
            {
                server_dll.sendFile(str1 + i + str2, 0);
                Thread.Sleep(1000);
            }

            return View("zhuanhuan");

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

            // 删除该文件
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

            return View();
        }


        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }
    }

}