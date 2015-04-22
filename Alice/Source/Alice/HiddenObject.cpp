// Fill out your copyright notice in the Description page of Project Settings.

#include "Alice.h"
#include "HiddenObject.h"

AHiddenObject::AHiddenObject(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer) 
{
	inRangeOfEcho = false;
	wasLocated = false;
}
