// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Containers/UnrealString.h"
#include "InteractiveSign.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSignInformation {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SignDialogue")
		int32 signID;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SignDialogue")
		FString line;

	FSignInformation() {
		signID = -1;
		line = " ";
	}

};

UCLASS()
class ALICE_API AInteractiveSign : public AActor {
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SignDialogue")
		FSignInformation signInfo;

	AInteractiveSign(const FObjectInitializer& ObjectInitializer);
};
