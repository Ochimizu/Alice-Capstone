// Fill out your copyright notice in the Description page of Project Settings.

#include "Alice.h"
#include "Inventory.h"
#include "PlayerCharacter.h"


Inventory::Inventory() {
	items = new FInventoryItem[4];

	items[(int)EItemType::HP_RESTORATIVE] = FInventoryItem(EItemType::HP_RESTORATIVE);
	items[(int)EItemType::MP_RESTORATIVE] = FInventoryItem(EItemType::MP_RESTORATIVE);
	items[(int)EItemType::BLIND_CURE] = FInventoryItem(EItemType::BLIND_CURE);
	items[(int)EItemType::CURSE_CURE] = FInventoryItem(EItemType::CURSE_CURE);

	
}

Inventory::~Inventory() {}

void Inventory::LoadKeyItems() {
	for (int i = 0; i < 3; i++) {
		keyItems.Add(FKeyItem());
		int typeID = (int)EItemType::KEYITEM_01 + i;
		keyItems[i].Setup((EItemType)typeID);
	}

	//keyItems[0].Setup(EItemType::KEYITEM_01);
	//keyItems[1].Setup(EItemType::KEYITEM_02);
	//keyItems[2].Setup(EItemType::KEYITEM_03);
}

void Inventory::AddItem(EItemType type) {
	// If it's a regular item 
	if (type < EItemType::ITEM_COUNT && (int)type >= 0) {
		items[(int)type].inventoryCount++;
	}
	// If it's within the "KeyItem" range
	else if (type > EItemType::ITEM_COUNT && type < EItemType::TYPE_NOT_SET) {
		switch (type) {
			case EItemType::KEYITEM_01:
				keyItems[0].beenCollected = true;
				break;
			case EItemType::KEYITEM_02:
				keyItems[1].beenCollected = true;
				break;
			case EItemType::KEYITEM_03:
				keyItems[2].beenCollected = true;
				break;
			default:
				break;
		}
	}
}

bool Inventory::UseItem(EItemType type, FPlayerInformation& playerInfo) {
	int iType = (int)type;
	
	if (type < EItemType::ITEM_COUNT && iType >= 0 
		&& items[iType].inventoryCount > 0) 
	{
		// Use Item
		int value = items[iType].value;

		switch (type) {
			case EItemType::HP_RESTORATIVE:
				if ((playerInfo.currentHP + value) > playerInfo.BASE_MAX_HP)
					playerInfo.currentHP = playerInfo.BASE_MAX_HP;
				else
					playerInfo.currentHP += value;
				break;
			case EItemType::MP_RESTORATIVE:
				if ((playerInfo.currentMP + value) > playerInfo.BASE_MAX_MP)
					playerInfo.currentMP = playerInfo.BASE_MAX_MP;
				else
					playerInfo.currentMP += value;
				break;
			case EItemType::BLIND_CURE:
				if (playerInfo.currentStatus == EStatusEffect::BLIND) {
					playerInfo.currentStatus = EStatusEffect::NONE;
				}
				break;
			case EItemType::CURSE_CURE:
				if (playerInfo.currentStatus == EStatusEffect::CURSE) {
					playerInfo.currentStatus = EStatusEffect::NONE;
				}
				break;
			default:
				break;
		}
		
		// The item has been used
		items[iType].inventoryCount--;
		return true;
	}
	return false;
}

FKeyItem Inventory::GetKeyItem(EItemType type) {
	if (type > EItemType::ITEM_COUNT && type < EItemType::TYPE_NOT_SET) {
		int id = ((int)type - (int)EItemType::ITEM_COUNT) - 1;
		if (keyItems.IsValidIndex(id))
			return keyItems[id];
	}
	return FKeyItem();
}