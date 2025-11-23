# 블루프린트 수정 가이드 - 무기 장착/해제 시스템

## 문제점
데디케이트 서버 환경에서 애니메이션 레이어 링크와 능력 부여가 제대로 복제되지 않는 문제가 있었습니다.
블루프린트에서 `K2_HasAuthority` 체크 후 Multicast를 호출하는 방식이 일관성이 없었습니다.

## 해결 방법
C++에서 `HandleWeaponEquip`와 `HandleWeaponUnequip` 함수를 추가하여 모든 복제 로직을 한 곳에서 관리하도록 변경했습니다.

---

## GA_Player_EquipSword 블루프린트 수정

### HandleEquipWeapon 함수 수정

**기존 복잡한 로직을 다음과 같이 단순화하세요:**

1. **함수 시작 부분 (InWeaponToEquip 파라미터 받은 직후)**:
   ```
   InWeaponToEquip 파라미터를 받음
   ↓
   [GetPlayerCombatComponentFromActorInfo] 호출
   ↓
   [HandleWeaponEquip] 호출
   - Target: 위에서 받은 Combat Component
   - Weapon To Equip: InWeaponToEquip
   ```

2. **삭제할 기존 노드들**:
   - ExecutionSequence (3개 분기)
   - K2_HasAuthority 체크들
   - IsLocallyControlled 체크
   - Multicast_LinkAnimLayer 호출
   - AddMappingContext 호출
   - GetDDSAbilitySystemComponentFromActorInfo
   - GrantPlayerWeaponAbilities 호출
   - AssignGrantedAbilitySpecHandles 호출
   - 관련된 Break Struct, Get 노드들

3. **유지할 노드들**:
   - InWeaponToEquip 파라미터 받기
   - ChachedPlayerWeaponData 로컬 변수 (필요하다면)

---

## GA_Player_DespawnCrafted 블루프린트 수정 (또는 GA_Player_UnequipSword)

### HandleUnequipWeapon 함수 수정

**기존 복잡한 로직을 다음과 같이 단순화하세요:**

1. **함수 시작 부분 (InWeaponToUnequip 파라미터 받은 직후)**:
   ```
   InWeaponToUnequip 파라미터를 받음
   ↓
   [GetPlayerCombatComponentFromActorInfo] 호출
   ↓
   [HandleWeaponUnequip] 호출
   - Target: 위에서 받은 Combat Component
   - Weapon To Unequip: InWeaponToUnequip
   ```

2. **삭제할 기존 노드들**:
   - ExecutionSequence (3개 분기)
   - K2_HasAuthority 체크들
   - IsLocallyControlled 체크
   - Multicast_UnlinkAnimLayer 호출
   - RemoveMappingContext 호출
   - GetDDSAbilitySystemComponentFromActorInfo
   - RemoveGrantedPlayerWeaponAbilities 호출
   - 관련된 Break Struct, Get 노드들

---

## 변경 사항 요약

### C++에서 처리하는 것들:
1. **애니메이션 레이어 링크/언링크** - 서버에서 Multicast 호출하여 모든 클라이언트에 복제
2. **능력 부여/제거** - 서버에서만 실행
3. **IMC 추가/제거** - 로컬 플레이어만 실행

### 블루프린트에서 할 일:
1. 무기 액터를 찾기
2. `HandleWeaponEquip` 또는 `HandleWeaponUnequip` 한 번만 호출
3. 나머지는 C++가 자동으로 처리

---

## 장점

1. **일관성**: 모든 복제 로직이 C++에서 통일되어 관리됨
2. **단순성**: 블루프린트가 매우 단순해짐
3. **디버깅**: C++ 로그로 모든 단계 추적 가능
4. **안정성**: 데디케이트 서버와 리슨 서버 모두에서 동일하게 작동

---

## 테스트 체크리스트

- [ ] 리슨 서버 환경에서 무기 장착/해제 테스트
- [ ] 데디케이트 서버 환경에서 무기 장착/해제 테스트
- [ ] 3명 이상 플레이어로 테스트
- [ ] 서버 플레이어의 애니메이션이 다른 클라이언트에게 보이는지 확인
- [ ] 클라이언트 플레이어의 애니메이션이 서버와 다른 클라이언트에게 보이는지 확인
- [ ] IMC가 로컬 플레이어에만 적용되는지 확인
- [ ] 능력이 제대로 부여/제거되는지 확인

---

## 로그 확인

수정 후 다음 로그들이 보여야 합니다:

```
[Server] Calling Multicast_LinkAnimLayer for weapon: BP_PlayerSswordGrip_C_0
[Server] Granted 3 abilities to weapon: BP_PlayerSswordGrip_C_0
[Client] Added IMC for weapon: BP_PlayerSswordGrip_C_0
[Server] Multicast_LinkAnimLayer called - Role: 3, RemoteRole: 2
[Client] Multicast_LinkAnimLayer called - Role: 2, RemoteRole: 3
[Server] Successfully linked anim layer: AnimLayer_PlayerTestSword_C
[Client] Successfully linked anim layer: AnimLayer_PlayerTestSword_C
```

---

## 추가 정보

문제가 계속되면:
1. OutputLog에서 "Multicast_LinkAnimLayer" 검색
2. 서버와 클라이언트 모두에서 호출되는지 확인
3. Role: 3 (Authority), RemoteRole: 2 (SimulatedProxy) 확인
4. "Successfully linked anim layer" 메시지 확인


