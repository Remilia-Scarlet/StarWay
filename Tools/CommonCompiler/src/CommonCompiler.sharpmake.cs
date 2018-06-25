using Sharpmake;

[module: Sharpmake.Include("..\\..\\..\\Engine\\Ash\\project\\Ash.sharpmake.cs")]

namespace CommonCompiler
{
	[Sharpmake.Generate]
	public class CommonCompilerProject : Project
    {
        public CommonCompilerProject()
        {
            Name = "CommonCompiler";

            AddTargets(new Target(
                    Platform.win64,
                    DevEnv.vs2017,
                    Optimization.Debug | Optimization.Release
            ));
            RootPath = @"[project.SharpmakeCsPath]";
            SourceRootPath = RootPath;
        }

        [Configure()]
        public void ConfigureAll(Configuration conf, Target target)
        {
            conf.ProjectFileName = "[project.Name]_[target.DevEnv]";
            conf.ProjectPath = @"[project.SharpmakeCsPath]";
			conf.IntermediatePath = @"[project.RootPath]\..\..\..\output\_temp\[target.DevEnv]\[target.Platform]\[project.Name]\[target.Optimization]";
			conf.TargetPath = @"[project.RootPath]\..\build";
			conf.IncludePaths.Add(@"[project.RootPath]");
			conf.IncludePaths.Add(@"[project.RootPath]\..\..\..\");
			
			conf.TargetFileFullName = @"[project.Name]";
			conf.Output = Configuration.OutputType.Exe;
			
			conf.Options.Add(Options.Vc.General.TreatWarningsAsErrors.Enable);
			conf.Options.Add(Options.Vc.Compiler.Exceptions.Enable);
			conf.Options.Add(Options.Vc.Compiler.MinimalRebuild.Disable);
			conf.Options.Add(Options.Vc.Compiler.FloatingPointModel.Precise);
			conf.Options.Add(Options.Vc.General.WarningLevel.Level3);		
			conf.Options.Add(Options.Vc.Compiler.RTTI.Enable);		
			conf.Options.Add(Options.Vc.Linker.SubSystem.Console);	

			conf.AddPrivateDependency<Ash.AshProject>(target);
        }
	}

    [Sharpmake.Generate]
    public class CommonCompilerSolution : Sharpmake.Solution
    {
        public CommonCompilerSolution()
        {
            Name = "CommonCompiler";

            AddTargets(new Target(
                    Platform.win64,
                    DevEnv.vs2017,
                    Optimization.Debug | Optimization.Release
            ));
        }

        [Configure()]
        public void ConfigureAll(Configuration conf, Target target)
        {
		    conf.SolutionFileName = "[solution.Name]_[target.DevEnv]";
            conf.SolutionPath = @"[solution.SharpmakeCsPath]";

            conf.AddProject<Ash.AshProject>(target);
			conf.AddProject<CommonCompilerProject>(target);
        }
		
        [Sharpmake.Main]
        public static void SharpmakeMain(Sharpmake.Arguments arguments)
        {
            arguments.Generate<CommonCompilerSolution>();
        }
    }
}
