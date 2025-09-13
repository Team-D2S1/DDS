// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GenericTeamAgentInterface.h"
#include "DDSTypes/DDSEnumTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DDSFunctionLibrary.generated.h"


class UEquipWidgetController;
class UDDSWidgetController;
class UItemInstance;
class ADDSHUD;
class UPawnCombatComponent;
/**
 * DDS 프로젝트에서 쓰이는 static 헬퍼 함수들.
 * 블루프린트에서도 부를 수 있음
 */
UCLASS()
class DDS_API UDDSFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	
public:

	/*
	 * 위젯 컨트롤러 관련
	 */
	UFUNCTION(BlueprintPure, Category = "DDS|FunctionLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutParams, const ADDSHUD* OutHUD);
	
	UFUNCTION(BlueprintPure, Category = "DDS|FunctionLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UDDSWidgetController* GetDefualtWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "DDS|FunctionLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UDDSWidgetController* GetMainMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "DDS|FunctionLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UEquipWidgetController* GetEquipWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "DDS|FunctionLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UDDSWidgetController* GetInventoryWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "DDS|FunctionLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UCraftingWidgetController* GetCraftingWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "DDS|FunctionLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UDDSWidgetController* GetWeaponCraftWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "DDS|FunctionLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UDDSWidgetController* GetSystemWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "DDS|FunctionLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UDDSWidgetController* GetStatusWidgetController(const UObject* WorldContextObject);

	/*
	 *  인게임 로직
	 */

	static UPawnCombatComponent* NativeGetCombatComponent(AActor* InActor);
	
	UFUNCTION(BlueprintCallable, Category = "DDS|FunctionLibrary", meta = (DisplayName = "Get Pawn Combat Component From Actor", ExpandEnumAsExecs = "OutValid"))
	static UPawnCombatComponent* BP_GetCombatComponent(AActor* InActor, EDDSValidType& OutValid);

	static bool NativeDoesActorHasTag(AActor* InActor, FGameplayTag InTag);

	UFUNCTION(BlueprintCallable, Category = "DDS|FunctionLibrary")
	static void BP_DoesActorHasTag(AActor* InActor, FGameplayTag InTag, EDDSConfirmType& OutConfirm);

	/**
	 * 패링의 방향만 확인한다.
	 * @param InAttacker 
	 * @param InDefender 
	 * @return 유효한 패링이면 true
	 */
	UFUNCTION(BlueprintPure, Category = "DDS|FunctionLibrary")	
	static bool IsValidParry(AActor* InAttacker, AActor* InDefender);
	
	/// <summary>
	/// QueryPawn이 TargetPawn을 적으로 인식하는지 여부를 반환
	/// 주의! 서버에서만 됨(플레이어 컨트롤러는 서버에서만 생성됨)
	/// </summary>
	/// <param name="QueryPawn"></param>
	/// <param name="TargetPawn"></param>
	/// <returns>적이면 true</returns>
	UFUNCTION(BlueprintPure, Category = "DDS|FunctionLibrary")
	static bool IsTargetHostile(APawn* QueryPawn, APawn* TargetPawn);

	static IGenericTeamAgentInterface* GetTeamAgentInterface(APawn* InActor);
	static FGenericTeamId NativeGetTeamId(APawn* InActor);
	
	
	UFUNCTION(BlueprintPure, Category = "DDS|FunctionLibrary")
	static FGenericTeamId BP_GetTeamId(APawn* InActor);

	/*
	 * 기타 헬퍼
	 */
	UFUNCTION(BlueprintCallable, Category = "DDS|FunctionLibrary")
	static EPartType GetPartType(UItemInstance* ItemInstance);

	UFUNCTION(BlueprintPure, Category = "DDS|FunctionLibrary")
	static EPartType GetPartTypeByTag(FGameplayTag TypeTag);

};
