// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Transformation.generated.h"
/**
 * 
 */

class APlayerCharacter;

UENUM(BlueprintType)
enum class ETransformationID {
	ALICE,
	BAT,
	TREANT,
	WIZARD
};

USTRUCT(BlueprintType)
struct FTransformation {
	GENERATED_USTRUCT_BODY()

	/* NOTE: Would prefer to have these as seperate structs, however TArray's were giving issues. 
			If revising, test the usage of inheriting from UClass NOT UObject, may solve problem? */


	// General Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShapeShift")
		USkeletalMesh* mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShapeShift")
		float rechargeRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShapeShift")
		float usageRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShapeShift")
		TEnumAsByte<ETransformationID> id;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShapeShift")
		bool unlocked;


	// Bat Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BatTransformation")
		bool isConcealed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BatTransformation")
		float baseConcealmentDuration = 3.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BatTransformation")
		float remainingConcealmentDuration;


	FTransformation() {}

	FTransformation& operator=(const FTransformation& other) {
		mesh = other.mesh;
		rechargeRate = other.rechargeRate;
		usageRate = other.usageRate;
		id = other.id;
		unlocked = other.unlocked;

		isConcealed = other.isConcealed;
		remainingConcealmentDuration = other.remainingConcealmentDuration;
		return *this;
	}

	/* Set all the relevant variables for the specified transformation */
	void Setup(ETransformationID _id);

	/* Called upon each player tick, determines which transformation update to call */
	void Update(float DeltaSeconds, APlayerCharacter& playerRef);

	/* Determines which Primary Ability to call upon - References the current transformation */
	void PrimaryAbility(APlayerCharacter& playerRef);

	/* Determines which Secondary ability to call upon - References the current transformation */
	void SecondaryAbility(APlayerCharacter& playerRef); 


	/* The Bat's primary ability is concealing itself from enemies. */
	void Bat_PrimaryAbility(APlayerCharacter& playerRef);

	/* Handles the echolocation */
	void Bat_SecondaryAbility(APlayerCharacter& playerRef);

	/* Updates the concealment timer */
	void Bat_Update(float DeltaSeconds, APlayerCharacter& playerRef);

	/* Removes the bat's invisibility */
	void RemoveBatInvisibility(APlayerCharacter& playerRef);


	/* Treant Abilities */
	/* Handles the physics to levitate an object */
	/* NOTE: Would be nice to have it working through code */
	void Treant_PrimaryAbility(APlayerCharacter& playerRef);

	/* Updates the physics actor that is levitating - If holding onto one */
	void Treant_Update(float DeltaSeconds, APlayerCharacter& playerRef);
};