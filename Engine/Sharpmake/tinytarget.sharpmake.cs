using Sharpmake;
using System;

namespace Common
{
	[Fragment, Flags]
	public enum Mode
	{
		EngineMode = 1,
		ToolMode = 2
	}
    public class TinyTarget : Target
    {
		public static TinyTarget GetEngineTarget()
		{
			return new TinyTarget(
                    Platform.win32 | Platform.win64,
                    DevEnv.vs2017,
                    Optimization.Debug | Optimization.Release | Optimization.Retail,
					Mode.EngineMode,
					OutputType.Lib,
					Blob.NoBlob/* | Blob.Blob */);
		}
		public static TinyTarget GetToolTarget()
		{
			return new TinyTarget(
                    Platform.win32 | Platform.win64,
                    DevEnv.vs2017,
                    Optimization.Debug | Optimization.Release | Optimization.Retail,
					Mode.ToolMode,
					OutputType.Lib,
					Blob.NoBlob/* | Blob.Blob */);
		}
		public static TinyTarget GetAllTarget()
		{
			return new TinyTarget(
                    Platform.win32 | Platform.win64,
                    DevEnv.vs2017,
                    Optimization.Debug | Optimization.Release | Optimization.Retail,
					Mode.ToolMode | Mode.EngineMode,
					OutputType.Lib,
					Blob.NoBlob/* | Blob.Blob */);
		}
		public Mode Mode;
		public TinyTarget(){}
		public TinyTarget(
		    Platform platform,
            DevEnv devEnv,
            Optimization optimization,
			Mode mode,
            OutputType outputType = OutputType.Lib,
            Blob blob = Blob.NoBlob,
            BuildSystem buildSystem = BuildSystem.MSBuild,
            DotNetFramework framework = DotNetFramework.v3_5
		)
			:base(platform,devEnv,optimization,outputType,blob,buildSystem,framework)
		{
			Mode = mode;
		}
    }
}