#include "BlueprintOrderFix.h"

#include "FGBlueprintSubsystem.h"
#include "FGFactoryBlueprintTypes.h"
#include "Algo/StableSort.h"
#include "Patching/NativeHookManager.h"
#include "UObject/UnrealType.h"

DEFINE_LOG_CATEGORY_STATIC(LogBlueprintOrderFix, Log, All);

namespace
{
	const TArray<FBlueprintCategoryRecord>* GetRecords(AFGBlueprintSubsystem* Subsystem)
	{
		const FArrayProperty* Prop = CastField<FArrayProperty>(
			AFGBlueprintSubsystem::StaticClass()->FindPropertyByName(TEXT("mBlueprintCategoryRecords")));
		return Prop ? Prop->ContainerPtrToValuePtr<TArray<FBlueprintCategoryRecord>>(Subsystem) : nullptr;
	}

	/** Reorders each live sub-category's descriptors to match the saved BlueprintNames order. Idempotent. */
	void ApplyOrder(AFGBlueprintSubsystem* Subsystem, const TCHAR* Where)
	{
		const TArray<FBlueprintCategoryRecord>* Records = Subsystem ? GetRecords(Subsystem) : nullptr;
		if (!Records)
		{
			UE_LOG(LogBlueprintOrderFix, Warning, TEXT("[%s] mBlueprintCategoryRecords not found, blueprint order not applied"), Where);
			return;
		}

		TArray<UFGBlueprintCategory*> Categories;
		Subsystem->GetAllBlueprintCategories(Categories);

		int32 Reordered = 0;
		for (const FBlueprintCategoryRecord& CatRec : *Records)
		{
			for (UFGBlueprintCategory* Cat : Categories)
			{
				if (!Cat || Cat->GetCategoryNameFromInstanceAsString() != CatRec.CategoryName)
				{
					continue;
				}
				TArray<UFGBlueprintSubCategory*> Subs;
				Cat->GetSubCategories(Subs);
				for (const FBlueprintSubCategoryRecord& SubRec : CatRec.SubCategoryRecords)
				{
					for (UFGBlueprintSubCategory* Sub : Subs)
					{
						if (!Sub || Sub->GetCategoryNameFromInstanceAsString() != SubRec.SubCategoryName
							|| !FMath::IsNearlyEqual(Sub->GetMenuPriorityFromInstance(), SubRec.MenuPriority))
						{
							continue;
						}
						auto IndexOf = [&SubRec](const UFGBlueprintDescriptor* Desc)
						{
							const int32 Idx = Desc ? SubRec.BlueprintNames.IndexOfByPredicate([Desc](const FString& N)
								{ return N.Equals(Desc->GetBlueprintNameAsString(), ESearchCase::IgnoreCase); }) : INDEX_NONE;
							return Idx == INDEX_NONE ? MAX_int32 : Idx;
						};
						Algo::StableSortBy(Sub->mBlueprintDescriptors, [&](const TObjectPtr<UFGBlueprintDescriptor>& D) { return IndexOf(D); });
						// The menu may sort by descriptor priority, so keep it in step with the order.
						for (int32 i = 0; i < Sub->mBlueprintDescriptors.Num(); ++i)
						{
							if (UFGBlueprintDescriptor* D = Sub->mBlueprintDescriptors[i])
							{
								D->SetPriorityOnInstance(static_cast<float>(i));
							}
						}
						++Reordered;
					}
				}
			}
		}
		if (FCString::Strcmp(Where, TEXT("BeginPlay")) == 0)
		{
			UE_LOG(LogBlueprintOrderFix, Log, TEXT("Applied saved blueprint order to %d sub-categories"), Reordered);
		}
	}
}

void FBlueprintOrderFixModule::StartupModule()
{
#if !WITH_EDITOR
	SUBSCRIBE_METHOD_AFTER(AFGBlueprintSubsystem::Init,
		[](AFGBlueprintSubsystem* Self) { ApplyOrder(Self, TEXT("Init")); });

	SUBSCRIBE_METHOD_AFTER(AFGBlueprintSubsystem::RefreshBlueprintsAndDescriptors,
		[](AFGBlueprintSubsystem* Self) { ApplyOrder(Self, TEXT("Refresh")); });

	AFGBlueprintSubsystem* Sample = GetMutableDefault<AFGBlueprintSubsystem>();
	SUBSCRIBE_METHOD_VIRTUAL_AFTER(AFGBlueprintSubsystem::BeginPlay, Sample,
		[](AFGBlueprintSubsystem* Self) { ApplyOrder(Self, TEXT("BeginPlay")); });
#endif
}

void FBlueprintOrderFixModule::ShutdownModule()
{
}

IMPLEMENT_MODULE(FBlueprintOrderFixModule, BlueprintOrderFix)
