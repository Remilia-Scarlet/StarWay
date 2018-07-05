using Sharpmake;

[module: Sharpmake.Include("Engine\\Ash\\project\\Ash.sharpmake.cs")]
[module: Sharpmake.Include("Engine\\Graphic\\project\\Graphic.sharpmake.cs")]
[module: Sharpmake.Include("Engine\\Math\\project\\Math.sharpmake.cs")]
[module: Sharpmake.Include("Engine\\TinyEngine\\project\\TinyEngine.sharpmake.cs")]
[module: Sharpmake.Include("StarWay\\project\\StarWay.sharpmake.cs")]
[module: Sharpmake.Include("Engine\\WinLancher\\project\\WinLancher.sharpmake.cs")]
[module: Sharpmake.Include("Engine\\UnitTest\\project\\UnitTest.sharpmake.cs")]

namespace StarWay
{
    [Sharpmake.Generate]
    public class StarWaySolution : Sharpmake.Solution
    {
        public StarWaySolution()
        {
            Name = "StarWay";

            AddTargets(new Target(
                    Platform.win32 | Platform.win64,
                    DevEnv.vs2017,
                    Optimization.Debug | Optimization.Release,
					OutputType.Lib,
					Blob.NoBlob/* | Blob.Blob */
            ));
        }

        [Configure()]
        public void ConfigureAll(Configuration conf, Target target)
        {
			conf.Name = "[target.Blob] [target.Optimization]";
		    conf.SolutionFileName = "[solution.Name]_[target.DevEnv]_[target.Platform]";
            conf.SolutionPath = @"[solution.SharpmakeCsPath]";

            conf.AddProject<Ash.AshProject>(target);
		    conf.AddProject<Graphic.GraphicProject>(target);
			conf.AddProject<Math.MathProject>(target);
			conf.AddProject<TinyEngine.TinyEngineProject>(target);
			conf.AddProject<StarWay.StarWayProject>(target);
			conf.AddProject<WinLancher.WinLancherProject>(target);
			conf.AddProject<UnitTest.UnitTestProject>(target);
        }

		[Configure()]
        public void ConfigureWindows(Configuration conf, Target target)
		{
			conf.SolutionFileName = "[solution.Name]_[target.DevEnv]_win";
		}
		
        [Sharpmake.Main]
        public static void SharpmakeMain(Sharpmake.Arguments arguments)
        {
            arguments.Generate<StarWaySolution>();
        }
    }
}
