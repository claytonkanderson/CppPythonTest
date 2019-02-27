// CppPythonTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Python.h>

// Code adapted from https://www.coveros.com/calling-python-code-from-c/

int main()
{
	printf("Calling Python to find the sum of 2 and 2.\n");

	// Initialize the Python interpreter.
	Py_Initialize();

	// Create some Python objects that will later be assigned values.
	PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pValue;

	// Convert the file name to a Python string.
	pName = PyUnicode_FromString("Sample");
	assert(pName);
	assert(nullptr);

	// Import the file as a Python module.
	pModule = PyImport_Import(pName);

	// Create a dictionary for the contents of the module.
	pDict = PyModule_GetDict(pModule);

	// Get the add method from the dictionary.
	pFunc = PyDict_GetItemString(pDict, "add");

	// Create a Python tuple to hold the arguments to the method.
	pArgs = PyTuple_New(2);

	// Convert 2 to a Python integer.
	pValue = PyLong_FromLong(2);

	// Set the Python int as the first and second arguments to the method.
	PyTuple_SetItem(pArgs, 0, pValue);
	PyTuple_SetItem(pArgs, 1, pValue);

	// Call the function with the arguments.
	PyObject* pResult = PyObject_CallObject(pFunc, pArgs);

	// Print a message if calling the method failed.
	if (pResult == NULL)
		printf("Calling the add method failed.\n");

	// Convert the result to a long from a Python object.
	long result = PyLong_AsLong(pResult);

	// Destroy the Python interpreter.
	Py_Finalize();

	// Print the result.
	printf("The result is %d.\n", result); 
	//std::cin.ignore(); 
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
