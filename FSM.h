
// - Mario Palmero [2017], zlib/libpng licensed.

#pragma once

#include <vector>
#include <functional>

#include "FSMData.h"

/*
 * Interface for the FSM
 * FSM<EnumState> myFSM;
 * or
 * class FSMChild : public FSM<EnumState> {};
 * FSMChild myFSM;
 * myFSM.SetState(EnumState); // This calls to the EndState of the current State and to the BeginState of the new State
 * EnumState currentState = myFSM.GetCurrentState();
 * myFSM.SetBeginStateFunction(EnumState, std::function<void(EnumState newState)>);
 * myFSM.SetEndStateFunction(EnumState, std::function<void(EnumState previousState>);
 * myFSM.Update(deltaSeconds);
 *
 * Almost every control variable should be read and witten in the FSMData, so it's serialized directly
 * In the FSMChild should be only data that doesn't need serialization
 */

template<typename E, int SIZE>
class FSM
{
public:
	FSM();
	FSM(E startingState);

	E GetCurrentState() const;
	void SetState(E newState);
	void Update(float deltaSeconds);

	FSMData* SaveFSM();
	void LoadFSM(FSMData* newData);

protected:
	void SetBeginStateFunction(E state, std::function<void(E)> newBeginFunction);
	void SetEndStateFunction(E state, std::function<void(E)> newEndFunction);
	void SetUpdateFunction(E state, std::function<void(float)> newUpdateFunction);

	FSMData*									m_data;
private:
	E											m_currentState;
	std::vector<std::function<void(E)>>			m_beginFunctions;
	std::vector<std::function<void(E)>>			m_endFunctions;
	std::vector<std::function<void(float)>>		m_updateFunctions;

};


template<typename E, int SIZE>
inline FSM<E, SIZE>::FSM()
{
	// Initialize functions maps
	for (int i = 0; i < SIZE; ++i)
	{
		m_beginFunctions.push_back({});
		m_endFunctions.push_back({});
		m_updateFunctions.push_back({});
	}

	m_currentState = E(0);
}

template<typename E, int SIZE>
inline FSM<E, SIZE>::FSM(E startingState)
{
	// Initialize functions maps
	for (int i = 0; i < int(SIZE); ++i)
	{
		m_beginFunctions.push_back({});
		m_endFunctions.push_back({});
		m_updateFunctions.push_back({});
	}

	m_currentState = startingState;
}

template<typename E, int SIZE>
inline E FSM<E, SIZE>::GetCurrentState() const
{
	return m_currentState;
}

template<typename E, int SIZE>
inline void FSM<E, SIZE>::SetState(E newState)
{
	m_endFunctions[m_currentState](newState);
	m_beginFunctions[newState](m_currentState);
	m_currentState = newState;
}

template<typename E, int SIZE>
inline void FSM<E, SIZE>::Update(float deltaSeconds)
{
	m_updateFunctions[m_currentState](deltaSeconds);
}

template<typename E, int SIZE>
inline FSMData* FSM<E, SIZE>::SaveFSM()
{
	m_data->currentState = m_currentState;
	return m_data;
}

template<typename E, int SIZE>
inline void FSM<E, SIZE>::LoadFSM(FSMData* newData)
{
	m_currentState = m_data->currentState;
	*m_data = *newData;
}

template<typename E, int SIZE>
inline void FSM<E, SIZE>::SetBeginStateFunction(E state, std::function<void(E)> newBeginFunction)
{
	m_beginFunctions[state] = newBeginFunction;
}

template<typename E, int SIZE>
inline void FSM<E, SIZE>::SetEndStateFunction(E state, std::function<void(E)> newEndFunction)
{
	m_endFunctions[state] = newEndFunction;
}

template<typename E, int SIZE>
inline void FSM<E, SIZE>::SetUpdateFunction(E state, std::function<void(float)> newUpdateFunction)
{
	m_updateFunctions[state] = newUpdateFunction;
}
