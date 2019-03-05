// CppPythonTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Python.h>
#include <ArrayObject.h>
#include "CppPythonTest.h"

////////////////////////////////////////////////////////////////////////////////

class State
{
public:
	float posx;
	float posy;
	float speed;
	float theta;
	float goalx;
	float goaly;
};

////////////////////////////////////////////////////////////////////////////////

class Experience
{
public:
	State State_;
	float Steering;
	float Throttle;
	float Reward;
	State NextState;
};

////////////////////////////////////////////////////////////////////////////////

class Agent
{
public:
	Agent()
	{
		Py_Initialize();
		auto pName = PyUnicode_FromString("Wrapper");
		auto pModule = PyImport_Import(pName);
		auto pDict = PyModule_GetDict(pModule);

		auto pInitializeFunc = PyDict_GetItemString(pDict, "Initialize");
		auto pInitializeArgs = PyTuple_New(0);
		Wrapper = PyObject_CallObject(pInitializeFunc, pInitializeArgs);

		GetActionFunc = PyDict_GetItemString(pDict, "GetAction");
		ProcessSARSFunc = PyDict_GetItemString(pDict, "Update");
	}
	~Agent()
	{

	}

	float Train(Experience * experience);
	void ChooseAction(float & outSteering, float & outThrottle, State * state);

	PyObject * Wrapper;
	PyObject * GetActionFunc;
	PyObject * ProcessSARSFunc;
};

////////////////////////////////////////////////////////////////////////////////

float Agent::Train(Experience * experience)
{
	auto pProcessArgs = PyTuple_New(4);

	const size_t cNumStateParams = 6;
	const size_t cNumActions = 2;

	auto pStateList = PyList_New(cNumStateParams);
	PyList_SetItem(pStateList, 0, PyFloat_FromDouble(experience->State_.posx));
	PyList_SetItem(pStateList, 1, PyFloat_FromDouble(experience->State_.posy));
	PyList_SetItem(pStateList, 2, PyFloat_FromDouble(experience->State_.speed));
	PyList_SetItem(pStateList, 3, PyFloat_FromDouble(experience->State_.theta));
	PyList_SetItem(pStateList, 4, PyFloat_FromDouble(experience->State_.goalx));
	PyList_SetItem(pStateList, 5, PyFloat_FromDouble(experience->State_.goaly));

	auto pNextStateList = PyList_New(cNumStateParams);
	PyList_SetItem(pNextStateList, 0, PyFloat_FromDouble(experience->NextState.posx));
	PyList_SetItem(pNextStateList, 1, PyFloat_FromDouble(experience->NextState.posy));
	PyList_SetItem(pNextStateList, 2, PyFloat_FromDouble(experience->NextState.speed));
	PyList_SetItem(pNextStateList, 3, PyFloat_FromDouble(experience->NextState.theta));
	PyList_SetItem(pNextStateList, 4, PyFloat_FromDouble(experience->NextState.goalx));
	PyList_SetItem(pNextStateList, 5, PyFloat_FromDouble(experience->NextState.goaly));

	auto pActionList = PyList_New(cNumActions);
	PyList_SetItem(pNextStateList, 0, PyFloat_FromDouble(experience->Steering));
	PyList_SetItem(pNextStateList, 1, PyFloat_FromDouble(experience->Throttle));

	PyTuple_SetItem(pProcessArgs, 0, pStateList);
	PyTuple_SetItem(pProcessArgs, 1, pActionList);
	PyTuple_SetItem(pProcessArgs, 2, PyFloat_FromDouble(experience->Reward));
	PyTuple_SetItem(pProcessArgs, 3, pNextStateList);

	auto error = PyObject_CallObject(ProcessSARSFunc, pProcessArgs);

	return PyFloat_AsDouble(error);
}

////////////////////////////////////////////////////////////////////////////////

void Agent::ChooseAction(float & outSteering, float & outThrottle, State * state)
{
	const size_t cNumStateParams = 6;
	auto pStateList = PyList_New(cNumStateParams);
	PyList_SetItem(pStateList, 0, PyFloat_FromDouble(state->posx));
	PyList_SetItem(pStateList, 1, PyFloat_FromDouble(state->posy));
	PyList_SetItem(pStateList, 2, PyFloat_FromDouble(state->speed));
	PyList_SetItem(pStateList, 3, PyFloat_FromDouble(state->theta));
	PyList_SetItem(pStateList, 4, PyFloat_FromDouble(state->goalx));
	PyList_SetItem(pStateList, 5, PyFloat_FromDouble(state->goaly));

	auto pGetActionArgs = PyTuple_New(2);
	PyTuple_SetItem(pGetActionArgs, 0, Wrapper);
	PyTuple_SetItem(pGetActionArgs, 1, pStateList);

	auto pActionChoices = (PyArrayObject *)PyObject_CallObject(GetActionFunc, pGetActionArgs);

	outSteering = *((float *)PyArray_GETPTR1(pActionChoices, 0));
	outThrottle = *((float *)PyArray_GETPTR1(pActionChoices, 1));
}

////////////////////////////////////////////////////////////////////////////////

CPPPYTHONTEST_API void * AgentInit()
{
	return new Agent();
}

////////////////////////////////////////////////////////////////////////////////

CPPPYTHONTEST_API void AgentTrain(float & outError, void * agent, void * experience)
{
	auto experience_ = reinterpret_cast<Experience *>(experience);
	auto agent_ = reinterpret_cast<Agent *>(agent);
	
	outError = agent_->Train(experience_);
}

////////////////////////////////////////////////////////////////////////////////

CPPPYTHONTEST_API void AgentChooseAction(float & outSteering, float & outThrottle, void * agent, void * state)
{
	auto state_ = reinterpret_cast<State *>(state);
	auto agent_ = reinterpret_cast<Agent *>(agent);

	agent_->ChooseAction(outSteering, outThrottle, state_);
}

////////////////////////////////////////////////////////////////////////////////