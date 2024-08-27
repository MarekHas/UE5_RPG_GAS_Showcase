// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/FFS_UserWidget.h"

void UFFS_UserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
