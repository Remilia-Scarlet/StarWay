#pragma once


namespace Dango
{
	struct GameConfig
	{
		int width = 1;
		int height = 1;
		int fps = 30;
	};
	
	class Game
	{
	public:
		Game() = default;
		virtual ~Game() = default;

	public:
		//once game start, call this. Before anyothers
		virtual bool preInit(const std::string& cmdLine) = 0;
		//window created call this
		virtual bool init(Ash::NativeWindow nativeWindow) = 0;

		virtual bool start() = 0;

		virtual bool cleanUp() = 0;

		virtual bool destroy() = 0;

		virtual const GameConfig& getGameConfig() const { return _gameConfig; };
	protected:
		GameConfig _gameConfig;
	};
}

#define DEFINE_GAME(GAME) \
	static std::unique_ptr<GAME> dangoCreateGameInstance(){return std::make_unique<GAME>();}