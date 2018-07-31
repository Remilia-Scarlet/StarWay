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
			GameRoot = @"[project.SharpmakeCsPath]\..\..\..\";
			SourceRootPath = @"[project.SharpmakeCsPath]\..";

			SourceFilesExtensions.Add(".hlsl");
			SourceFilesCompileExtensions.Add(".hlsl");
			SourceFilesBuildExcludeRegex.Add(@"shader\\.*\.hlsl");
        }
		
        public override void ConfigureAll(Configuration conf, Target target)
        {		
			base.ConfigureAll(conf,target);
			
			conf.AddPublicDependency<Prebuild.PrebuildProject>(target);
			
			conf.LibraryFiles.Add("d3d11");
			//conf.LibraryFiles.Add("d3dcompiler.lib");
			conf.LibraryFiles.Add("dxguid.lib");

        }
    }
}