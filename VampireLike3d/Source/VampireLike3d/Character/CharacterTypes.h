#pragma once

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Alive UMETA(DisplayName = "Alive"),
	EDP_DeathForward UMETA(DisplayName = "DeathForward"),
	EDP_DeathBack UMETA(DisplayName = "DeathBack")
};