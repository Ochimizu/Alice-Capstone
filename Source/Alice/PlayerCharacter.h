// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Containers/UnrealString.h"
#include "Inventory.h"
#include "Transformation.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBindableEvent_PickupItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBindableEvent_Echolocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBindableEvent_ProgressDialogue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBindableEvent_CollectShape);

UENUM(BlueprintType)
enum class EPlayerActions {
	NONE,
	JUMP,
	TAKE_DMG,
	TREANT_ABILITY
};


UENUM(BlueprintType)
enum class EStatusEffect {
	BLIND,
	CURSE,
	NONE
};

/* NOTE: Move to it's own file */
/* Container for all the player's necessary information */
USTRUCT(BlueprintType)
struct FPlayerInformation {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerInfo")
		FString name;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerInfo")
		float currentHP;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerInfo")
		float currentMP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerInfo")
		float BASE_MAX_HP = 500.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerInfo")
		float BASE_MAX_MP = 250.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerInfo")
		TEnumAsByte<EStatusEffect> currentStatus;

	FPlayerInformation() {
		Setup();
	}

	void Setup() {
		name = "Alice";
		currentHP = BASE_MAX_HP;
		currentMP = BASE_MAX_MP;

		currentStatus = EStatusEffect::NONE;
	}

};

/* The Player's main class */
UCLASS()
class ALICE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	float baseTurnRate;

	/* Post Processor Settings */
	FPostProcessSettings hitSettings;

	/* Post Processor Settings */
	FPostProcessSettings normalSettings;

	/* Refers to HitSettings */
	bool hitStyleActive;

public:
	/* Player Components */

	/* The arm that holds the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		USpringArmComponent* cameraBoom;

	/* Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		UCameraComponent* followCamera;


	/* Player Information*/

	/* Information on the player's character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
		FPlayerInformation info;

	/* This controls the Idle-Walk-Run blendspace */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerInfo")
		float speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerInfo")
		bool isAlive;

	/* Contains the current action the player is trying to perform */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
		TEnumAsByte<EPlayerActions> playerAction;


	/* HUD & Dialogue */

	/* Controls the visibility of the transformation menu */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
		bool displayTransformationMenu;

	/* Handles the visibility of the inventory menu */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
		bool displayInventoryMenu;

	/* Is Alice currently running through dialogue? */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dialogue")
		bool dialogueActive;
	

	/* Inventory */
	
	/* The inventory for the player */
	Inventory inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		int32 itemsInRange;


	/* ShapeShift */

	/* The particle system */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShapeShift")
		UParticleSystemComponent* shapeShiftSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShapeShift")
		USoundWave* shapeShiftSoundFX;

	/* List of transformation data */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShapeShift")
		TArray<FTransformation> transformationList;

	/* Current transformation for the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShapeShift")
		TEnumAsByte<ETransformationID> currentTransformation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShapeShift")
		bool collectShape;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShapeShift")
		TEnumAsByte<ETransformationID> collectibleShape;


	// Methods
	/* Constructor */
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);
	
	void ChangePostProcessSettings(bool healthLow);

	/* Blueprint Callable Methods */

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void AddItemToInventory(EItemType t);
	
	UFUNCTION(BlueprintCallable, Category = "PlayerInfo")
		int32 GetInventoryItemCount(EItemType t);

	UFUNCTION(BlueprintCallable, Category = "LoadGame")
		void SetInventoryItemCount(EItemType t, int32 count);

	UFUNCTION(BlueprintCallable, Category = "PlayerInfo")
		void ApplyDamage(float dmg);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		FKeyItem GetKeyItem(EItemType t);

	UFUNCTION(BlueprintCallable, Category = "PlayerInfo")
		void SetHP(float hp);

	UFUNCTION(BlueprintCallable, Category = "PlayerInfo")
		void SetMP(float mp);


	/* Blueprint Native Events */ 

	UFUNCTION(BlueprintNativeEvent, Category = "TreantTransformation")
		void Treant_Update();

	UFUNCTION(BlueprintNativeEvent, Category = "TreantTransformation")
		void Treant_PrimaryAbility(bool overrideDrop);

	UFUNCTION(BlueprintNativeEvent, Category = "ShapeShift")
		void PopulateTransformations();

	UFUNCTION(BlueprintNativeEvent, Category = "ShapeShift")
		void ChangeAnimInstance();

	UFUNCTION(BlueprintNativeEvent, Category = "HUD")
		void AddStatusHUD();

	UFUNCTION(BlueprintNativeEvent, Category = "ShapeShift")
		void SwapBatMaterial(bool isConcealed);

	UFUNCTION(BlueprintNativeEvent, Category = "BatTransformation")
		void ActivateEchoSystem();


	/* Event Dispatchers */ 

	UPROPERTY(BlueprintAssignable)
		FBindableEvent_PickupItem PickedUpItem_Delegate;

	UPROPERTY(BlueprintAssignable)
		FBindableEvent_Echolocation Echolocation_Delegate;

	UPROPERTY(BlueprintAssignable)
		FBindableEvent_ProgressDialogue ProgressDialogue_Delegate;

	UPROPERTY(BlueprintAssignable)
		FBindableEvent_CollectShape CollectShape_Delegate;

protected:

	/* Movement Methods */
	void MoveForward(float value);

	void MoveSideways(float value);

	void TurnAtRate(float rate);

	void LookUpAtRate(float rate);


	/* Misc. Methods */

	void ChangeMesh(FTransformation shape);

	void UseItem(EItemType id);


	/* Necessary Methods */

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void BeginPlay() override;


	/* Input Actions */

	/* Handles DPad Y-input for Inventory/Shapeshift menus */
	void HandleDPadMenuY(float value);

	/* Handles DPad X-input for Inventory/Shapeshift menus */
	void HandleDPadMenuX(float value);

	/* This if for the Action button, which would be A on the gamepad */
	void ActionButton();

	/* Handles the input for the inventory menu when displayed */
	void InventoryMenuInput();

	/* Will hide the inventory menu */
	void HideInventoryMenu();

	/* Handles the input for the transformation menu when displayed */
	void TransformationMenuInput();

	/* Will hide the transformation menu */
	void HideTransformationMenu();

	/* Shapeshift Methods */

	/* Change the Player's Shape depending on the incoming ID */
	void ShapeShift(ETransformationID id);

	/* Responsible for determining which primary ability is to be used based on the transformation */
	void PrimaryAbility();

	/* Responsible for determining which secondary ability is to be used based on the transformation */
	void SecondaryAbility();
};
