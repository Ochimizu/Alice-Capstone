// Fill out your copyright notice in the Description page of Project Settings.

#include "Alice.h"
#include "KeyItemsHandler.h"


AKeyItemsHandler::AKeyItemsHandler(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer) 
{ }

void AKeyItemsHandler::LoadFile(FString fileName, APlayerCharacter* playerRef) {
	FXmlFile file(fileName); // "E://KeyItems.xml"
	FXmlNode* root = file.GetRootNode();

	TArray<FXmlNode*> rootChildren = root->GetChildrenNodes();

	// Run through the root's children
	for (int i = 0; i < rootChildren.Num(); i++) {
		if (rootChildren.IsValidIndex(i)) {
			TArray<FXmlNode*> children = rootChildren[i]->GetChildrenNodes();

			if (children.IsValidIndex(0) && children.IsValidIndex(1)) {
				playerRef->inventory.keyItems[i].name = children[0]->GetContent();
				playerRef->inventory.keyItems[i].flavourText = children[1]->GetContent();
			}
		}
	}

}

