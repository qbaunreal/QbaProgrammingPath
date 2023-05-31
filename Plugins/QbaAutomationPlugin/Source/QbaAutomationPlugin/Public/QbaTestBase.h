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
#include "Subsystems/AssetEditorSubsystem.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "ObjectTools.h"
#include "PackageTools.h"
#include "UObject/SavePackage.h"
#include "Editor.h"

#endif // WITH_EDITOR


#define QBATESTS_API
DECLARE_LOG_CATEGORY_EXTERN(LogQbaAutomation, Log, All);
DEFINE_LOG_CATEGORY(LogQbaAutomation);


PRAGMA_DISABLE_OPTIMIZATION
namespace QBATESTS_API FQbaTestHelpers //TODO move helpers to other file.
{
	struct FAssetCreationData
	{
		UClass* AssetClass{nullptr};
		UFactory* Factory{ nullptr };
		FString AssetName{ FString(TEXT("")) };
		FString AssetPath{FString(TEXT(""))};

	};

	static TObjectPtr<UObject> CreateAsset(const FAssetCreationData& AssetCreationData, UPackage*& OutPackage)
	{
#if WITH_EDITOR
		if (!AssetCreationData.AssetClass || !AssetCreationData.Factory)
		{
			UE_LOG(LogQbaAutomation, Error, TEXT("Provided null class or factory for asset creation, aborting asset creation."));
			return nullptr;
		}
		const FName UniquePackageName = MakeUniqueObjectName(OutPackage, UPackage::StaticClass(), FName(*AssetCreationData.AssetName));
		const FString FullPackageName = AssetCreationData.AssetPath + TEXT("/") + UniquePackageName.ToString();

		OutPackage = CreatePackage(*FullPackageName);
		EObjectFlags Flags = RF_Public | RF_Standalone;
		
		TObjectPtr<UObject> CreatedAsset = AssetCreationData.Factory->FactoryCreateNew(AssetCreationData.AssetClass, OutPackage, FName(*AssetCreationData.AssetName), Flags, NULL, GWarn);

		if (CreatedAsset)
		{
			FAssetRegistryModule::AssetCreated(CreatedAsset);
			OutPackage->MarkPackageDirty();
			UE_LOG(LogQbaAutomation, VeryVerbose, TEXT("Asset %s correctly created"), *AssetCreationData.AssetName);
			return CreatedAsset;
		}
		else
		{
			UE_LOG(LogQbaAutomation, Error, TEXT("Could not create %s asset"), *AssetCreationData.AssetClass->GetName());
		}
#else
		UE_LOG(LogQbaAutomation, Error, TEXT("Cannot create new assets outside of editor"));
#endif // WITH_EDITOR
		return nullptr;
	};

	/* Factory isn't necessary when calling CreateBlueprint function */
	static UBlueprint* CreateBlueprint(const FAssetCreationData& AssetCreationData, UPackage*& OutPackage)
	{
#if WITH_EDITOR
		if (!AssetCreationData.AssetClass)
		{
			UE_LOG(LogQbaAutomation, Error, TEXT("Provided null class for blueprint creation, aborting blueprint creation."));
		}
		const FName UniquePackageName = MakeUniqueObjectName(OutPackage, UPackage::StaticClass(), FName(*AssetCreationData.AssetName));
		const FString FullPackageName = AssetCreationData.AssetPath + TEXT("/") + UniquePackageName.ToString();

		OutPackage = CreatePackage(*FullPackageName);

		if (OutPackage)
		{
			const FName BlueprintName = MakeUniqueObjectName(OutPackage, AssetCreationData.AssetClass, FName(*AssetCreationData.AssetName));
			
			UObject* ExistingBlueprint = FindObject<UBlueprint>(OutPackage, *AssetCreationData.AssetName);
		
			if (ExistingBlueprint)
			{
				UE_LOG(LogQbaAutomation, Error, TEXT("Cannot create new blueprint, old one still exists"), ExistingBlueprint);
				return false;
			}

			UBlueprint* CreatedBlueprint = FKismetEditorUtilities::CreateBlueprint(AssetCreationData.AssetClass, OutPackage, BlueprintName, BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass());
			if (CreatedBlueprint)
			{
				UE_LOG(LogQbaAutomation, VeryVerbose, TEXT("Succesully created new blueprint"));
				CreatedBlueprint->SetFlags(RF_Public | RF_Standalone);
				FAssetRegistryModule::AssetCreated(CreatedBlueprint);
				OutPackage->MarkPackageDirty();
				return CreatedBlueprint;
			}
			else
			{
				UE_LOG(LogQbaAutomation, Error, TEXT("Failed to create new blueprint"));
				return nullptr;
			}
		}
#else
		UE_LOG(LogQbaAutomation, Error, TEXT("Cannot create new assets outside of editor"));
#endif // WITH_EDITOR
		return nullptr;
	}

	static bool SaveAsset(TObjectPtr<UObject> AssetToSave) 
	{ 
#if WITH_EDITOR
		if (AssetToSave)
		{
			UPackage* Package = AssetToSave->GetPackage();

			Package->SetDirtyFlag(true);
			Package->FullyLoad();

			if (Package)
			{
				const FString PackageName = Package->GetName();
				const FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());

				FSavePackageArgs SaveArgs;
				{
					SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
					SaveArgs.Error = GWarn;
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
			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->CloseAllAssetEditors();

			UPackage* Package = AssetToDelete->GetPackage();

			FAutomationEditorCommonUtils::NullReferencesToObject(AssetToDelete);
			ObjectTools::DeleteSingleObject(AssetToDelete, false);
			
			FString PackageFilename;
			if (FPackageName::DoesPackageExist(Package->GetName(), &PackageFilename))
			{
				TArray<UPackage*> PackagesToDelete;
				PackagesToDelete.Add(Package);

				Package->SetDirtyFlag(false);
				UPackageTools::UnloadPackages(PackagesToDelete);

				ObjectTools::DeleteSingleObject(Package, false);
				IFileManager::Get().Delete(*PackageFilename);
				UE_LOG(LogQbaAutomation, VeryVerbose, TEXT("Succesfully deleted asset"));
				CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);
				return true;
			}
		}
#else
		UE_LOG(LogQbaAutomation, Error, TEXT("Cannot delete assets in non editor context!"));
#endif // WITH_EDITOR
		return false;
	};

	static FString DefaultAssetPath{ FString(TEXT("/Game/QbaAutomation")) };
	static FString GetPathForAsset(const FString& TestName, const FString& TargetPath = DefaultAssetPath) { return FString::Printf(TEXT("%s/%s_Test"), *TargetPath, *TestName); };

	/* Deletes directory and all assets*/
	static bool DeleteTestDirectory(const FString& Directory = DefaultAssetPath)
	{
#if WITH_EDITOR

		bool bDirectoryWasRemoved{ false };
		FString PathToDeleteOnDisk{ FString(TEXT("")) };

		if (FPackageName::TryConvertLongPackageNameToFilename(Directory, PathToDeleteOnDisk))
		{
			bDirectoryWasRemoved = IFileManager::Get().DeleteDirectory(*PathToDeleteOnDisk, false, true);
		}

		else
		{
			UE_LOG(LogQbaAutomation, Error, TEXT("Folder deletion failed. Could not convert the path. %s"), *Directory);
		}

		if (bDirectoryWasRemoved)
		{
			FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
			AssetRegistryModule.Get().RemovePath(Directory);
			return true;
		}
#else
		UE_LOG(LogQbaAutomation, Error, TEXT("Cannot delete directory in non editor context!"));
#endif // WITH_EDITOR
		return false;
	};
}


struct QBATESTS_API FQbaTestBase
{
	FAutomationTestBase* TestRunner{nullptr};
	FORCEINLINE FAutomationTestBase& GetTestRunner() const { check(TestRunner); return *TestRunner; }

	FORCEINLINE const FString& GetAssetSaveLocation() const { return AssetSaveLocation; }
	void SetAssetSaveLocation() { AssetSaveLocation = FQbaTestHelpers::GetPathForAsset(GetTestRunner().GetTestName()); };

	FString AssetSaveLocation{ FString(TEXT("")) };
	
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
		}

		FQbaTestHelpers::DeleteTestDirectory();
		return bSuccessDeleteAssets;
	};


public:
	FORCEINLINE virtual void SetTestRunner(FAutomationTestBase& AutomationTestInstance) { TestRunner = &AutomationTestInstance; }

	//Interface
	virtual ~FQbaTestBase() {};

	// Check if initialized properly, loading environment, required assets etc.
	virtual bool PrepareTest() 
	{
		UE_LOG(LogQbaAutomation, Log, TEXT("Preparing the test..."));
		FQbaTestHelpers::DeleteTestDirectory();
		return true;
	};

	// Run logic of your test here 
	virtual bool RunTestLogic() 
	{
		UE_LOG(LogQbaAutomation, Log, TEXT("Running test logic..."));
		return true;
	};

	// Delete all necessary assets, finish test logic
	virtual bool FinishTest() 
	{
		UE_LOG(LogQbaAutomation, Log, TEXT("Finishing the test..."));
		const bool bDeletedAllAssets = DeleteAllTestAssets();
		GetTestRunner().TestTrue(FString(TEXT("DeleteAllTestAssets")), bDeletedAllAssets);
		return true;
	};


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
	TestInstance->SetAssetSaveLocation(); \
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
		TestInstance->SetAssetSaveLocation(); \
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

PRAGMA_ENABLE_OPTIMIZATION