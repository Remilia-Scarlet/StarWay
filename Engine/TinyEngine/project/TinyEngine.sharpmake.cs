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
            Name = "TinyEngine";
        }
		
		public override void ConfigureAll(Configuration conf, Target target)
        {
			base.ConfigureAll(conf,target);
			conf.AddPrivateDependency<Prebuild.PrebuildProject>(target);
		}
    }
}