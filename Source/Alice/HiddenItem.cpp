// Fill out your copyright notice in the Description page of Project Settings.

#include "Alice.h"
#include "HiddenItem.h"


AHiddenItem::AHiddenItem(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	wasLocated = false;
	inRangeOfEcho = false;
}

