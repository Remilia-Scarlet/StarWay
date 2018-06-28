using Sharpmake;
using Common;

[module: Sharpmake.Include("..\\..\\Engine\\Sharpmake\\common.sharpmake.cs")]

namespace StarWay
{
    [Sharpmake.Generate]
    public class StarWayProject : CommonProject
    {
        public StarWayProject()
        {
            Name = "StarWay";
        }

        public override void ConfigureAll(Configuration conf, Target target)
        {
			base.ConfigureAll(conf,target);
			conf.IntermediatePath = @"[project.RootPath]\..\output\_temp\[target.DevEnv]\[target.Platform]\[project.Name]\[target.Optimization]";
			conf.TargetPath = @"[project.RootPath]\..\output\_temp\[target.DevEnv]\lib";			

			conf.AddPrivateDependency<Ash.AshProject>(target);
		    conf.AddPrivateDependency<Graphic.GraphicProject>(target);
			conf.AddPrivateDependency<Math.MathProject>(target);
			conf.AddPrivateDependency<TinyEngine.TinyEngineProject>(target);				
        }
    }
}