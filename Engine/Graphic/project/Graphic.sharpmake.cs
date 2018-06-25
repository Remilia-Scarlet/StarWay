using Sharpmake;

namespace Graphic
{
    [Sharpmake.Generate]
    public class GraphicProject : Project
    {
        public GraphicProject()
        {
            Name = "Graphic";

            AddTargets(new Target(
                    Platform.win32 | Platform.win64,
                    DevEnv.vs2017,
                    Optimization.Debug | Optimization.Release,
					OutputType.Lib,
					Blob.NoBlob | Blob.Blob
            ));
            RootPath = @"[project.SharpmakeCsPath]\..";
            SourceRootPath = RootPath;
			
			SourceFilesExtensions.Add(".hlsl");
			SourceFilesCompileExtensions.Add(".hlsl");
			SourceFilesBuildExcludeRegex.Add(@"shader\\.*\.hlsl");
        }
		
        [Configure()]
        public void ConfigureAll(Configuration conf, Target target)
        {			
			//conf.EventPreBuild.Add(@"..\..\..\Tools\ShaderCompiler\build\shadercompiler.exe"+
			//	@" /Source:..\shader\" +
			//	@" /Output:..\..\..\output\[target.Platform]\" +
			//	@" /IntDir:..\..\..\output\_temp\ShaderCompilerIntDir\" +
			//	@" /Filter:*.hlsl"
			//);
			//conf.EventPreBuildDescription = "Compiling shader";
			//conf.EventPreBuildExcludedFromBuild = false;
			
			conf.EventCustomBuildDescription = "Compiling shader";
			conf.EventCustomBuild.Add(@"..\..\..\Tools\ShaderCompiler\build\shadercompiler.exe"+
				@" /Source:..\shader\" +
				@" /Output:..\..\..\output\[target.Platform]\" +
				@" /IntDir:..\..\..\output\_temp\ShaderCompilerIntDir\" +
				@" /Filter:*.hlsl"
			);
			conf.EventCustomBuildOutputs = "ThereIsNoOutputSoThatExecuteEveryBuild.txt";
			conf.CustomBuildStepBeforeTargets = "ClCompile";
			
            conf.ProjectFileName = "[project.Name]_[target.DevEnv]_[target.Platform]";
            conf.ProjectPath = @"[project.SharpmakeCsPath]";
			conf.IntermediatePath = @"[project.RootPath]\..\..\output\_temp\[target.DevEnv]\[target.Platform]\[project.Name]\[target.Optimization]";
			conf.TargetPath = @"[project.RootPath]\..\..\output\_temp\[target.DevEnv]\lib";
			conf.IncludePaths.Add(@"[project.RootPath]");
			conf.IncludePaths.Add(@"[project.RootPath]\..");
			
			conf.TargetFileFullName = @"[project.Name]_[target.Platform]_[target.Optimization]";
			conf.Output = Configuration.OutputType.Lib;
			
			conf.Options.Add(Options.Vc.General.TreatWarningsAsErrors.Enable);
			conf.Options.Add(Options.Vc.Compiler.Exceptions.Enable);
			conf.Options.Add(Options.Vc.Compiler.MinimalRebuild.Disable);
			conf.Options.Add(Options.Vc.Compiler.FloatingPointModel.Precise);	
			conf.Options.Add(Options.Vc.General.WarningLevel.Level3);
			conf.Options.Add(Options.Vc.Compiler.RTTI.Enable);
			
			conf.LibraryFiles.Add("d3d11");
			conf.LibraryFiles.Add("d3dcompiler.lib");
			conf.LibraryFiles.Add("dxguid.lib");
				
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
			conf.ProjectFileName = "[project.Name]_[target.DevEnv]_win";
		}
    }
}