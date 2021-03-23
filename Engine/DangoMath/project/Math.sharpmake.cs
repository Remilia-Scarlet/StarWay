using Sharpmake;
using Common;

[module: Sharpmake.Include("..\\..\\Sharpmake\\common.sharpmake.cs")]

namespace Math
{
    [Sharpmake.Generate]
    public class MathProject : CommonProject
    {
        public MathProject()
        {
			AddTargets(TinyTarget.GetAllTarget());
            Name = "Math";
			GameRoot = @"[project.SharpmakeCsPath]\..\..\..\";
			SourceRootPath = @"[project.SharpmakeCsPath]\..";
        }
	}
}