using Sharpmake;
using Common;

[module: Sharpmake.Include("..\\..\\Sharpmake\\common.sharpmake.cs")]

namespace Ash
{
    [Sharpmake.Generate]
    public class AshProject : CommonProject
    {
        public AshProject()
        {
            Name = "Ash";
			GameRoot = @"[project.SharpmakeCsPath]\..\..\..\";
			SourceRootPath = @"[project.SharpmakeCsPath]\..";
        }
    }
}