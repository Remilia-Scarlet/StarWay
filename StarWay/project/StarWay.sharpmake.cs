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
			AddTargets(TinyTarget.GetAllTarget());
            Name = "StarWay";
			GameRoot = @"[project.SharpmakeCsPath]\..\..\..\";
			SourceRootPath = @"[project.SharpmakeCsPath]\..";
        }

        public override void ConfigureAll(Configuration conf, Target target)
        {
			base.ConfigureAll(conf,target);
			
			conf.IncludePaths.Add(@"[project.SourceRootPath]\");

			conf.AddPrivateDependency<Ash.AshProject>(target);
			conf.AddPrivateDependency<Prebuild.PrebuildProject>(target);
		    conf.AddPrivateDependency<Graphic.GraphicProject>(target);
			conf.AddPrivateDependency<Math.MathProject>(target);
			conf.AddPrivateDependency<DangoEngine.TinyEngineProject>(target);	
			
            conf.PrecompHeader = "precomp.h";
            conf.PrecompSource = "precomp.cpp";			
        }
    }
}