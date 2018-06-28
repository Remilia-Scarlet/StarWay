using Sharpmake;
using Common;

[module: Sharpmake.Include("..\\..\\Sharpmake\\common.sharpmake.cs")]

namespace WinLancher
{
    [Sharpmake.Generate]
    public class WinLancherProject : CommonProject
    {
        public WinLancherProject()
        {
            Name = "WinLancher";
        }

        public override void ConfigureAll(Configuration conf, Target target)
        {
			base.ConfigureAll(conf,target);
			conf.TargetPath = @"[project.RootPath]\..\..\output\[target.Platform]";
			conf.Output = Configuration.OutputType.Exe;

			conf.Options.Add(Options.Vc.Linker.SubSystem.Application);	
	
			conf.EventPostBuild.Add("xcopy /E /H /R /Y \"$(SolutionDir)resources\\*.*\" \"$(TargetDir)\"");
						
			// Renderdoc
			conf.IncludePaths.Add(@"[project.RootPath]\..\..\Tools\renderdoc");
			if(target.Platform == Platform.win32)
			{
				conf.TargetCopyFiles.Add(@"[project.RootPath]\..\..\Tools\renderdoc\x86\renderdoc.pdb");
				conf.TargetCopyFiles.Add(@"[project.RootPath]\..\..\Tools\renderdoc\x86\renderdoc.dll");
				
			}
			else if(target.Platform == Platform.win64)
			{
				conf.TargetCopyFiles.Add(@"[project.RootPath]\..\..\Tools\renderdoc\renderdoc.dll");
				conf.TargetCopyFiles.Add(@"[project.RootPath]\..\..\Tools\renderdoc\renderdoc.pdb");
			}
				
			conf.AddPrivateDependency<StarWay.StarWayProject>(target);

        }
    }
}