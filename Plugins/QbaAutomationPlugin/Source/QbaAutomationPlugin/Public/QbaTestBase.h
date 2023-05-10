// Copyright Jakub Urbanek. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/UObjectGlobals.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Engine/EngineBaseTypes.h"

#define QBATESTS_API


// NOTE: The idea/shape of this base was originating from AITestsCommon.h to not reinvent the wheel


namespace FQbaTestHelpers
{
	 static FString DefaultAssetsSavePath{ TEXT("AAAA") };


	 template<typename ClassToSpawn>
	 ClassToSpawn* NewAutoDestroyObject(UObject* Outer = GetTransientPackage())
	 {
		 ClassToSpawn* ObjectInstance = NewObject<ClassToSpawn>(Outer);
		 ObjectInstance->AddToRoot();
		 SpawnedObjects.Add(ObjectInstance);
		 return ObjectInstance;
	 }
	// Create assets, get name for a test etc.

	 //;
	 //	
	 //	FORCEINLINE const TArray<TObjectPtr<UObject>>& GetCreatedAssets() const { return TestAssets; };
	 //FORCEINLINE void SetDefaultAssetSavePath(const FString& NewSavePath) { DefaultAssetsSavePath = NewSavePath; }
	 //	bool CreateNewAsset(const FString& AssetPath, const FString& AssetName);
	 //	
	 //	bool SaveAssets(TObjectPtr<UObject>& Asset);
	 //	bool SaveAssets(const FString& AssetPath);
	 //	bool SaveAssets(TArray<TObjectPtr<UObject>>& Assets);
	 //
	 //	bool DeleteAssets(TObjectPtr<UObject>& Asset);
	 //	bool DeleteAssets(const FString& AssetPath);
	 //	bool DeleteAssets(TArray<TObjectPtr<UObject>>& Assets);
	 //
	 //
	 //
	 //private:
	 //	FString DefaultAssetsSavePath{ TEXT("AAAA") };
	 //	TArray<TObjectPtr<UObject>> TestAssets;
}


struct QBATESTS_API FQbaTestBase
{

	FAutomationTestBase* TestRunner;

	FQbaTestBase()
		: TestRunner(nullptr){}

	FORCEINLINE FAutomationTestBase& GetTestRunner() const { check(TestRunner); return *TestRunner; }
	
	
	// TODO: hold assets created in test 
	const TArray<TObjectPtr<UObject>>& GetTestAssets() const { return TestAssets; }
	TArray<TObjectPtr<UObject>> TestAssets;
	//

public:
	FORCEINLINE virtual void SetTestRunner(FAutomationTestBase& AutomationTestInstance) { TestRunner = &AutomationTestInstance; }

	//Interface
	virtual ~FQbaTestBase() {};

	// Check if initialized properly, loading environment, required assets etc.
	virtual bool PrepareTest() { return false; }

	// Run logic of your test here 
	virtual bool RunTestLogic() { return false; }

	// Delete all necessary assets, finish test logic
	virtual bool FinishTest() { return false; }
};

class QBATESTS_API FQbaTestRunnerBase : public FQbaTestBase
{
public:
	//Interface
	virtual ~FQbaTestRunnerBase() {};
	
};

// @note that TestClass needs to derive from FQbaTestBase
#define IMPLEMENT_QBA_INSTANT_TEST(TestClass, PrettyName, TFlags) \
IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestClass##Runner, PrettyName, TFlags) \
bool TestClass##_Runner::RunTest(const FString& Parameters) \
{ \
	bool bTestSuccess {false}; \
	TestClass* TestInstance = new TestClass(); \
	TestInstance->SetTestRunner(*this); \
	if (TestInstance) \
	{ \
		bTestSuccess = TestInstance->PrepareTest(); \
		if (bTestSuccess) \
		{ \
			bTestSuccess = TestInstance->RunTestLogic(); \
			if (bTestSuccess) \
			{ \
				bTestSuccess = TestInstance->FinishTest(); \
			} \
		} \
	} \
		return bTestSuccess; \
} \

DEFINE_EXPORTED_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(QBATESTS_API, FQbaFinishTest, FQbaTestBase*, QbaTest);

// @note that TestClass needs to derive from FQbaTestBase
#define IMPLEMENT_QBA_LATENT_TEST(TestClass, PrettyName, TFlags) \
IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestClass##_Runner, PrettyName, TFlags) \
bool TestClass##_Runner::RunTest(const FString& Parameters) \
{ \
		bool bTestSuccess {false}; \
		TestClass* TestInstance = new TestClass(); \
		TestInstance->SetTestRunner(*this); \
		bTestSuccess = TestInstance->PrepareTest();\
		if (bTestSuccess)\
		{\
			bTestSuccess = TestInstance->RunTestLogic(); \
			ADD_LATENT_AUTOMATION_COMMAND(FQbaFinishTest(TestInstance));\
		};\
		/* run latent command to tear down */ \
		return bTestSuccess; \
}\

bool FQbaFinishTest::Update()
{
	return QbaTest && QbaTest->FinishTest();
}