using Sharpmake;
using Common;

[module: Sharpmake.Include("..\\..\\Sharpmake\\common.sharpmake.cs")]

namespace Graphic
{
    [Sharpmake.Generate]
    public class GraphicProject : CommonProject
    {
        public GraphicProject()
        {
            Name = "Graphic";

			SourceFilesExtensions.Add(".hlsl");
			SourceFilesCompileExtensions.Add(".hlsl");
			SourceFilesBuildExcludeRegex.Add(@"shader\\.*\.hlsl");
        }
		
        public override void ConfigureAll(Configuration conf, Target target)
        {		
			base.ConfigureAll(conf,target);
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
				@" /Output:..\..\..\output\[target.Platform]\shaders" +
				@" /IntDir:$(IntDir)" +
				@" /Filter:*.hlsl"
			);
			conf.EventCustomBuildOutputs = "ThereIsNoOutputSoThatExecuteEveryBuild.txt";
			conf.CustomBuildStepBeforeTargets = "ClCompile";
				
			conf.LibraryFiles.Add("d3d11");
			conf.LibraryFiles.Add("d3dcompiler.lib");
			conf.LibraryFiles.Add("dxguid.lib");

        }
    }
}