using Sharpmake;
using System;

using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Text;

namespace CompileShader
{
    [Sharpmake.Generate]
    public class CompileShaderProject : Project
    {
        public CompileShaderProject()
        {
            Name = "CompileShader";

            AddTargets(new Target(
                    Platform.win32 | Platform.win64,
                    DevEnv.vs2015 | DevEnv.vs2017,
                    Optimization.Debug | Optimization.Release,
					OutputType.Lib,
					Blob.NoBlob | Blob.Blob
            ));
            RootPath = @"[project.SharpmakeCsPath]\..";
            SourceRootPath = RootPath;
			
			SourceFilesExtensions.Add(".hlsl");
			SourceFilesCompileExtensions.Add(".hlsl");
        }
		
		protected override void ExcludeOutputFiles()
        {
            base.ExcludeOutputFiles();
			
			
			 // Filter all the files by the filters we've already specified, so we don't moc a file that's excluded from the solution.
            List<Regex> filters = this.SourceFilesExcludeRegex.Select(filter => new Regex(filter, RegexOptions.Compiled | RegexOptions.Singleline | RegexOptions.CultureInvariant | RegexOptions.IgnoreCase)).ToList();
            //filters.AddRange(ExcludeMocRegex.Select(filter => new Regex(filter, RegexOptions.Compiled | RegexOptions.Singleline | RegexOptions.CultureInvariant | RegexOptions.IgnoreCase)));

            var preFilteredFiles = this.ResolvedSourceFiles.Where(file => !filters.Any(filter => filter.IsMatch(file)) && !this.Configurations.Any(conf => FileIsPrecompiledHeader(file, conf))).ToList();

            // Async load all the source files and look for Q_OBJECT that we want to keep.
            //var answerSetTask = Task.WhenAll(preFilteredFiles.Select(async file => new { file = file, runMoc = await FileContainsQObject(file) }));
            // Compile a list of qrc and ui files.
            Strings hlslFiles = new Strings(preFilteredFiles.Where(file => file.EndsWith(".hlsl", StringComparison.InvariantCultureIgnoreCase)));

            // Wait for the moc files.
            //answerSetTask.Wait();
            //var filterPass = answerSetTask.Result;
            // These are the files we want to moc.
            //Strings FilteredResolvedSourceFiles = new Strings(filterPass.Where(result => result.runMoc).Select(result => result.file));
			foreach (String name in hlslFiles)
			{
                Project.Configuration.CustomFileBuildStep cus = new Project.Configuration.CustomFileBuildStep();
                cus.Description = "Compilling shader";
                cus.Output = @"[project.SharpmakeCsPath]\..\..\..\output\$(Platform)\%(Filename).cso";
                cus.AdditionalInputs.Add(@"[project.SharpmakeCsPath]\..\shader\common.h");
                cus.KeyInput = name;
				cus.Executable = @"[project.SharpmakeCsPath]\..\..\..\Tools\ShaderCompiler\build\shadercompiler.exe";
				cus.ExecutableArguments = "/Source:\"%(FullPath)\" /Output:\"[project.SharpmakeCsPath]\\..\\..\\..\\output\\[target.Platform]\\%(Filename).cso\"";
				foreach (Sharpmake.Project.Configuration conf in this.Configurations)
				{
					conf.CustomFileBuildSteps.Add(cus);
				}
			}
        }
		
        [Configure()]
        public void ConfigureAll(Configuration conf, Target target)
        {
			
			// conf.CustomBuildForAllSources = new Configuration.FileCustomBuild();
			// conf.CustomBuildForAllSources.CommandLines.Add("..\\..\\..\\Tools\\ShaderCompiler\\build\\shadercompiler.exe /Source:\"%(FullPath)\" /Output:\"\\..\\..\\output\\$(Platform)\\%(Filename).cso\"");
			// conf.CustomBuildForAllSources.Description = "Compiling shader";
			// conf.CustomBuildForAllSources.Inputs.Add("%(FullPath)");
			// conf.CustomBuildForAllSources.Outputs.Add(@"\..\..\output\$(Platform)\%(Filename).cso");
			
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

		bool FileIsPrecompiledHeader(string file, Project.Configuration conf)
        {
            return (conf.PrecompHeader != null && file.EndsWith(conf.PrecompHeader, StringComparison.InvariantCultureIgnoreCase))
                     || (conf.PrecompSource != null && file.EndsWith(conf.PrecompSource, StringComparison.InvariantCultureIgnoreCase));
        }
    }
}