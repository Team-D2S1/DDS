# AttributePoint 시스템 설정 가이드

AttributePoint를 사용하여 Primary Attribute(체력, 지구력, 힘, 기량, 마력)를 업그레이드하는 시스템입니다.

## 📋 구현된 파일 목록

### 1. C++ 파일
- **GEExecCalc_UseAttributePoint.h/cpp**: AttributePoint 소모 및 Attribute 증가를 처리하는 ExecutionCalculation
- **DDSAbilitySystemComponent.h/cpp**: `UseAttributePointEffectClass` 멤버 변수 및 `Server_UseAttributePointToAttribute` RPC 함수
- **StatusWidgetController.h/cpp**: UI와 연동되는 컨트롤러, `UpgradeAttribute()` 함수 제공

### 2. 기존 시스템 활용
- **DDSAttributeSet**: `AttributePoints` 이미 구현되어 있음
- **DDSGameplayTags**: `Attribute_Default_AttributePoints` 태그 사용

## 🎮 에디터 설정 방법

### Step 1: GameplayEffect 블루프린트 생성

1. **Content Browser**에서 `Content/Blueprints/GAS/GameplayEffects` 폴더로 이동 (또는 새로 생성)

2. 우클릭 → **Blueprint Class** → **GameplayEffect** 선택

3. 이름을 `GE_UseAttributePoint`로 설정

4. 블루프린트를 열고 다음과 같이 설정:
   - **Duration Policy**: `Instant` (즉시 적용)
   - **Executions** 섹션에서 **+** 버튼 클릭
   - **Calculation Class**: `GEExecCalc_UseAttributePoint` 선택

5. 저장 후 닫기

### Step 2: AbilitySystemComponent에 Effect 할당

1. **플레이어 캐릭터 블루프린트** (예: `BP_Player`) 열기

2. **DDS Ability System Component** 선택

3. **Details 패널**에서 **GAS|Attributes** 카테고리 찾기

4. **Use Attribute Point Effect Class** 항목에 위에서 만든 `GE_UseAttributePoint` 할당

5. 컴파일 & 저장

### Step 3: StatusWidgetController 설정

1. **StatusWidget 블루프린트** (예: `WBP_StatusMenu`) 열기

2. **Widget Controller**를 `StatusWidgetController`로 설정 (이미 되어있을 수 있음)

3. **Details 패널**에서 **GAS|Attributes** 카테고리 찾기

4. **Attribute Info**에 `DA_AttributeInfo` DataAsset 할당

### Step 4: UI 블루프린트 구현

#### AttributePoints 표시하기

```
Event Graph:
- Event Construct
  └─ StatusWidgetController → Bind Event to AttributePointsChangedDelegate
     └─ Create Custom Event (예: "OnAttributePointsChanged")
        └─ TextBlock에 AttributePoints 값 표시
```

#### Attribute 버튼 클릭 처리

```
Button "Upgrade Vitality" 클릭:
- StatusWidgetController → UpgradeAttribute
  └─ Attribute Tag: Attribute.Primary.Vitality
```

예시 블루프린트:
```
[Button_UpgradeVitality] OnClicked
  └─ Get StatusWidgetController
     └─ UpgradeAttribute
        └─ Attribute Tag: [Attribute.Primary.Vitality]
```

## 💡 사용 예시

### 블루프린트에서 직접 호출

```cpp
// Get Player's AbilitySystemComponent
UDDSAbilitySystemComponent* ASC = Player->GetAbilitySystemComponent();

// Upgrade Strength Attribute
FGameplayTag StrengthTag = FGameplayTag::RequestGameplayTag("Attribute.Primary.Strength");
ASC->Server_UseAttributePointToAttribute(StrengthTag);
```

### UI Widget에서 호출

```cpp
// StatusWidgetController를 통해 호출
StatusWidgetController->UpgradeAttribute(AttributeTag);
```

## 🔧 동작 원리

1. **클라이언트**: UI 버튼 클릭 → `StatusWidgetController->UpgradeAttribute()` 호출
2. **RPC**: `Server_UseAttributePointToAttribute()` 서버로 전송
3. **서버**: GameplayEffect 적용 → `GEExecCalc_UseAttributePoint` 실행
4. **Exec**:
   - AttributePoints가 1 이상인지 확인
   - SetByCaller로 전달된 태그 확인
   - AttributePoints -1
   - 해당 Primary Attribute +1
5. **자동 업데이트**: Attribute 변경 → UI에 자동 반영 (델리게이트)

## ✅ 체크리스트

- [ ] `GE_UseAttributePoint` GameplayEffect 블루프린트 생성
- [ ] `GE_UseAttributePoint`에 `GEExecCalc_UseAttributePoint` 할당
- [ ] Player BP의 AbilitySystemComponent에 Effect 할당
- [ ] StatusWidget에 `DA_AttributeInfo` 할당
- [ ] UI에서 AttributePoints 델리게이트 바인딩
- [ ] UI 버튼에 `UpgradeAttribute()` 연결
- [ ] 테스트: AttributePoints 소모 및 Attribute 증가 확인

## 🐛 디버깅

로그 확인:
- `Used AttributePoint! Upgraded: Attribute.Primary.Vitality | Remaining Points: 9`
- `Not enough AttributePoints! Current: 0`

## 📌 주의사항

1. **서버 권한**: AttributePoint 소모는 서버에서만 실행됩니다 (치트 방지)
2. **SetByCaller**: GameplayEffect는 SetByCaller를 통해 어떤 Attribute를 업그레이드할지 결정합니다
3. **Primary Attribute만 가능**: Vitality, Endurance, Strength, Dexterity, Magic만 업그레이드 가능
4. **자동 복제**: AttributeSet은 Replicated이므로 클라이언트에 자동으로 동기화됩니다

## 🎯 확장 가능성

- Attribute별로 필요한 포인트 수를 다르게 설정 (현재는 모두 1)
- 최대 레벨 제한 추가
- 포인트 환불 기능
- 애니메이션/사운드 효과 추가

