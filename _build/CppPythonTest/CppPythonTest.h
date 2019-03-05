#pragma once

#ifdef CPPPYTHONTEST_EXPORTS
#define CPPPYTHONTEST_API __declspec(dllexport)
#else
#define CPPPYTHONTEST_API __declspec(dllimport)
#endif

extern "C" CPPPYTHONTEST_API void* AgentInit();
extern "C" CPPPYTHONTEST_API void AgentTrain(float & outError, void* agent, void * experience);
extern "C" CPPPYTHONTEST_API void AgentChooseAction(float & outSteering, float & outThrottle, void* agent, void * state);
