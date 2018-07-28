using Sharpmake;
using Common;

[module: Sharpmake.Include("..\\..\\Sharpmake\\common.sharpmake.cs")]

namespace Prebuild
{
    [Sharpmake.Generate]
    public class PrebuildProject : CommonProject
    {
        public PrebuildProject()
        {
            Name = "Prebuild";
        }
		
        public override void ConfigureAll(Configuration conf, Target target)
        {		
			base.ConfigureAll(conf,target);

			conf.SolutionFolder = "Tools";
			conf.EventCustomBuildDescription = "Compiling shader";
			conf.EventCustomBuild.Add(@"..\..\..\Tools\ShaderCompiler\build\shadercompiler.exe"+
				@" /Source:..\..\Graphic\EmbeddedShader\" +
				@" /Source:..\..\..\resources\shaders\" +
				@" /Output:..\..\..\output\[target.Platform]\shaders" +
				@" /IntDir:$(IntDir)" +
				@" /Filter:*.hlsl" + 
				@" /Include:..\..\Graphic\ShaderInclude\" +
				@" /MetaOut:..\..\Graphic\ShaderInclude\ShaderInfo.h"
			);
			conf.EventCustomBuildOutputs = "ThereIsNoOutputSoThatExecuteEveryBuild.txt";
			conf.CustomBuildStepBeforeTargets = "ClCompile";
        }
    }
}