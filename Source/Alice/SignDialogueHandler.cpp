// Fill out your copyright notice in the Description page of Project Settings.

#include "Alice.h"
#include "SignDialogueHandler.h"

ASignDialogueHandler::ASignDialogueHandler(const FObjectInitializer& ObjectInitializer) 
: Super(ObjectInitializer) 
{

}

void ASignDialogueHandler::LoadFile(FString fileName) {
	FXmlFile file(fileName);
	FXmlNode* root = file.GetRootNode();

	if (root != NULL) {
		TArray<FXmlNode*> rootChildren = root->GetChildrenNodes();

		// Run through the root's children
		for (int i = 0; i < rootChildren.Num(); i++) {
			if (rootChildren.IsValidIndex(i)) {
				TArray<FXmlNode*> children = rootChildren[i]->GetChildrenNodes();

				if (children.IsValidIndex(0) && children.IsValidIndex(1)
					&& SignArray.IsValidIndex(i)) {

					SignArray[i]->signInfo.signID = FCString::Atoi(*children[0]->GetContent());
					SignArray[i]->signInfo.line = children[1]->GetContent();
				}
			}
		}
	}
}

