using Sharpmake;
using Common;

[module: Sharpmake.Include("..\\..\\Sharpmake\\common.sharpmake.cs")]

namespace MaterialCompiler
{
    [Sharpmake.Generate]
    public class MaterialCompilerProject : CommonProject
    {
        public MaterialCompilerProject()
        {
            Name = "MaterialCompiler";
			GameRoot = @"[project.SharpmakeCsPath]\..\..\..\";
			SourceRootPath = @"[project.SharpmakeCsPath]\..";
        }

        public override void ConfigureAll(Configuration conf, Target target)
        {
			base.ConfigureAll(conf,target);
			conf.SolutionFolder = "Tools";
			conf.TargetPath = @"[project.GameRoot]\output\[target.Platform]";
			conf.Output = Configuration.OutputType.Exe;

			conf.Options.Add(Options.Vc.Linker.SubSystem.Application);	

			conf.AddPrivateDependency<StarWay.StarWayProject>(target);

        }
    }
}