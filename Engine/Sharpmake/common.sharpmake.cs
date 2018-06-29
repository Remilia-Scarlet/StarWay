using Sharpmake;

namespace Common
{
    [Sharpmake.Generate]
    public class CommonProject : Project
    {
        public CommonProject()
        {
            //Name = "ProjectName";

            AddTargets(new Target(
                    Platform.win32 | Platform.win64,
                    DevEnv.vs2017,
                    Optimization.Debug | Optimization.Release,
					OutputType.Lib,
					Blob.NoBlob | Blob.Blob
            ));
            RootPath = @"[project.SharpmakeCsPath]\..";
            SourceRootPath = RootPath;
			
			BlobPath = @"[project.RootPath]\..\..\output\_temp\_blob\";
        }
		
        [Configure()]
        public virtual void ConfigureAll(Configuration conf, Target target)
        {			
			string platformStr = (target.Platform == Platform.win32 | target.Platform == Platform.win64 ? "win" : "[target.Platform]");
            conf.ProjectFileName = "[project.Name]_[target.DevEnv]_" + platformStr + "_[target.Blob]";
            conf.ProjectPath = @"[project.SharpmakeCsPath]";
			conf.IntermediatePath = @"[project.RootPath]\..\..\output\_temp\[target.DevEnv]\[target.Platform]\[project.Name]_[target.Blob]\[target.Optimization]";
			conf.TargetPath = @"[project.RootPath]\..\..\output\_temp\[target.DevEnv]\lib";
			conf.IncludePaths.Add(@"[project.RootPath]");
			conf.IncludePaths.Add(@"[project.RootPath]\..");
			
			conf.Name = "[target.Blob] [target.Optimization]";
			
			conf.TargetFileFullName = @"[project.Name]_[target.Platform]_[target.Optimization]_[target.Blob]";
			conf.Output = Configuration.OutputType.Lib;
			
			conf.Options.Add(Options.Vc.General.TreatWarningsAsErrors.Enable);
			conf.Options.Add(Options.Vc.Compiler.Exceptions.Enable);
			conf.Options.Add(Options.Vc.Compiler.MinimalRebuild.Disable);
			conf.Options.Add(Options.Vc.Compiler.FloatingPointModel.Precise);	
			conf.Options.Add(Options.Vc.General.WarningLevel.Level3);
			conf.Options.Add(Options.Vc.Compiler.RTTI.Enable);
			conf.Options.Add(Options.Vc.Compiler.CppLanguageStandard.CPP17);
			conf.Options.Add(Options.Vc.General.WindowsTargetPlatformVersion.v10_0_16299_0);

            conf.PrecompHeader = "precomp.h";
            conf.PrecompSource = "precomp.cpp";
        }
		
		[Configure(Platform.win32 | Platform.win64)]
        public virtual void ConfigureWindows(Configuration conf, Target target)
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
		
		[Configure(Blob.Blob)]
		public virtual void BlobBlob(Configuration conf, Target target)
		{
			conf.IsBlobbed = true;
			conf.ProjectName += ".Blob";
			conf.IncludeBlobbedSourceFiles = false;
			conf.SolutionFolder = "Blob";
		}
		
		[Configure(Blob.NoBlob)]
		public virtual void BlobNoBlob(Configuration conf, Target target)
		{
			conf.IsBlobbed = false;
		}
    }
}