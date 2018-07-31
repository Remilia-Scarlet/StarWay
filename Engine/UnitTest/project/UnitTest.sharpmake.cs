using Sharpmake;
using Common;

[module: Sharpmake.Include("..\\..\\Sharpmake\\common.sharpmake.cs")]

namespace UnitTest
{
    [Sharpmake.Generate]
    public class UnitTestProject : CommonProject
    {
        public UnitTestProject()
        {
			AddTargets(TinyTarget.GetAllTarget());
            Name = "UnitTest";
			GameRoot = @"[project.SharpmakeCsPath]\..\..\..\";
			SourceRootPath = @"[project.SharpmakeCsPath]\..";
        }

        public override void ConfigureAll(Configuration conf, Target target)
        {
			base.ConfigureAll(conf,target);
			conf.SolutionFolder = "Tools";
			conf.TargetPath = @"[project.GameRoot]\output\_temp\UnitTest\";
			conf.Output = Configuration.OutputType.Exe;
			conf.Options.Add(Options.Vc.Linker.SubSystem.Console);	
			
			string debugSuffix = (target.Optimization == Optimization.Debug ? "d" : "");
			conf.LibraryFiles.Add("gtest" + debugSuffix + ".lib");
			//conf.LibraryFiles.Add("gtest_main" + debugSuffix + ".lib");
			conf.LibraryPaths.Add(@"[project.GameRoot]\Tools\GoogleTest\build\[target.Platform]");
			conf.IncludePaths.Add(@"[project.GameRoot]\Tools\GoogleTest\src\googletest\include");

			
			conf.AddPrivateDependency<StarWay.StarWayProject>(target);

        }
    }
}