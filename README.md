# 3D Vamire Survival

<p align="center">
  <img src="./gifs/gameplay.gif" alt="Gameplay">
  <img src="./gifs/Reward.gif" alt="Gameplay">
  <img src="./gifs/Diary.gif" alt="Gameplay">
</p>

`3D Vampire Survival`은 서로 다른 공격 방식을 가진 3명의 캐릭터를 교대하며, 주어진 시간 동안 적을 처치하고 살아남는 3D 액션 로그라이크 게임입니다.
스왑 게이지가 충분한 상태에서 캐릭터를 교체하면, 퇴장하는 캐릭터가 고유한 스왑 액션을 수행합니다.

<br>

## Project Overview

| 항목 | 내용 |
|---|---|
| 개발 기간 | 2026.03 ~ 2026.06 |
| 개발 인원 | 개인 프로젝트 |
| 플랫폼 | PC |
| 주요 구현 | 캐릭터 전투 및 교체, Enemy Spawn, Object Pooling, Boss AI, HUD / UI |

<br>

## Tech Stack

- **Engine**: Unreal Engine 5.3
- **Language**: C++

<br>

## Core Features

### 1. Player / Combat
- 캐릭터별 전투 로직 및 자동 공격 구현
- 근접, 원거리 캐릭터 구현

### 2. Party / Swap Action
- 3인 파티 구성 및 플레이 캐릭터 교체
- 스왑 게이지에 따른 퇴장 캐릭터의 고유 액션 실행
- AnimNotify와 Delegate를 통한 스왑 액션 종료 및 비활성화 처리

### 3. Enemy / Wave
- DataTable 기반 시간별 스폰 페이즈 구성
- 페이즈별 목표 적 수와 적 종류별 구성 비율 관리
- Object Pooling을 활용한 적 재사용

### 4. Boss AI
- Behaviour Tree와 Blackboard 기반 보스 행동 제어
- 충격파, 범위 공격, 점프 공격을 개별 Task로 구현
- 체력 조건에 따른 페이즈 전환
- 공격 범위를 알리는 경고 Decal 및 이펙트 연동


<br>

## Architecture

### 캐릭터 교체 흐름 
```text
캐릭터 교체 요청
        ↓
PartyManager
        ├─ 스왑 게이지 확인
        │      ├─ 충분: 퇴장 캐릭터의 스왑 액션 실행
        │      │              ↓
        │      │       종료 AnimNotify
        │      │              ↓
        │      │       Delegate → 캐릭터 비활성화
        │      │
        │      └─ 부족: 퇴장 캐릭터 즉시 비활성화
        │
        └─ 진입 캐릭터로 위치·회전 전달 및 Possess
                       ↓
              OnPlayerSwapped
                       ↓
                적 추적 대상 갱신
```

<br>

### 주요 클래스

| 클래스 | 역할 |
|---|---|
| [`APartyManager`](./VampireLike3d/Source/VampireLike3d/Systems/Party/PartyManager.h) | 파티 구성, 캐릭터 교체, 공용 능력치 및 교체 이벤트 관리 |
| [`ABaseCharacter`](./VampireLike3d/Source/VampireLike3d/Entities/Character/BaseCharacter.h) | 캐릭터 공통 전투 기능과 스왑 액션 인터페이스 제공 |
| [`ASpawnManager`](./VampireLike3d/Source/VampireLike3d/Systems/Wave/SpawnManager.h) | 페이즈 데이터에 따른 적 종류 선택 및 스폰 수 관리 |
| [`UPoolManagerSubsystem`](./VampireLike3d/Source/VampireLike3d/Systems/ObjectPool/PoolManagerSubsystem.h) | 클래스별 Actor 풀 생성, 획득 및 반환 관리 |
| [`ABossAIController`](./VampireLike3d/Source/VampireLike3d/Entities/Enemies/BossAIController.h)| Behaviour Tree 실행 및 Blackboard 타깃 갱신|

<br>

## Technical Highlights

### AnimNotify 기반 3인 캐릭터 스압 액션

#### 문제
캐릭터 교체 시 게이지가 충분할 경우 캐릭터가 스왑 액션을 마친 뒤 비활성화되어야 했습니다.
캐릭터마다 이동, 타격, 이펙트 발생 시점과 몽타주 길이가 달라, 코드에 고정된 타이머나 딜레이를 사용하면 애니메이션이 변경될 때마다 실행 시점을 다시 조정해야 했습니다.

#### 접근
애니메이션은 행동 시점을, 캐릭터는 실제 동작을, [`PartyManager`]는 교체와 퇴장을 담당하도록 역할을 분리했습니다.

**행동 시점은 AnimNotify로 전달했습니다.**  
몽타주 타임라인에 Notify를 배치하여 스왑 액션의 시작, 이동, 타격, 이펙트, 종료 시점을 전달하도록 구성했습니다.

**캐릭터별 동작은 가상 함수로 분리했습니다.**  
[`UBaseCharacterAnimInstance`](./VampireLike3d/Source/VampireLike3d/Entities/Character/BaseCharacterAnimInstance.h)는 [`ABaseCharacter`](./VampireLike3d/Source/VampireLike3d/Entities/Character/BaseCharacter.h)의 공통 함수를 호출하고, 각 캐릭터는 해당 함수를 override하여 고유한 동작을 구현했습니다.

| 캐릭터 | 전투 유형 | 스왑 액션 |
|---|---|---|
| Revenant | 원거리 | LineTrace 기반 관통 레이저 공격 |
| Nullex | 소환 | 적 위치에 중력장을 소환하여 둔화 |
| Gareth | 근접 | 적 위치로 낙하한 뒤 범위 타격 |

**퇴장 처리는 종료 Delegate와 연결했습니다.**  
스왑 게이지가 충분하면 퇴장 캐릭터의 액션을 실행하면서 새 캐릭터로 조작을 전환합니다. 
이후 종료 Notify에서 `OnSwapAttackEnded`를 브로드캐스트하면, PartyManager가 퇴장 캐릭터의 표시•충돌•Tick을 비활성화합니다.

```text
SwapAttackMontage
        ↓
AnimNotify
        ↓
BaseCharacter의 공통 함수
        ↓
캐릭터별 override 실행
        ↓
종료 Notify
        ↓
OnSwapAttackEnded
        ↓
PartyManager → 퇴장 캐릭터 비활성화
```

#### 결과
- 이동•타격•이펙트 시점을 몽타주에서 조정할 수 있도록 구성
- 공통 Notify 체계를 유지하면서 캐릭터별 스왑 액선 구현
- 스왑 액션 종료 시점과 퇴장 처리를 Delegate로 연결

<br>

## Links

- [Gameplay Video](https://youtu.be/q749euj1SLg)

