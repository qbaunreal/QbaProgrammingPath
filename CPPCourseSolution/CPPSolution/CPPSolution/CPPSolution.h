#pragma once


template<class T>
void ConstructSectionFromClass()
{
	T* Section = new T();
	delete Section;
}



// My TODO for this CPP course:
// Get better at templating
// Try to make delegates
// static classes, functions and variables
// static structs / structs in general
// 