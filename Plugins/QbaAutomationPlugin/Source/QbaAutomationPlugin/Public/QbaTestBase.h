// Copyright Jakub Urbanek. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/UObjectGlobals.h"
#include "Misc/AutomationTest.h"

#include "Tests/AutomationCommon.h"
#include "Engine/EngineBaseTypes.h"

#if WITH_EDITOR
#include "AssetRegistry/IAssetRegistry.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Tests/AutomationEditorCommon.h"
#include "ActorFactories/ActorFactory.h"
#include "Editor/UnrealEdEngine.h"
#include "Factories/Factory.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "ObjectTools.h"
#include "PackageTools.h"
#include "UObject/SavePackage.h"
#include "Editor.h"

#endif // WITH_EDITOR


#define QBATESTS_API
DECLARE_LOG_CATEGORY_EXTERN(LogQbaAutomation, Log, All);
DEFINE_LOG_CATEGORY(LogQbaAutomation);

// NOTE: The idea/shape of this base was originating from AITestsCommon.h + EditorAssetAutomationTests.cpp to not reinvent the wheel

PRAGMA_DISABLE_OPTIMIZATION
namespace FQbaTestHelpers
{
	// Only need to Specify Asset Class, Factory, Name and path
	struct FAssetCreation
	{
		FAssetCreation()
			: AssetClass(nullptr), Factory(nullptr), AssetPackage(nullptr), AssetName(FString(TEXT(""))), AssetPath(FString(TEXT(""))){}

		UClass* AssetClass{nullptr};
		UFactory* Factory{ nullptr };
		UPackage* AssetPackage{ nullptr };
		FString AssetName{ FString(TEXT("")) };
		FString AssetPath{FString(TEXT(""))};

	};

	static TObjectPtr<UObject> CreateAsset(FAssetCreation& AssetCreation)
	{
#if WITH_EDITOR
		if (!AssetCreation.AssetClass || !AssetCreation.Factory)
		{
			UE_LOG(LogQbaAutomation, Error, TEXT("Provided null class or factory for asset creation, aborting asset creation."));
			return nullptr;
		}
		const FString PackageName = AssetCreation.AssetPath + TEXT("/") + AssetCreation.AssetName;
		AssetCreation.AssetPackage = CreatePackage(*PackageName);
		EObjectFlags Flags = RF_Public | RF_Standalone;
		
		TObjectPtr<UObject> CreatedAsset = AssetCreation.Factory->FactoryCreateNew(AssetCreation.AssetClass, AssetCreation.AssetPackage, FName(*AssetCreation.AssetName), Flags, NULL, GWarn);

		if (CreatedAsset)
		{
			FAssetRegistryModule::AssetCreated(CreatedAsset);
			AssetCreation.AssetPackage->MarkPackageDirty();
			UE_LOG(LogQbaAutomation, VeryVerbose, TEXT("Asset %s correctly created"), *AssetCreation.AssetName);
			return CreatedAsset;
		}
		else
		{
			UE_LOG(LogQbaAutomation, Error, TEXT("Could not create %s asset"), *AssetCreation.AssetClass->GetName());
		}
#else
		UE_LOG(LogQbaAutomation, Error, TEXT("Cannot create new assets outside of editor"));
#endif // WITH_EDITOR
		return nullptr;
	};

	// Only need to Specify Asset Class, Name and path
	struct FBlueprintCreation
	{
		FBlueprintCreation()
			: AssetClass(nullptr), Package(nullptr), AssetName(FString(TEXT(""))), AssetPath(FString(TEXT(""))) {}

		UClass* AssetClass{ nullptr };
		UPackage* Package{ nullptr };
		FString AssetName{ FString(TEXT("")) };
		FString AssetPath{ FString(TEXT("")) };

	};
	static TObjectPtr<UObject> CreateBlueprint(FBlueprintCreation& BlueprintCreation)
	{
//#if WITH_EDITOR
		if (!BlueprintCreation.AssetClass)
		{
			UE_LOG(LogQbaAutomation, Error, TEXT("Provided null class for blueprint creation, aborting blueprint creation."));
		}
		const FString PackageName = BlueprintCreation.AssetPath + TEXT("/") + BlueprintCreation.AssetName;
		BlueprintCreation.Package = CreatePackage(*PackageName);

		if (BlueprintCreation.Package)
		{
			const FName BlueprintName = MakeUniqueObjectName(BlueprintCreation.Package, BlueprintCreation.AssetClass, FName(*BlueprintCreation.AssetName));

			UBlueprint* CreatedBlueprint = FKismetEditorUtilities::CreateBlueprint(BlueprintCreation.AssetClass, BlueprintCreation.Package, BlueprintName, BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass());
			if (CreatedBlueprint)
			{
				UE_LOG(LogQbaAutomation, VeryVerbose, TEXT("Succesully created new blueprint"));
				CreatedBlueprint->SetFlags(RF_Public | RF_Standalone);
				FAssetRegistryModule::AssetCreated(CreatedBlueprint);
				BlueprintCreation.Package->MarkPackageDirty();

				return CreatedBlueprint;
			}
			else
			{
				UE_LOG(LogQbaAutomation, Error, TEXT("Failed to create new blueprint"));
				return nullptr;
			}
		}
//#else
//		UE_LOG(LogQbaAutomation, Error, TEXT("Cannot create new assets outside of editor"));
//#endif // WITH_EDITOR
		return nullptr;
	}

	static bool SaveAsset(TObjectPtr<UObject> AssetToSave) 
	{ 
#if WITH_EDITOR
		if (AssetToSave)
		{
			UPackage* Package = AssetToSave->GetPackage();
			if (Package)
			{
				const FString PackageName = Package->GetName();
				const FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());

				FSavePackageArgs SaveArgs;
				{
					SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
					SaveArgs.SaveFlags = SAVE_NoError;
				}

				const bool bSucceeded = UPackage::SavePackage(Package, nullptr, *PackageFileName, SaveArgs);
				if (bSucceeded)
				{
					UE_LOG(LogQbaAutomation, VeryVerbose, TEXT("Successfully saved asset"));
				}
				else
				{
					UE_LOG(LogQbaAutomation, Error, TEXT("Could not save the %s asset"), *AssetToSave->GetName());
				}
				return bSucceeded;
			}
		}
#else
		UE_LOG(LogQbaAutomation, Error, TEXT("Cannot save assets in non editor context!"));
#endif // WITH_EDITOR
		return false;
	};

	static bool DeleteAsset(TObjectPtr<UObject> AssetToDelete)
	{
#if WITH_EDITOR
		if (AssetToDelete)
		{
			bool bSuccessfulDelete = false;

			FAutomationEditorCommonUtils::NullReferencesToObject(AssetToDelete);
			bSuccessfulDelete = ObjectTools::DeleteSingleObject(AssetToDelete, false);

			if (bSuccessfulDelete)
			{
				UPackage* Package = AssetToDelete->GetPackage();
				FString PackageFilename;
				if (FPackageName::DoesPackageExist(Package->GetName(), &PackageFilename))
				{
					TArray<UPackage*> PackagesToDelete;
					PackagesToDelete.Add(Package);

					Package->SetDirtyFlag(false);
					UPackageTools::UnloadPackages(PackagesToDelete);

					IFileManager::Get().Delete(*PackageFilename);
					UE_LOG(LogQbaAutomation, VeryVerbose, TEXT("Succesfully deleted asset"));
					return true;
				}
			}
			else
			{
				UE_LOG(LogQbaAutomation, Error, TEXT("Cannot delete: %s"), *AssetToDelete->GetName());
				return false;
			}
		}
#else
		UE_LOG(LogQbaAutomation, Error, TEXT("Cannot delete assets in non editor context!"));
#endif // WITH_EDITOR
		return false;
	};


	static FString DefaultAssetPath{ FString(TEXT("/Game/QbaAutomation")) };
	static FString GetPathForAsset(const FString& TestName, const FString& TargetPath = DefaultAssetPath) { return FString::Printf(TEXT("%s/%s_Test"), *TargetPath, *TestName); };
}
PRAGMA_ENABLE_OPTIMIZATION

struct QBATESTS_API FQbaTestBase
{
	FAutomationTestBase* TestRunner;

	FQbaTestBase()
		: TestRunner(nullptr){}

	FORCEINLINE FAutomationTestBase& GetTestRunner() const { check(TestRunner); return *TestRunner; }
	
	FORCEINLINE const TArray<TObjectPtr<UObject>>& GetTestAssets() const { return TestAssets; }
	FORCEINLINE void AddAssetToTestAssets(const TObjectPtr<UObject> AssetToAdd) { TestAssets.Add(AssetToAdd); }

	TArray<TObjectPtr<UObject>> TestAssets;
	bool SaveAllTestAssets()
	{
		bool bSuccessSaveAllAssets{ true };
		for (TObjectPtr<UObject>& Asset : TestAssets)
		{
			const bool bDeletedAsset = FQbaTestHelpers::SaveAsset(Asset);
			if (!bDeletedAsset)
			{
				bSuccessSaveAllAssets = false;
			}
			GetTestRunner().TestTrue(TEXT("Save all test assets: "), bSuccessSaveAllAssets);
		}
		return true;
	};

	bool DeleteAllTestAssets()
	{
		bool bSuccessDeleteAssets{ true };
		for (TObjectPtr<UObject>& Asset : TestAssets)
		{
			const bool bDeletedAsset = FQbaTestHelpers::DeleteAsset(Asset);
			if (!bDeletedAsset)
			{
				bSuccessDeleteAssets = false;
			}
			GetTestRunner().TestTrue(TEXT("Delete all assets: "), bSuccessDeleteAssets);

			//TODO: Delete directory
		}
		return bSuccessDeleteAssets;
	};


public:
	FORCEINLINE virtual void SetTestRunner(FAutomationTestBase& AutomationTestInstance) { TestRunner = &AutomationTestInstance; }

	//Interface
	virtual ~FQbaTestBase() {};

	// Check if initialized properly, loading environment, required assets etc.
	virtual bool PrepareTest() { return false; }

	// Run logic of your test here 
	virtual bool RunTestLogic() { return false; }

	// Delete all necessary assets, finish test logic
	virtual bool FinishTest() {  DeleteAllTestAssets(); return true; }

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