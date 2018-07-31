using Sharpmake;
using Common;

[module: Sharpmake.Include("..\\..\\Sharpmake\\common.sharpmake.cs")]

namespace TinyEngine
{
    [Sharpmake.Generate]
    public class TinyEngineProject : CommonProject
    {
        public TinyEngineProject()
        {
			AddTargets(TinyTarget.GetAllTarget());
            Name = "TinyEngine";
			GameRoot = @"[project.SharpmakeCsPath]\..\..\..\";
			SourceRootPath = @"[project.SharpmakeCsPath]\..";
        }
		
		public override void ConfigureAll(Configuration conf, Target target)
        {
			base.ConfigureAll(conf,target);
			conf.AddPrivateDependency<Prebuild.PrebuildProject>(target);
		}
    }
}