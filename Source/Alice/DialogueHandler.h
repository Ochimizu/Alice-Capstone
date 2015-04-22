// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "XmlParser.h"
#include "Containers/UnrealString.h"
#include "DialogueHandler.generated.h"

/**
 * 
 */
USTRUCT()
struct FDialoguePortion {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
		FString character;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
		FString line;

	FDialoguePortion() { }

	FDialoguePortion(FString chara, FString speech) {
		character = chara;
		line = speech;
	}
};


UCLASS()
class ALICE_API ADialogueHandler : public AActor {
	GENERATED_BODY()

public:

	ADialogueHandler(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
		FString ActiveCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
		FString ActiveDialogueLine;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dialogue")
		int32 ActiveIndex;

	TArray<FDialoguePortion> SpeechArray;

	/* Loads the Dialogue's XML file */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		void LoadFile(FString fileName);

	/* Progresses the dialogue */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		bool NextLine(int32 currentIndex);
};
