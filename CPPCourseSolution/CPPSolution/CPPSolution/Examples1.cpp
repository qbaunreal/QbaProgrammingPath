
#include "Examples1.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Section6::Section6()
{
	unsigned int NumberOfRooms{};

	cout << "Enter number of rooms: ";
	cin >> NumberOfRooms;

	const float CostOfRooms = PricePerRoom * NumberOfRooms;
	const float Tax = CostOfRooms * TaxRate;
	const float TotalPrice = CostOfRooms + Tax;

	cout << "Costs for " << NumberOfRooms << " rooms are:" << endl << endl;
	cout << "Cost of rooms only: " << CostOfRooms << endl;
	cout << "Tax: " << Tax << endl;
	cout << "Total Price: " << TotalPrice << endl;
	cout << "This estimate is valid for: " << EstimateDays << " days." << endl;
}


template<typename T, int ArraySize>
void PrintArrayElems(const T(&Array)[ArraySize])
{
	T ArrayLenght = sizeof(Array) / sizeof(T);
	for (int i = 0; i < ArrayLenght; i++)
	{
		cout << "Array elem on position: " << i << " is: " << Array[i] << endl;
	}
}

template<typename T>
void PrintVectorElems(const vector<T>& InVector)
{
	for (int i = 0; i < InVector.size(); i++)
	{
		cout << "Vector elem on position: " << i << " is: " << InVector.at(i) << endl;
	}
}

template<typename T>
void Print2DVectorElems(const vector<vector<T>>& InVector)
{
	if constexpr (is_integral_v<T>)  //Q: big question time: why have i use contexpr here? otherwise it does not compile correctly
	{								// found "Used where code needs to be executed based on a run-time or compile-time (since C++17) condition" not sure why it can't just be normal if
									// behaves the same for:   is_same_v< T, int > , is_same< T, int >::value, is_integral_v<T>
									//basically without contexpr it ommits this if :thonking:
									// what i've found that this expression should contexpr inline bool, 
									//I have no clue how they work and why tf had i put the word before nawias () and not inside the expression
									// it's not usable at all in this function but I was curious and was testing some stuff
	}
	else
	{
		cout << "Could not convert to int" << endl;
	}

	for (int i = 0; i < InVector.size(); i++)
	{
		for (int j = 0; j < InVector[i].size(); j++)
		{
			cout << InVector[i][j] << endl;
		}
	}
}

template<typename T, int FirstDimention, int SecondDimention>
void Print2DArrayElems(const T(&Array)[FirstDimention][SecondDimention]) //probably there is some way to access array 
{
	for (int i = 0; i < FirstDimention; i++)
	{
		for (int j = 0; j < SecondDimention; j++)
		{
			cout << "Row: " << i << " column: " << j << " value: " << Array[i][j] << endl;
		}
	}

}

Section7::Section7()
{
	cout << "------ Printing Arrays ---------" << endl;
	TestArrays Section7Array = TestArrays();
	PrintArrayElems(Section7Array.TestFloatArray1);
	Print2DArrayElems(Section7Array.MultiDimension);
	PrintArrayElems(Section7Array.CharArray);

	cout << "------ Printing 1d Vectors ---------" << endl;
	vector <float> SomeVectorRecord{ 2,1,3,7 };
	SomeVectorRecord.push_back(2137);
	PrintVectorElems(SomeVectorRecord);
	vector <float> OtherVector(10, 21.37f);
	PrintVectorElems(OtherVector);

	cout << "------ Printing 2d Vectors ---------" << endl;
	vector<vector<float>> TwoDimensionalVector;
	TwoDimensionalVector.push_back(SomeVectorRecord);
	TwoDimensionalVector.push_back(OtherVector);
	Print2DVectorElems(TwoDimensionalVector);
}

Section8::Section8()
{



}
