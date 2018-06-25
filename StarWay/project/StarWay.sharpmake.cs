using Sharpmake;

namespace StarWay
{
    [Sharpmake.Generate]
    public class StarWayProject : Project
    {
        public StarWayProject()
        {
            Name = "StarWay";

            AddTargets(new Target(
                    Platform.win32 | Platform.win64,
                    DevEnv.vs2017,
                    Optimization.Debug | Optimization.Release,
					OutputType.Lib,
					Blob.NoBlob | Blob.Blob
            ));
            RootPath = @"[project.SharpmakeCsPath]\..";
            SourceRootPath = RootPath;
        }

        [Configure()]
        public void ConfigureAll(Configuration conf, Target target)
        {
            conf.ProjectFileName = "[project.Name]_[target.DevEnv]";
            conf.ProjectPath = @"[project.SharpmakeCsPath]";
			conf.IntermediatePath = @"[project.RootPath]\..\output\_temp\[target.DevEnv]\[target.Platform]\[project.Name]\[target.Optimization]";
			conf.TargetPath = @"[project.RootPath]\..\output\_temp\[target.DevEnv]\lib";
			conf.IncludePaths.Add(@"[project.RootPath]");
			
			conf.TargetFileFullName = @"[project.Name]_[target.Platform]_[target.Optimization]";
			conf.Output = Configuration.OutputType.Lib;
			
			conf.Options.Add(Options.Vc.General.TreatWarningsAsErrors.Enable);
			conf.Options.Add(Options.Vc.Compiler.Exceptions.Enable);
			conf.Options.Add(Options.Vc.Compiler.MinimalRebuild.Disable);
			conf.Options.Add(Options.Vc.Compiler.FloatingPointModel.Precise);	
				
			conf.AddPrivateDependency<Ash.AshProject>(target);
		    conf.AddPrivateDependency<Graphic.GraphicProject>(target);
			conf.AddPrivateDependency<Math.MathProject>(target);
			conf.AddPrivateDependency<TinyEngine.TinyEngineProject>(target);
			conf.Options.Add(Options.Vc.General.WarningLevel.Level3);
			conf.Options.Add(Options.Vc.Compiler.RTTI.Enable);
				
            conf.PrecompHeader = "precomp.h";
            conf.PrecompSource = "precomp.cpp";
        }
		
		[Configure()]
        public void ConfigureWindows(Configuration conf, Target target)
		{			
			if(target.Platform == Platform.win32)
			{
				conf.ExportDefines.Add("PLATFORM_WIN32");
				conf.Defines.Add("PLATFORM_WIN32");
			}
			else if(target.Platform == Platform.win64)
			{
				conf.ExportDefines.Add("PLATFORM_WIN64");
				conf.Defines.Add("PLATFORM_WIN64");
			}
		}
    }
}