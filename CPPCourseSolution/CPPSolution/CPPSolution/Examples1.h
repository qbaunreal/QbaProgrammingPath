#pragma once

//basics
class Section6
{
public:
	Section6();

private:
	const unsigned int EstimateDays{ 30 };
	const float PricePerRoom{ 30.f };
	const float TaxRate{ 0.06f };
};

// Arrays and vectors
class Section7
{
public:
	Section7();

private:


	struct TestArrays
	{
		int TestArray1[5]{0}; //this inits all to 0
		int TestArray2[4]{ 2,1,3,7 };
		int TestArray3[10]{ 2,7 };
		float TestFloatArray1[3]{ 0.1f, 30.f, 60.2f };
		float MultiDimension[2][3]
		{ 
			{ 2, 1 ,0 },
			{3, 5, 10} 
		};
		char CharArray[5]{ 'a', 'b', 'c', 'd', 'e' };
		//NOTEL you cannot declare vector in header, dunno why maybe because it's dynamic. 
		//As i remember i could not do the "dynamic" arrays either (the ones that were guessing size based on recieved params in {} 
	};


};

class Section8
{
public:
	Section8();

};
