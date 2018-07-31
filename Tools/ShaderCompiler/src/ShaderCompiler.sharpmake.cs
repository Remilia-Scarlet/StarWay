using Sharpmake;
using Common;

[module: Sharpmake.Include("..\\..\\..\\Engine\\Ash\\project\\Ash.sharpmake.cs")]

namespace ShaderCompiler
{
	[Sharpmake.Generate]
	public class ShaderCompilerProject : Common.CommonProject
    {
        public ShaderCompilerProject()
        {
			AddTargets(TinyTarget.GetToolTarget());
            Name = "ShaderCompiler";
			GameRoot = @"[project.SharpmakeCsPath]\..\..\..\";
            SourceRootPath = @"[project.SharpmakeCsPath]";
        }

        [Configure()]
        public override void ConfigureAll(Configuration conf, Target target)
        {
			base.ConfigureAll(conf,target);
			conf.TargetPath = @"[project.RootPath]\..\build";
			
			conf.TargetFileFullName = @"[project.Name]";
			conf.Output = Configuration.OutputType.Exe;
			
			conf.Options.Add(Options.Vc.Linker.SubSystem.Console);	
			
			conf.Defines.Add("_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING");
			
			conf.LibraryFiles.Add("D3DCompiler.lib");
			conf.LibraryFiles.Add("dxguid.lib");
			
			conf.PrecompHeader = "precomp.h";
            conf.PrecompSource = "precomp.cpp";
			
			conf.AddPrivateDependency<Ash.AshProject>(target);
        }
	}

    [Sharpmake.Generate]
    public class ShaderCompilerSolution : Sharpmake.Solution
    {
        public ShaderCompilerSolution()
			:base(typeof(TinyTarget))
        {
            Name = "ShaderCompiler";

            AddTargets(new TinyTarget(
                    Platform.win64,
                    DevEnv.vs2017,
                    Optimization.Debug | Optimization.Release | Optimization.Retail,
					Mode.ToolMode
            ));
        }

        [Configure()]
        public void ConfigureAll(Configuration conf, Target target)
        {
		    conf.SolutionFileName = "[solution.Name]_[target.DevEnv]";
            conf.SolutionPath = @"[solution.SharpmakeCsPath]";

            conf.AddProject<Ash.AshProject>(target);
			conf.AddProject<ShaderCompilerProject>(target);
        }
		
        [Sharpmake.Main]
        public static void SharpmakeMain(Sharpmake.Arguments arguments)
        {
            arguments.Generate<ShaderCompilerSolution>();
        }
    }
}
