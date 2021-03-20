#pragma once
#include <utility>
#include <vector>
#include <functional>
#include <memory>
#include <optional>
#include <map>

namespace Ash
{
	template <class ItemType>
	class StateMachine;

	class StateUserData
	{
	public:
		virtual ~StateUserData() = default;
	};

	template <class ItemType>
	class State
	{
		friend class StateMachine<ItemType>;
		template<class _Ty1, class _Ty2>
		friend struct std::pair;
	protected:
		//Do not new a State manually, use StateMachine::createState
		State(std::string stateName)
			: _name(std::move(stateName))
		{
		}
		State(State&& other) = default;
		State& operator=(State&& other) = default;
	public:
		State(const State& other) = delete;
		State& operator=(const State& other) = delete;
		~State() = default;
	public:
		// If judgeFun returns true, then state will change to target state. Before changing, changeCb will be called
		void DefineChangeTo(State<ItemType>& target, std::function<bool(ItemType)> judgeFun,
			std::function<void(ItemType)> changeCb = [](ItemType) {})
		{
			_chageStatesInfo.emplace_back(std::move(StateInfo{ target, std::move(judgeFun), std::move(changeCb) }));
		}

		// If all change fun returned false, then this handleFun will be called.
		void DefineHandleFun(std::function<void(ItemType)> handleFun)
		{
			_handleFun = std::move(handleFun);
		}

		// When state will enter, onEnterCb will be called. Param is last state. If this state is init state, the param will be itself
		void onEnter(std::function<void(State<ItemType>&)> onEnterCb)
		{
			_onEnterCb = std::move(onEnterCb);
		}

		// When state will exit, onExitCb will be called. Param is the next state.
		void onExit(std::function<void(State<ItemType>&)> onExitCb)
		{
			_onEnterCb = std::move(onExitCb);
		}

		template <class UserDataType>
		UserDataType& setUserData(const std::shared_ptr<UserDataType>& userData)
		{
			_userData = std::static_pointer_cast<StateUserData>(userData);
			return getUserData<UserDataType>();
		}

		template <class UserDataType>
		UserDataType& getUserData()
		{
			return static_cast<UserDataType&>(*_userData);
		}

	protected:
		State<ItemType>& run(ItemType item);

		std::string _name;
		std::shared_ptr<StateUserData> _userData = nullptr;
		std::function<void(State<ItemType>&)> _onEnterCb;
		std::function<void(State<ItemType>&)> _onExitCb;
		struct StateInfo
		{
			StateInfo(State<ItemType>& state, std::function<bool(ItemType)> judgeFun, std::function<void(ItemType)> changeCb)
				: _state(state), _judgeFun(std::move(judgeFun)), _changeCb(std::move(changeCb))
			{
				TinyAssert(static_cast<bool>(_judgeFun));
			}
			StateInfo(const StateInfo& other) = default;
			StateInfo(StateInfo&& other) = default;
			StateInfo& operator=(const StateInfo& other) = delete;
			StateInfo& operator=(StateInfo&& other) = default;
			~StateInfo() = default;

			State<ItemType>& _state;
			std::function<bool(ItemType)> _judgeFun;
			std::function<void(ItemType)> _changeCb;
		};
		std::vector<StateInfo> _chageStatesInfo;
		std::function<void(ItemType)> _handleFun;
	};

	template <class ItemType>
	class StateMachine
	{
	public:
		State<ItemType>& createState(const std::string& stateName);
		State<ItemType>& getState(const std::string& stateName);
		void run(State<ItemType>& startState, std::function<std::optional<ItemType>(void)> elementProvider);
	protected:
		std::map<std::string, State<ItemType>> _states;
	};

#include "StateMachine.inl"

}