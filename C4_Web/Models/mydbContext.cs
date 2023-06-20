using Microsoft.EntityFrameworkCore;

namespace WebApplication2.Models;

public partial class mydbContext : DbContext
{
    public mydbContext()
    {
    }

    public mydbContext(DbContextOptions<mydbContext> options)
        : base(options)
    {
    }

    public virtual DbSet<ProductInfo> ProductInfos { get; set; }

    public virtual DbSet<UserInfo> UserInfos { get; set; }

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<ProductInfo>(entity =>
        {
            entity.HasKey(e => e.Id).HasName("PK__product___3214EC270AD2A005");

            entity.ToTable("product_info");

            entity.Property(e => e.Id).HasColumnName("ID");
            entity.Property(e => e.Description)
                .HasColumnType("text")
                .HasColumnName("description");
            entity.Property(e => e.Name)
                .HasMaxLength(50)
                .IsUnicode(false)
                .HasColumnName("name");
            entity.Property(e => e.Price)
                .HasColumnType("decimal(10, 2)")
                .HasColumnName("price");
        });

        modelBuilder.Entity<UserInfo>(entity =>
        {
            entity.HasKey(e => e.Id).HasName("PK__user_inf__3214EC277F60ED59");

            entity.ToTable("user_info");

            entity.Property(e => e.Id).HasColumnName("ID");
            entity.Property(e => e.Password)
                .HasMaxLength(50)
                .IsUnicode(false)
                .HasColumnName("password");
            entity.Property(e => e.Username)
                .HasMaxLength(50)
                .IsUnicode(false)
                .HasColumnName("username");
        });

        OnModelCreatingPartial(modelBuilder);
    }

    private partial void OnModelCreatingPartial(ModelBuilder modelBuilder);
}