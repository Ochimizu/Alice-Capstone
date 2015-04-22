// Fill out your copyright notice in the Description page of Project Settings.

#include "Alice.h"
#include "PlayerCharacter.h"
#include "Alice_AnimInstance.h"
#include "HiddenObject.h"
#include "HiddenItem.h"

const int HEALTH_LOW_VALUE = 175;

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer) 
{
	// Set player action
	playerAction = EPlayerActions::NONE;

	// Set the player to alive
	isAlive = true;
	itemsInRange = 0;
	dialogueActive = false;
	collectShape = false;

	GetCapsuleComponent()->InitCapsuleSize(42, 96);
	baseTurnRate = 45;

	// Don't rotate when the controller rotates
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Set up the player movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 540, 0);
	GetCharacterMovement()->JumpZVelocity = 600;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create the camera boom
	cameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("Camera Boom"));
	cameraBoom->AttachTo(RootComponent);
	cameraBoom->TargetArmLength = 300;
	cameraBoom->bUsePawnControlRotation = true;

	// Create the camera
	followCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Follow Camera"));
	followCamera->AttachTo(cameraBoom, USpringArmComponent::SocketName);
	followCamera->bUsePawnControlRotation = false;
	
	// Set the settings
	normalSettings = followCamera->PostProcessSettings;
	hitSettings.VignetteIntensity = 0.4f;
	hitSettings.VignetteColor = FColor::Red;

	// Create the particle system
	shapeShiftSystem = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("Transformation System"));
	shapeShiftSystem->AttachTo(RootComponent);
}

/* Necessary Methods */

void APlayerCharacter::BeginPlay() {
	PopulateTransformations();
	// Call set up on each
	for (int i = 0; i < (int)ETransformationID::WIZARD; i++) {
		transformationList[i].Setup((ETransformationID)i);
	}

	currentTransformation = ETransformationID::ALICE;

	AddStatusHUD();

	inventory.LoadKeyItems();
}

void APlayerCharacter::Tick(float DeltaSeconds) {
	speed = GetVelocity().Size();

	// Handle the Transformation
	int id = (int)currentTransformation.GetValue();

	// Should it be recharged?
	if (info.currentMP < info.BASE_MAX_MP)
		info.currentMP += transformationList[id].rechargeRate * DeltaSeconds;
	
	// If player is transformed, apply usage rate
	if (currentTransformation != ETransformationID::ALICE) {
		if (info.currentMP > 0)
			info.currentMP -= transformationList[id].usageRate * DeltaSeconds;
		else
			ShapeShift(ETransformationID::ALICE);
	}

	// Hide the hit overlay
	if (hitStyleActive && info.currentHP > HEALTH_LOW_VALUE) {
		ChangePostProcessSettings(false);
	}

	// Update the transformations
	transformationList[id].Update(DeltaSeconds, *this);
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
	InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::ActionButton);
	InputComponent->BindAction("TransformationMenu", IE_Pressed, this, &APlayerCharacter::TransformationMenuInput);
	InputComponent->BindAction("TransformationMenu", IE_Released, this, &APlayerCharacter::HideTransformationMenu);
	InputComponent->BindAction("InventoryMenu", IE_Pressed, this, &APlayerCharacter::InventoryMenuInput);
	InputComponent->BindAction("InventoryMenu", IE_Released, this, &APlayerCharacter::HideInventoryMenu);

	InputComponent->BindAction("PrimaryAbility", IE_Pressed, this, &APlayerCharacter::PrimaryAbility);
	InputComponent->BindAction("SecondaryAbility", IE_Pressed, this, &APlayerCharacter::SecondaryAbility);

	InputComponent->BindAxis("DPadMenuY", this, &APlayerCharacter::HandleDPadMenuY);
	InputComponent->BindAxis("DPadMenuX", this, &APlayerCharacter::HandleDPadMenuX);

	InputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	InputComponent->BindAxis("MoveSideways", this, &APlayerCharacter::MoveSideways);

	// Turn rate is for joystick
	// Turn is for mouse
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);
}


/* Inventory Methods */
/* NOTE: These really shouldn't be here, Inventory should handle all of it's stuff */

int32 APlayerCharacter::GetInventoryItemCount(EItemType t) {
	return inventory.items[(int)t].inventoryCount;
}

void APlayerCharacter::SetInventoryItemCount(EItemType t, int32 count) {
	if (count >= 0)
		inventory.items[(int)t].inventoryCount = count;
}

void APlayerCharacter::AddItemToInventory(EItemType t) {
	inventory.AddItem(t);
}

FKeyItem APlayerCharacter::GetKeyItem(EItemType t) {
	return inventory.GetKeyItem(t);
}

void APlayerCharacter::UseItem(EItemType id) {
	if (inventory.UseItem(id, info)) {
		// TODO: Let player know it didn't work
		// Play a sound? Anything.
	}
	else {
		displayInventoryMenu = false;
	}
}

void APlayerCharacter::SetHP(float hp) {
	info.currentHP = hp;
}

void APlayerCharacter::SetMP(float mp) {
	info.currentMP = mp;
}


/* Movement Methods */

void APlayerCharacter::MoveForward(float value) {
	if (dialogueActive)
		return;

	if (Controller != nullptr && value != 0 && playerAction == EPlayerActions::NONE) {
		// Find forward
		FRotator rotation = Controller->GetControlRotation();
		FRotator yawRotation(0, rotation.Yaw, 0);

		// Get the forward vector
		FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(direction, value);
	}
}

void APlayerCharacter::MoveSideways(float value) {
	if (dialogueActive)
		return;

	if (Controller != nullptr && value != 0 && playerAction == EPlayerActions::NONE) {
		// Find which way is right
		FRotator rotation = Controller->GetControlRotation();
		FRotator yawRotation(0, rotation.Yaw, 0);

		// Get right vector
		FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(direction, value);
	}
}

void APlayerCharacter::TurnAtRate(float rate) {
	if (dialogueActive)
		return;
	AddControllerYawInput(rate * baseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float rate) {
	if (dialogueActive)
		return;
	AddControllerPitchInput(rate * baseTurnRate * GetWorld()->GetDeltaSeconds());
}


/* Input Actions */

void APlayerCharacter::HandleDPadMenuY(float value) {
	// Inventory Menu
	if (displayInventoryMenu) {
		// Up
		if (value > 0.9f) {
			UseItem(EItemType::HP_RESTORATIVE);
		}
		// Down
		else if (value < -0.9f) {
			UseItem(EItemType::CURSE_CURE);
		}

	}
	// Transformation Menu
	else if (displayTransformationMenu) {
		// Up - Alice
		if (value > 0.5f) {
			ShapeShift(ETransformationID::ALICE);
		}
		// Down - Blade Girl for Testing
		else if (value < -0.5f) {
			ShapeShift(ETransformationID::BAT);
		}
	}
}

void APlayerCharacter::HandleDPadMenuX(float value) {
	// Inventory Menu
	if (displayInventoryMenu) {
		// Right
		if (value > 0.9f) {
			UseItem(EItemType::BLIND_CURE);
		}
		// Left
		else if (value < -0.9f) {
			UseItem(EItemType::MP_RESTORATIVE);
		}
	}
	// Transformation Menu
	else if (displayTransformationMenu) {
		// Right
		if (value > 0.5f) {
			ShapeShift(ETransformationID::WIZARD);
		}
		// Left
		else if (value < -0.5f) {
			ShapeShift(ETransformationID::TREANT);
		}
	}
}

void APlayerCharacter::ActionButton() {
	if (dialogueActive) {
		ProgressDialogue_Delegate.Broadcast();
		return;
	}

	if (itemsInRange > 0) {
		// Broadcast that they wanted to pick up an item?
		PickedUpItem_Delegate.Broadcast();
	}
	else if (collectShape) {
		int id = (int)(collectibleShape.GetValue());
		if (transformationList.IsValidIndex(id)) {
			transformationList[id].unlocked = true;
			CollectShape_Delegate.Broadcast();
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Collectible Shape has been Unlocked"));
			collectShape = false;
		}
	}
	else {
		ACharacter::Jump();
	}

}

void APlayerCharacter::TransformationMenuInput() {
	if (!displayInventoryMenu && !displayTransformationMenu) {
		displayTransformationMenu = true;
	}
}

void APlayerCharacter::HideTransformationMenu() {
	displayTransformationMenu = false;
}

void APlayerCharacter::InventoryMenuInput() {
	if (!displayTransformationMenu && !displayInventoryMenu) {
		displayInventoryMenu = true;
	}
}

void APlayerCharacter::HideInventoryMenu() {
	displayInventoryMenu = false;
}


/* Shapeshifting Methods */

void APlayerCharacter::ShapeShift(ETransformationID id) {
	if (currentTransformation != id) {
		int idNum = (int)id;
		if (transformationList.IsValidIndex(idNum) && transformationList[idNum].unlocked) {
			// REMOVE THE BAT'S INVISIBILITY
			if (currentTransformation == ETransformationID::BAT) {
				transformationList[(int)currentTransformation.GetValue()].RemoveBatInvisibility(*this);
			}

			// Drop the Treant's item
			if (currentTransformation == ETransformationID::TREANT) {
				Treant_PrimaryAbility(true);
			}

			// Reset it back to normal
			playerAction = EPlayerActions::NONE;

			shapeShiftSystem->ActivateSystem();

			currentTransformation = id;
			GetMesh()->SetSkeletalMesh(transformationList[idNum].mesh);
			//ChangeMesh(transformationList[idNum]);

			// Sets the Anim Instance in blueprints
			ChangeAnimInstance();

			displayTransformationMenu = false;
		}
	}
}

void APlayerCharacter::ChangeMesh(FTransformation shape) {
	GetMesh()->SetSkeletalMesh(shape.mesh);
}

void APlayerCharacter::PrimaryAbility() {
	int id = (int)currentTransformation.GetValue();
	transformationList[id].PrimaryAbility(*this);
}

void APlayerCharacter::SecondaryAbility() {
	int id = (int)currentTransformation.GetValue();
	transformationList[id].SecondaryAbility(*this);
}


/* Misc. Methods */

void APlayerCharacter::ApplyDamage(float dmg) {
	float tmpHP = info.currentHP - dmg;
	if (tmpHP < 0) {
		info.currentHP = 0;
	}
	else {
		info.currentHP = tmpHP;

		// Display the hit overlay
		if (info.currentHP <= HEALTH_LOW_VALUE) {
			ChangePostProcessSettings(true);
		}
	}
}

void APlayerCharacter::ChangePostProcessSettings(bool healthLow) {
	if (healthLow && !hitStyleActive) {
		followCamera->PostProcessSettings = hitSettings;
		hitStyleActive = true;
	}
	else if (!healthLow) {
		followCamera->PostProcessSettings = normalSettings;
		hitStyleActive = false;
	}
}


/* Implementations for Blueprint Native Events */
void APlayerCharacter::Treant_Update_Implementation() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Implementation of Treant_Update() was called from code"));
}

void APlayerCharacter::Treant_PrimaryAbility_Implementation(bool overrideDrop) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Implementation of PrimaryAbility() was called from code"));
}

void APlayerCharacter::AddStatusHUD_Implementation() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Implementation of AddStatusHUD() was called from code"));
}

void APlayerCharacter::ChangeAnimInstance_Implementation() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Implmentation of ChangeAnimInstance() was called from code"));
}

void APlayerCharacter::PopulateTransformations_Implementation() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Implementation of PopulateTransformations() was called from code"));
}

void APlayerCharacter::SwapBatMaterial_Implementation(bool isConcealed) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Implementation of SwapBatMaterial() was called from code"));
}

void APlayerCharacter::ActivateEchoSystem_Implementation() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Implementation of ActivateEchoSystem() was called from code"));
}