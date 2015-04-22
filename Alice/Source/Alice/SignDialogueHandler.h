// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "InteractiveSign.h"
#include "XmlParser.h"
#include "SignDialogueHandler.generated.h"

/**
 * 
 */
UCLASS()
class ALICE_API ASignDialogueHandler : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SignDialogue")
		TArray<FSignInformation> InformationArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SignDialogue")
		TArray<AInteractiveSign*> SignArray;

	ASignDialogueHandler(const FObjectInitializer& ObjectInitializer);
	
	/* Load data from the file path and populate the sign array's information */
	UFUNCTION(BlueprintCallable, Category = "SignDialogue")
		void LoadFile(FString fileName);
};
