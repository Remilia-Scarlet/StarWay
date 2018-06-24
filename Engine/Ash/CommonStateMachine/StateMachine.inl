
template<class ItemType>
inline State<ItemType>& State<ItemType>::run(ItemType item)
{
	for (StateInfo& it : _chageStatesInfo)
	{
		const bool changeState = it._judgeFun(item);
		if (changeState)
		{
			State<ItemType>& nextState = it._state;
			if (it._changeCb)
				it._changeCb(item);
			if (_onExitCb)
				_onExitCb(nextState);
			if (nextState._onEnterCb)
				nextState._onEnterCb(*this);
			return nextState;
		}
	}
	if(_handleFun)
		_handleFun(item);
	return *this;
}

template <class ItemType>
State<ItemType>& StateMachine<ItemType>::createState(const std::string& stateName)
{
	auto pr = _states.emplace(std::piecewise_construct, std::forward_as_tuple(stateName), std::forward_as_tuple(stateName));
	State<ItemType>& state = pr.first->second;
	return state;
}

template <class ItemType>
State<ItemType>& StateMachine<ItemType>::getState(const std::string& stateName)
{
	auto it = _states.find(stateName);
	if (it != _states.end())
		return it->second;

	// we can't find the state
	TinyAssert(false);
	return createState(stateName);
}

template <class ItemType>
void StateMachine<ItemType>::run(State<ItemType>& startState,
	std::function<std::optional<ItemType>()> elementProvider)
{
	std::optional<ItemType> item = elementProvider();
	if (!item)
		return;
	if (startState._onEnterCb)
	{
		startState._onEnterCb(startState);
	}
	State<ItemType>* nextState = &startState;
	do
	{
		nextState = &nextState->run(item.value());
		item = elementProvider();
	} while (item);
}
