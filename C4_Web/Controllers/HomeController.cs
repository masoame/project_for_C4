using C4_Web.Models;
using Microsoft.AspNetCore.Mvc;
using System.Diagnostics;
using System.IO.Compression;

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

            ZipFile.ExtractToDirectory($@"./Books/{fileNew}", $@"./Books");

            return View("Views\\Home\\chapter10.cshtml");
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }
    }
}