// Copyright Message


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UObject* InWidgeController)
{
	WidgetController = InWidgeController;
	WidgetControllerSet();
}
