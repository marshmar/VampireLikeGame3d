#include "Systems/UI/HUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHUDWidget::UpdateHpBar(float Current, float Max)
{
    if (HpBar)
        HpBar->SetPercent(Current / Max);
}

void UHUDWidget::UpdateBossHpBar(float Current, float Max)
{
    if (BossHpBar)
        BossHpBar->SetPercent(Current / Max);
}

void UHUDWidget::SetBossHpBar(bool bState)
{
    if (BossHpBar)
    {
        BossHpBar->SetVisibility(bState ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
    if (BossName)
    {
        BossName->SetVisibility(bState ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

