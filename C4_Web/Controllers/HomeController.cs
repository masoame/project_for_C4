using C4_Web.Models;
using Microsoft.AspNetCore.Hosting.Server;
using Microsoft.AspNetCore.Mvc;
using System.Diagnostics;
using System.IO.Compression;
using System.Text;
namespace C4_Web.Controllers
{
    public class HomeController : Controller
    {
        private readonly IHostEnvironment _hostEnvironment;

        public HomeController(IHostEnvironment hostEnvironment)
        {
            _hostEnvironment = hostEnvironment;
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
}