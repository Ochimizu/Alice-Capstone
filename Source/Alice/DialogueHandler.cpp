// Fill out your copyright notice in the Description page of Project Settings.

#include "Alice.h"
#include "DialogueHandler.h"


ADialogueHandler::ADialogueHandler(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer) 
{
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ID is: %s"), root->GetFirstChildNode()->GetFirstChildNode()->GetContent()));
}

void ADialogueHandler::LoadFile(FString fileName) {
	FXmlFile file(fileName);
	FXmlNode* root = file.GetRootNode();

	TArray<FXmlNode*> rootChildren;
	rootChildren.Init(1);
	rootChildren = root->GetChildrenNodes();

	// Run through the root's children
	for (int i = 0; i < rootChildren.Num(); i++) {
		if (rootChildren.IsValidIndex(i)) {
			TArray<FXmlNode*> children = rootChildren[i]->GetChildrenNodes();

			if (children.IsValidIndex(0) && children.IsValidIndex(1)) {
				FDialoguePortion portion;
				portion.character = children[0]->GetContent();
				portion.line = children[1]->GetContent(); 
				SpeechArray.Add(portion);
			}
		}
	}

	ActiveIndex = 0;
	ActiveCharacter = SpeechArray[ActiveIndex].character;
	ActiveDialogueLine = SpeechArray[ActiveIndex].line;
}

bool ADialogueHandler::NextLine(int32 currentIndex) {
	currentIndex += 1;
	if (SpeechArray.IsValidIndex(currentIndex)) {
		ActiveIndex++;
		ActiveCharacter = SpeechArray[ActiveIndex].character;
		ActiveDialogueLine = SpeechArray[ActiveIndex].line;
		return true;
	}
	else {
		return false;
	}
}


