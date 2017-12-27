using Sharpmake;

namespace Common
{
    public class Common
    {
        public static string GetDevEnvString(DevEnv env)
        {
            switch (env)
            {
                case DevEnv.vs2010: return "2010";
                case DevEnv.vs2012: return "2012";
                case DevEnv.vs2013: return "2013";
                default: return "";
            }
        }

        // Splitting 2008 for Framework v4.0 since it is not supported
        public static Target[] GetDefaultTargets()
        {
            return new Target[]{
            new Target(
            Platform.anycpu,
            DevEnv.vs2010 | DevEnv.vs2013,
            Optimization.Debug | Optimization.Release,
            OutputType.Dll,
            Blob.NoBlob,
            BuildSystem.MSBuild,
            DotNetFramework.v3_5 | DotNetFramework.v4_0)};
        }
    }

    [Sharpmake.Generate]
    public class ProjectTemplate : CSharpProject
    {
        public ProjectTemplate()
        {
            RootPath = @"[project.SharpmakeCsPath]\codebase\";

            // This Path will be used to get all SourceFiles in this Folder and all subFolders
            SourceRootPath = @"[project.RootPath]\[project.Name]";

            AddTargets(Common.GetDefaultTargets());

            // if set to true, dependencies that the project uses will be copied to the output directory
            DependenciesCopyLocal = DependenciesCopyLocalTypes.Default;

            // Set to null if you don't want to use Perforce
            PerforceRootPath = null;

            // Files put in this directory will be added to the project as resources (linked) build Action
            ResourcesPath = RootPath + @"\Resources\";


            // Files put in this directory will be added to the project as Content build Action
            ContentPath = RootPath + @"\Content\";

            //Specify if we want the project file to be LowerCase
            IsFileNameToLower = false;
        }

        [Configure()]
        public virtual void ConfigureAll(Configuration conf, Target target)
        {
            //-----------------OutputPath----------------------//

            // Path where the binaries will be stored
            conf.TargetPath = @"[conf.ProjectPath]\[project.OutputPathName]\[target.DevEnv]\[target.Framework]\[target.Platform]\[conf.Name]";

            // Visual Studio Default:
            //conf.TargetPath = string.Format(@"[conf.ProjectPath]\{0}", OutputPathName);

            // Choose between WindowsApplication ConsoleApplication or ClassLibraries
            conf.Output = Project.Configuration.OutputType.DotNetConsoleApp;

            // Sets the ProjectFileName for project where ProjectFileName isn't modified by configurations
            // Visual Studio Default:
            // public static string DefaultProjectFileName = "[project.Name]";
            conf.ProjectFileName = "[project.Name].[target.DevEnv].[target.Framework]";


            conf.ReferencesByName.AddRange(new Strings("System",
                                                        "System.Core",
                                                        "System.Xml.Linq",
                                                        "System.Data.DataSetExtensions",
                                                        "System.Data",
                                                        "System.Xml"));

            // Sets where the project file (csproj) will be saved
            conf.ProjectPath = @"[project.RootPath]\[project.Name]";


            //----------------IntermediatePath-----------------//

            // Usually the obj folder created to link files
            // Note: Due to a Visual Studio known Bug
            // The obj folder might still be created, but should be empty at the end of the build, if removed the rebuild project fonction won't work
            conf.IntermediatePath = @"[project.SharpmakeCsPath]\..\[project.IntermediatePathName]\[project.Name]\[target.DevEnv]\[target.Framework]\[target.Platform]";
            // Visual Studio Default:
            //public static string IntermediatePath = string.Format(@"[conf.ProjectPath]\{0}", IntermediatePathName);
        }
    }

    [Sharpmake.Generate]
    public class SolutionTemplate : CSharpSolution
    {
        public SolutionTemplate()
        {
            AddTargets(Common.GetDefaultTargets());
        }
        [Configure()]
        public virtual void ConfigureAll(Configuration conf, Target target)
        {
            conf.SolutionFileName = string.Format("{0}.{1}.{2}",
                Name,
                Common.GetDevEnvString(target.DevEnv),
                target.Framework.ToVersionString());
            conf.SolutionPath = @"[solution.SharpmakeCsPath]\codebase\";
        }
    }
}