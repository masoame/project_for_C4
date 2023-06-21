using Microsoft.AspNetCore.Builder;


using Microsoft.AspNetCore.Builder;
using C4_Web.Models;
using Microsoft.EntityFrameworkCore;
using global::WebApplication2.Models;

namespace C4_Web
{
    public class Startup
    {
        public void Configure(IApplicationBuilder app)
        {
            // For more information on how to configure your application, visit https://go.microsoft.com/fwlink/?LinkID=398940
        }
        public Startup(IConfiguration configuration)
        {
            Configuration = configuration;
        }
        public IConfiguration Configuration { get; }
        public void ConfigureServices(IServiceCollection services)
        {

            services.AddRazorPages();
            services.AddControllersWithViews();
            //添加数据库上下文对象
            services.AddDbContext<mydbContext>(options =>
            {
                options.UseSqlServer(Configuration.GetConnectionString("mydb"));
            });
        }
        public void Configure(WebApplication app, IWebHostEnvironment env)
        {
            if (!app.Environment.IsDevelopment())
               app.UseExceptionHandler("/Error"); 
            app.UseHsts();
            app.UseHttpsRedirection();
            app.UseStaticFiles();
            app.UseRouting();
            app.UseAuthorization();
            app.MapControllerRoute(
             name: "default",
             pattern: "{controller=Home}/{action=index}/{id?}");

        }
    }
}
