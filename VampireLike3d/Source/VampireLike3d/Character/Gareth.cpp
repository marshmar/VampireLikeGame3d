#include "Character/Gareth.h"

void AGareth::Attack()
{
	AActor* NearestEnemy = FindNearestEnemy();
	if (!NearestEnemy) return;

	PlayMontage(FName("Attack"), AttackMontage);
}
