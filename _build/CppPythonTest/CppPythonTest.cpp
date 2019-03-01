// CppPythonTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Python.h>
#include <ArrayObject.h>

// Code adapted from https://www.coveros.com/calling-python-code-from-c/

int main()
{
	printf("Calling Python to find the sum of 2 and 2.\n");

	// Initialize the Python interpreter.
	Py_Initialize();

	// Create some Python objects that will later be assigned values.
	PyObject *pName, *pModule, *pDict, *pValue;

	// Convert the file name to a Python string.
	pName = PyUnicode_FromString("Wrapper");

	// Import the file as a Python module.
	pModule = PyImport_Import(pName);
	if (pModule == nullptr)
	{
		PyErr_Print();
		std::exit(1);
	}

	// Create a dictionary for the contents of the module.
	pDict = PyModule_GetDict(pModule);

	// Get the add method from the dictionary.
	auto pInitializeFunc = PyDict_GetItemString(pDict, "Initialize");
	auto pGetActionFunc = PyDict_GetItemString(pDict, "GetAction");

	// Create a Python tuple to hold the arguments to the method.
	auto pInitializeArgs = PyTuple_New(0);
	auto pWrapper = PyObject_CallObject(pInitializeFunc, pInitializeArgs);

	// Create state representation using list
	const size_t cNumStateParams = 6;
	auto pStateList = PyList_New(cNumStateParams);
	
	auto tempValue = PyLong_FromLong(0);
	for (auto i = 0; i < cNumStateParams; i++)
		PyList_SetItem(pStateList, i, tempValue);

	auto pGetActionArgs = PyTuple_New(2);
	PyTuple_SetItem(pGetActionArgs, 0, pWrapper);
	PyTuple_SetItem(pGetActionArgs, 1, pStateList);

	auto pActionChoices = PyObject_CallObject(pGetActionFunc, pGetActionArgs);

	if (pActionChoices == nullptr)
	{
		PyErr_Print();
		std::exit(1);
	}

	float steering = *((float *)PyArray_GETPTR1(pActionChoices, 0));
	float gas = *((float *)PyArray_GETPTR1(pActionChoices, 1));

	printf("Steering : %f\n", steering);
	printf("Gas : %f\n", gas);

	// Convert 2 to a Python integer.
	//pValue = PyLong_FromLong(2);

	// Set the Python int as the first and second arguments to the method.
	// Call the function with the arguments.
	//PyObject* pResult = PyObject_CallObject(pFunc, pArgs);

	// Print a message if calling the method failed.
	//if (pResult == NULL)
	//	printf("Calling the add method failed.\n");

	// Convert the result to a long from a Python object.
	//long result = PyLong_AsLong(pResult);

	// Destroy the Python interpreter.
	// Note - See https://stackoverflow.com/questions/27844676/assertionerror-3-x-only-when-calling-py-finalize-with-threads
	Py_DECREF(PyImport_ImportModule("threading"));
	Py_Finalize();

	return 0;
}
