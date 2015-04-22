// Fill out your copyright notice in the Description page of Project Settings.

#include "Alice.h"
#include "CollectibleItem.h"
#include "PlayerCharacter.h"

ACollectibleItem::ACollectibleItem(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer) 
{
	USceneComponent* scene;
	scene = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Scene"));
	RootComponent = scene;

	trigger = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Trigger Sphere"));
	trigger->AttachTo(RootComponent);
	trigger->OnComponentBeginOverlap.AddDynamic(this, &ACollectibleItem::OnBeginOverlap);
	trigger->OnComponentEndOverlap.AddDynamic(this, &ACollectibleItem::OnEndOverlap);
	trigger->SetSphereRadius(3);

	isInRange = false;
	type = EItemType::TYPE_NOT_SET;
}

void ACollectibleItem::OnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	APlayerCharacter* player;
	player = Cast<APlayerCharacter>(OtherActor);
	if (player != NULL) {
		if (!isInRange) {
			isInRange = true;
			player->itemsInRange++;
			DisplayTextWidget();
		}
	}
}

void ACollectibleItem::OnEndOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
	if (player != NULL) {
		if (isInRange) {
			isInRange = false;
			player->itemsInRange--;
			HideTextWidget();
		}
	}
}

void ACollectibleItem::SetType(EItemType t) {
	if (t != EItemType::TYPE_NOT_SET && type == EItemType::TYPE_NOT_SET) {
		type = t;
		switch (t) {
		case EItemType::HP_RESTORATIVE:
			name = "HP Potion";
			break;
		case EItemType::MP_RESTORATIVE:
			name = "MP Potion";
			break;
		case EItemType::BLIND_CURE:
			name = "Blind Cure";
			break;
		case EItemType::CURSE_CURE:
			name = "Curse Cure";
			break;
		case EItemType::KEYITEM_01:
			name = player->GetKeyItem(t).name;
			break;
		case EItemType::KEYITEM_02:
			name =  player->GetKeyItem(t).name;
			break;
		case EItemType::KEYITEM_03:
			name =  player->GetKeyItem(t).name;
			break;
		default:
			break;
		}
	}
}

void ACollectibleItem::PickedUp() {
	if (isInRange && GetActorEnableCollision() && player != NULL) {
		player->AddItemToInventory(type);
		player->itemsInRange--;
		HideTextWidget();
		Destroy();
	}
}

void ACollectibleItem::DisplayTextWidget_Implementation() {}

void ACollectibleItem::HideTextWidget_Implementation() {}