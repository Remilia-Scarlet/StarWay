using Sharpmake;

[module: Sharpmake.Include("tinytarget.sharpmake.cs")]

namespace Common
{
    [Sharpmake.Generate]
    public class CommonProject : Project
    {
		public string GameRoot = "";
        public CommonProject()
			:base(typeof(TinyTarget))
        {
            //Name = "ProjectName";
			//SourceRootPath = RootPath;
			//GameRoot = @"[project.SharpmakeCsPath]\.."
			//AddTargets(TinyTarget.GetAllTarget());
			
            RootPath = @"[project.SharpmakeCsPath]";
			BlobPath = @"[project.GameRoot]\output\_temp\_blob\";
        }
		
        [Configure()]
        public virtual void ConfigureAll(Configuration conf, Target target)
        {			
			string platformStr = (target.Platform == Platform.win32 | target.Platform == Platform.win64 ? "win" : "[target.Platform]");
            conf.ProjectFileName = "[project.Name]_[target.DevEnv]_" + platformStr + "_[target.Blob]";
            conf.ProjectPath = @"[project.SharpmakeCsPath]";
			conf.IntermediatePath = @"[project.GameRoot]\output\_temp\[target.DevEnv]\[target.Platform]\[project.Name]_[target.Blob]\[target.Optimization]\[target.Mode]";
			conf.TargetPath = @"[project.GameRoot]\output\_temp\[target.DevEnv]\lib";
			conf.IncludePaths.Add(@"[project.GameRoot]\Engine");


            conf.ExportAdditionalLibrariesEvenForStaticLib = true;

            //boost
            conf.IncludePaths.Add(@"[project.GameRoot]\Engine\Extern\boost");
			conf.LibraryPaths.Add(@"[project.GameRoot]\Engine\Extern\boost\stage\lib");
			
			conf.Name = "[target.Mode] [target.Blob] [target.Optimization]";
			
			conf.TargetFileFullName = @"[project.Name]_[target.DevEnv]_[target.Platform]_[target.Optimization]_[target.Mode]_[target.Blob]";
			conf.Output = Configuration.OutputType.Lib;
			
			conf.Options.Add(Options.Vc.General.TreatWarningsAsErrors.Enable);
			conf.Options.Add(Options.Vc.Compiler.Exceptions.Enable);
			conf.Options.Add(Options.Vc.Compiler.MinimalRebuild.Disable);
			conf.Options.Add(Options.Vc.Compiler.FloatingPointModel.Precise);	
			conf.Options.Add(Options.Vc.General.WarningLevel.Level3);
			conf.Options.Add(Options.Vc.Compiler.RTTI.Enable);
			conf.Options.Add(Options.Vc.Compiler.CppLanguageStandard.CPP17);
			conf.Options.Add(Options.Vc.General.WindowsTargetPlatformVersion.v10_0_16299_0);

            conf.PrecompHeader = "[project.Name]\\precomp.h";
            conf.PrecompSource = "[project.Name]\\precomp.cpp";
        }
		
		[Configure(Optimization.Debug)]
		public virtual void OptimizationDebug(Configuration conf, Target target)
		{
			conf.Defines.Add("TINY_DEBUG");
		}
		
		[Configure(Optimization.Release)]
		public virtual void OptimizationRelease(Configuration conf, Target target)
		{
			conf.Defines.Add("TINY_RELEASE");
		}
		
		[Configure(Optimization.Debug)]
		public virtual void OptimizationRetail(Configuration conf, Target target)
		{
			conf.Defines.Add("TINY_RETAIL");
		}
		
		[Configure(Mode.ToolMode)]
		public virtual void ModeToolMode(Configuration conf, Target target)
		{
			conf.Defines.Add("TINY_TOOL_MODE");
		}
		
		[Configure(Mode.EngineMode)]
		public virtual void ModeEngineMode(Configuration conf, Target target)
		{
			conf.Defines.Add("TINY_ENGINE_MODE");
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