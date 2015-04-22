// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
/**
 * 
 */
#include "Array.h"
#include "Containers/UnrealString.h"
#include "Inventory.generated.h"

UENUM(BlueprintType)
enum class EItemType {
	HP_RESTORATIVE,
	MP_RESTORATIVE,
	BLIND_CURE,
	CURSE_CURE,
	ITEM_COUNT,
	KEYITEM_01,
	KEYITEM_02,
	KEYITEM_03,
	TYPE_NOT_SET
};

USTRUCT(BlueprintType)
struct FKeyItem {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "KeyItem")
		bool beenCollected;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KeyItem")
		FString flavourText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KeyItem")
		FString name;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KeyItem")
		TEnumAsByte<EItemType> type;

	FKeyItem() {
		type = EItemType::TYPE_NOT_SET;
	}

	void Setup(EItemType t) {
		if (t > EItemType::ITEM_COUNT && t < EItemType::TYPE_NOT_SET) {
			type = t;

			beenCollected = false;
			flavourText = "This is where my flavor text would go if I had any";
			name = "I have a name?";
		}
	}

};

struct FInventoryItem {
	TEnumAsByte<EItemType> type;
	float value;
	int32 inventoryCount;

	FInventoryItem() {}

	FInventoryItem(EItemType t) {
		Setup(t);
	}

	void Setup(EItemType t) {
		type = t;
		inventoryCount = 1;

		switch (t) {
			case EItemType::HP_RESTORATIVE:
				value = 100;
				break;
			case EItemType::MP_RESTORATIVE:
				value = 100;
				break;
			case EItemType::BLIND_CURE:
			case EItemType::CURSE_CURE:
				value = 0;
				break;
			default:
				type = EItemType::TYPE_NOT_SET;
				break;
		}
	}
};


class ALICE_API Inventory {
public:
	//TArray<FInventoryItem> items;
	FInventoryItem* items;
	TArray<FKeyItem> keyItems;

	Inventory();
	~Inventory();

	/* Add the specified item to the inventory */
	void AddItem(EItemType type);

	/* Apply the specified item type to the player */
	bool UseItem(EItemType type, struct FPlayerInformation& playerInfo);

	/* Setup the key items */
	void LoadKeyItems();

	/* Retrieve a specific key item upon request */
	FKeyItem GetKeyItem(EItemType type);
};
