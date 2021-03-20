#include "gtest/gtest.h"
#include "Ash/CommandLineCfg/CommandLineCfg.h"

TEST(Ash, CommonCommandLineTest)
{
	class CommandLineCfgTest : public Ash::CommandLineCfg
	{
	public:
		DEFINE_COMMANDLINE_BOOL(Renderdoc, "/Renderdoc:Enable renderdoc in-app api"); //Add your bool or int or string commandline option here.
		DEFINE_COMMANDLINE_STR(Name, "default name", "/Name:[str], Some hint");          //Add your bool or int or string commandline option here.
		DEFINE_COMMANDLINE_INT(Number, 0, "/Number:SomeDesc");
	};

	{
		CommandLineCfgTest cmdLine;
		cmdLine.init(" asdf fdsag \"fdas\" /Renderdoc /Name:233 /Number:244 /SomeArgThatNotDefined:good /SomeArgThatNotDefined2");
		EXPECT_EQ(cmdLine.getName(), "233");
		EXPECT_EQ(cmdLine.getNumber(), 244);
		EXPECT_EQ(cmdLine.getRenderdoc(), true);
		auto it = cmdLine.getSplitedCommandLine().find(toLower("SomeArgThatNotDefined"));
		EXPECT_NE(it, cmdLine.getSplitedCommandLine().end());
		EXPECT_EQ(it->second, "good");
		it = cmdLine.getSplitedCommandLine().find(toLower("SomeArgThatNotDefined2"));
		EXPECT_NE(it, cmdLine.getSplitedCommandLine().end());
		EXPECT_EQ(it->second, "");
	}
	{
		CommandLineCfgTest cmdLine;
		cmdLine.init("/Name:df\"f dg g \"fsda");
		EXPECT_EQ(cmdLine.getName(), "dff dg g fsda");
	}
	{
		CommandLineCfgTest cmdLine;
		std::string helperString = cmdLine.generateHelpString();
		EXPECT_EQ(helperString, "/Name:[str], Some hint\n/Number:SomeDesc\n/Renderdoc:Enable renderdoc in-app api\n");
	}
}
