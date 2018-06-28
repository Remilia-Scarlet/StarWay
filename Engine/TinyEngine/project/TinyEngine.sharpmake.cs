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
    }
}