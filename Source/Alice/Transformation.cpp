// Fill out your copyright notice in the Description page of Project Settings.

#include "Alice.h"
#include "Transformation.h"
#include "PlayerCharacter.h"


void FTransformation::Setup(ETransformationID _id) {
	id = _id;

	switch (_id){
		case ETransformationID::ALICE:
			usageRate = 0.f;
			rechargeRate = 10.f;
			break;
		case ETransformationID::BAT:
			usageRate = 10.f;
			rechargeRate = 0.f;
			isConcealed = false;
			break;
		case ETransformationID::TREANT:
			usageRate = 5.f;
			rechargeRate = 0.f;
			break;
		case ETransformationID::WIZARD:
			usageRate = 10.f;
			rechargeRate = 0.f;
			break;
		default:
			break;
	}
}

void FTransformation::Update(float DeltaSeconds, APlayerCharacter& playerRef) {
	switch (id) {
		case ETransformationID::ALICE:
			break;
		case ETransformationID::BAT:
			Bat_Update(DeltaSeconds, playerRef);
			break;
		case ETransformationID::TREANT:
			Treant_Update(DeltaSeconds, playerRef);
			break;
		case ETransformationID::WIZARD:
			break;
		default:
			break;
	}
}

void FTransformation::PrimaryAbility(APlayerCharacter& playerRef) {
	switch (id) {
		case ETransformationID::ALICE:
			break;
		case ETransformationID::BAT:
			Bat_PrimaryAbility(playerRef);
			break;
		case ETransformationID::TREANT:
			Treant_PrimaryAbility(playerRef);
			break;
		case ETransformationID::WIZARD:
			break;
		default:
			break;
	}
}

void FTransformation::SecondaryAbility(APlayerCharacter& playerRef) {
	switch (id) {
		case ETransformationID::ALICE:
			break;
		case ETransformationID::BAT:
			Bat_SecondaryAbility(playerRef);
			break;
		case ETransformationID::TREANT:
		case ETransformationID::WIZARD:
		default:
			break;
	}
}


/* Bat */
void FTransformation::Bat_PrimaryAbility(APlayerCharacter& playerRef) {
	// Is the ability active already?
	if (!isConcealed) {
		isConcealed = true;
		remainingConcealmentDuration = baseConcealmentDuration;
		playerRef.SwapBatMaterial(isConcealed);
	}
}

void FTransformation::Bat_SecondaryAbility(APlayerCharacter& playerRef) {
	if (playerRef.playerAction == EPlayerActions::NONE) {
		playerRef.Echolocation_Delegate.Broadcast();
		playerRef.ActivateEchoSystem();
	}
}

void FTransformation::Bat_Update(float DeltaSeconds, APlayerCharacter& playerRef) {
	// Check the primary ability
	if (isConcealed) {
		remainingConcealmentDuration -= DeltaSeconds;
		if (remainingConcealmentDuration <= 0) {
			isConcealed = false;
			remainingConcealmentDuration = 0;
			playerRef.SwapBatMaterial(isConcealed);
		}
	}
}

void FTransformation::RemoveBatInvisibility(APlayerCharacter& playerRef) {
	if (isConcealed) {
		isConcealed = false;
		remainingConcealmentDuration = 0;
		playerRef.SwapBatMaterial(isConcealed);
	}
}

/* Treant */
void FTransformation::Treant_PrimaryAbility(APlayerCharacter& playerRef) {
	// If it wasn't active!
	playerRef.Treant_PrimaryAbility(false);
	/*if (!physicsHandleActive) {		
		FVector Start;
		FVector End;

		Start = playerRef.CapsuleComponent->GetComponentLocation() - FVector(0, 0, 50);
		End = playerRef.CapsuleComponent->GetForwardVector() * pickupDistance + playerRef.CapsuleComponent->GetComponentLocation() - FVector(0, 0, 50);

		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Trace")));
		TraceParams.AddIgnoredActor(&playerRef);

		FHitResult Hit(ForceInit);

		bool hitSomething = playerRef.GetWorld()->LineTraceSingle(Hit, Start, End, TraceParams, ECC_PhysicsBody);

		if (hitSomething) {
			physicsHandleActive = true;

			otherItemLocation = (playerRef.GetActorLocation() - Hit.Location).Size();
			otherItemRotation = playerRef.GetActorRotation();
			
			//playerRef.physicsHandle->SetActive(true);
			physicsObject = Hit.GetComponent();
			playerRef.Treant_PrimaryAbility(true, Hit);
			//playerRef.physicsHandle->GrabComponent(Hit.GetComponent(), Hit.BoneName, Hit.Location, true);
			//Hit.GetComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		}
	} 
	else {
		physicsHandleActive = false;

		playerRef.Treant_PrimaryAbility(false, FHitResult(ForceInit));
		//playerRef.physicsHandle->ReleaseComponent();
	}*/
}

void FTransformation::Treant_Update(float DeltaSeconds, APlayerCharacter& playerRef) {
	playerRef.Treant_Update();
	/*if (physicsHandleActive) {
		handleLocation = (playerRef.GetActorLocation() + (playerRef.GetActorForwardVector() * otherItemLocation)) + FVector(0, 0, 50);
		playerRef.physicsHandle->SetTargetLocationAndRotation(handleLocation, playerRef.GetControlRotation());
	}*/
}
