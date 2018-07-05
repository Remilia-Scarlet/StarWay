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
            Name = "UnitTest";
        }

        public override void ConfigureAll(Configuration conf, Target target)
        {
			base.ConfigureAll(conf,target);
			conf.SolutionFolder = "UnitTest";
			conf.TargetPath = @"[project.RootPath]\..\..\output\_temp\UnitTest\";
			conf.Output = Configuration.OutputType.Exe;
			conf.Options.Add(Options.Vc.Linker.SubSystem.Console);	
			
			conf.PrecompHeader = "";
            conf.PrecompSource = "";
			
			string debugSuffix = (target.Optimization == Optimization.Debug ? "d" : "");
			conf.LibraryFiles.Add("gtest" + debugSuffix + ".lib");
			conf.LibraryFiles.Add("gtest_main" + debugSuffix + ".lib");
			conf.LibraryPaths.Add(@"[project.RootPath]\..\..\Tools\GoogleTest\build\[target.Platform]");
			conf.IncludePaths.Add(@"[project.RootPath]\..\..\Tools\GoogleTest\src\googletest\include");

			
			conf.AddPrivateDependency<StarWay.StarWayProject>(target);

        }
    }
}