# 테스트 문서 — DataPersistence PoC

자동화 테스트 프레임워크 없이 수동 시나리오로 진행한다.  
각 시나리오는 독립적으로 실행 가능하며, 순서대로 수행하면 전체 흐름을 검증할 수 있다.

---

## 테스트 환경

| 항목 | 값 |
|---|---|
| 운영체제 | Windows 10/11 |
| 빌드 구성 | x64 Debug |
| 실행 파일 | `x64/Debug/DataPersistence.exe` |
| 샘플 데이터 파일 | `x64/Debug/data/samples.json` |
| 주문 데이터 파일 | `x64/Debug/data/orders.json` |

---

## 1. JSON 파일 저장 테스트

### 목적

데이터 등록/수정/삭제 후 JSON 파일에 즉시 반영되는지 확인한다.

### 절차

```
1. 앱 실행
2. 샘플 관리 → 샘플 등록
   이름: TestSample / 설명: 저장 테스트용
3. 앱을 종료하지 않고 파일 탐색기로 이동
4. x64/Debug/data/samples.json 파일을 메모장으로 열기
```

### 기대 결과

```json
{"nextId":2,"items":[{"id":1,"name":"TestSample","description":"저장 테스트용"}]}
```

### 확인 항목

- `nextId`가 2인가 (다음 등록 시 id=2 부여 예정)
- `items` 배열에 방금 등록한 항목이 있는가
- JSON 구조가 `{` 로 시작하고 `}` 로 끝나는가
- 파일이 `.tmp` 상태로 남아 있지 않은가

---

## 2. JSON 파일 불러오기 테스트

### 목적

앱 시작 시 기존 파일을 읽어 메모리에 정상 적재되는지 확인한다.

### 절차

```
1. 앱이 종료된 상태에서 x64/Debug/data/samples.json 파일을 메모장으로 연다
2. 아래 내용을 직접 입력하고 저장한다
   {"nextId":4,"items":[{"id":1,"name":"Alpha","description":"첫 번째"},{"id":3,"name":"Gamma","description":"세 번째"}]}
3. 앱 실행
4. 샘플 관리 → 샘플 목록 조회
```

### 기대 결과

- id=1, 이름=Alpha, 설명=첫 번째
- id=3, 이름=Gamma, 설명=세 번째
- 두 건이 목록에 표시된다
- 콘솔에 `[JsonFileStorage] 읽기 성공` 메시지가 출력된다

### 추가 확인

```
5. 새 샘플 등록: 이름=Delta
6. 목록 조회 → id=4가 부여되었는가 확인 (nextId=4에서 이어받음)
```

---

## 3. CRUD 테스트 시나리오

### 3-1. 샘플 CRUD

#### 등록 (Create)

```
1. 앱 실행
2. 샘플 관리 → 1. 샘플 등록
3. 이름: Alpha / 설명: 첫 번째 샘플
4. 등록 → "샘플이 등록되었습니다." 메시지 확인
5. 동일 과정으로 Beta / 두 번째 샘플 등록
```

성공 기준: 목록 조회 시 Alpha(id=1), Beta(id=2) 두 건이 표시된다.

#### 조회 (Read)

```
1. 샘플 관리 → 2. 샘플 목록 조회
2. ID / 이름 / 설명 컬럼이 정렬되어 출력되는가 확인
3. 등록한 모든 샘플이 빠짐없이 표시되는가 확인
```

성공 기준: 등록한 항목 전체가 테이블 형태로 출력된다.

#### 수정 (Update)

```
1. 샘플 관리 → 3. 샘플 수정
2. 현재 목록이 먼저 출력된다
3. ID 입력: 1
4. 현재 Alpha 정보가 출력된다
5. 새 이름: Alpha-수정 / 새 설명: 수정된 설명
6. "샘플이 수정되었습니다." 메시지 확인
7. 목록 조회 → id=1의 이름이 Alpha-수정으로 바뀌었는가 확인
8. samples.json 파일 내용이 변경됐는가 확인
```

성공 기준: 수정 후 목록과 파일 내용이 모두 갱신된다.

#### 삭제 (Delete)

```
1. 샘플 관리 → 4. 샘플 삭제
2. 현재 목록이 먼저 출력된다
3. ID 입력: 2
4. "샘플이 삭제되었습니다." 메시지 확인
5. 목록 조회 → id=2(Beta)가 사라졌는가 확인
6. samples.json → items 배열에 Beta 항목이 없는가 확인
```

성공 기준: 삭제 후 목록에서 해당 항목이 제거되고 파일도 갱신된다.

---

### 3-2. 주문 CRUD

#### 등록 (Create)

```
1. 주문 관리 → 1. 주문 등록
2. 제품명: Widget / 수량: 10 / 상태: 1(접수)
3. "주문이 등록되었습니다." 메시지 확인
4. 추가 등록: 제품명: Gadget / 수량: 5 / 상태: 2(처리 중)
```

성공 기준: 목록 조회 시 Widget(id=1), Gadget(id=2) 두 건이 표시된다.

#### 수정 (Update)

```
1. 주문 관리 → 3. 주문 수정
2. ID 입력: 1
3. 현재 Widget 정보가 출력된다
4. 제품명: Widget-Pro / 수량: 20 / 상태: 3(완료)
5. "주문이 수정되었습니다." 메시지 확인
6. 목록 조회 → id=1의 상태가 완료로 바뀌었는가 확인
```

성공 기준: 수정 후 목록과 파일 내용이 모두 갱신된다.

#### 삭제 (Delete)

```
1. 주문 관리 → 4. 주문 삭제
2. ID 입력: 2 (Gadget)
3. "주문이 삭제되었습니다." 메시지 확인
4. 목록 조회 → Gadget이 사라졌는가 확인
```

성공 기준: 삭제 후 목록에서 제거되고 파일도 갱신된다.

---

## 4. 애플리케이션 재실행 후 데이터 유지 테스트

### 목적

앱을 완전히 종료했다가 다시 실행해도 이전에 저장한 데이터가 그대로 남아있는지 검증한다.

### 절차

```
1. 앱 실행
2. 샘플 등록: PersistTest / 영속성 테스트
3. 주문 등록: PersistOrder / 수량: 7 / 상태: 접수
4. 앱 정상 종료 (0. 돌아가기 → 0. 종료)
5. 앱 재실행
6. 샘플 목록 조회 → PersistTest가 보이는가 확인
7. 주문 목록 조회 → PersistOrder가 보이는가 확인
```

### 기대 결과

- 재실행 후 콘솔에 `[JsonFileStorage] 읽기 성공` 메시지가 출력된다
- 이전에 등록한 샘플과 주문이 그대로 목록에 표시된다
- 데이터의 id, 이름, 설명/수량/상태가 모두 일치한다

### nextId 연번 유지 확인

```
1. 샘플 2건 등록 → id=1, id=2 부여
2. id=2 삭제
3. 앱 종료 후 재실행
4. 새 샘플 등록 → id=3이 부여되어야 한다 (id=2 재사용 금지)
```

성공 기준: 재실행 후에도 ID가 1부터 재시작되지 않는다.

---

## 5. 파일 누락 테스트

### 목적

데이터 파일이 없는 초기 상태에서 앱이 안전하게 시작되는지 확인한다.

### 절차

```
1. x64/Debug/data/ 폴더를 삭제 (또는 samples.json, orders.json 삭제)
2. 앱 실행
3. 콘솔 출력 확인
4. 샘플 목록 조회
5. 주문 목록 조회
6. 샘플 등록: NewSample / 새 파일 테스트
7. 파일 탐색기 확인
```

### 기대 결과

| 단계 | 기대 출력 |
|---|---|
| 앱 시작 시 | `[JsonFileStorage] 파일 없음: data/samples.json (빈 데이터로 시작)` |
| 목록 조회 | `등록된 샘플이 없습니다.` |
| 등록 후 | `data/` 폴더가 자동 생성되고 `samples.json`이 생성된다 |

성공 기준: 파일 없음을 에러로 처리하지 않고, 등록 시 파일과 폴더가 자동 생성된다.

---

## 6. JSON 형식 깨짐 테스트

### 목적

JSON 파일이 손상된 상태에서도 앱이 중단되지 않고 안전하게 처리되는지 확인한다.

### 절차

```
1. x64/Debug/data/samples.json을 메모장으로 열기
2. 파일 내용을 아래와 같이 손상시킨다
   예: BROKEN_JSON_CONTENT
   예: {"nextId":2,"items":[{"id":1  ← 중괄호 미완성
3. 파일 저장
4. 앱 실행
5. 콘솔 출력 확인
6. 샘플 목록 조회
7. x64/Debug/data/ 폴더 내용 확인
```

### 기대 결과

| 항목 | 기대 동작 |
|---|---|
| 콘솔 경고 | `[JsonFileStorage] JSON 형식 오류: data/samples.json` |
| 백업 생성 | `samples.json.bak` 파일이 생성됨 |
| 앱 상태 | 에러 없이 빈 데이터로 시작됨 |
| 목록 조회 | `등록된 샘플이 없습니다.` 출력 |
| 신규 등록 | 정상 동작하고 새 `samples.json`이 생성됨 |

성공 기준: 손상 파일이 있어도 앱이 중단되지 않으며, 원본이 `.bak` 파일로 보관된다.

### 추가 확인: 빈 파일 처리

```
1. samples.json 파일을 열어 내용을 전부 지우고 저장
2. 앱 실행
3. "빈 파일" 메시지가 출력되고 빈 데이터로 시작되는가 확인
```

---

## 7. 잘못된 입력 테스트

### 목적

사용자가 잘못된 값을 입력했을 때 앱이 중단되지 않고 재입력을 유도하는지 확인한다.

### 7-1. 메뉴 선택 오류

```
1. 샘플 관리 메뉴에서 9 입력
2. "잘못된 입력입니다." 메시지가 출력되는가 확인
3. 메뉴로 돌아오는가 확인
```

### 7-2. ID 입력 오류

```
1. 샘플 수정 또는 삭제 선택
2. ID 입력 시 문자열 입력: abc
3. "유효한 ID를 입력하세요." 메시지 후 재입력 프롬프트 확인
4. 음수 입력: -1
5. 동일하게 재입력 유도 확인
```

### 7-3. 존재하지 않는 ID 수정

```
1. 샘플 수정 선택
2. 존재하지 않는 ID 입력: 9999
3. "해당 ID의 샘플을 찾을 수 없습니다." 메시지 확인
4. 파일이 변경되지 않았는가 확인
```

### 7-4. 존재하지 않는 ID 삭제

```
1. 샘플 삭제 선택
2. 존재하지 않는 ID 입력: 9999
3. "해당 ID의 샘플을 찾을 수 없습니다." 메시지 확인
4. 기존 데이터가 그대로인가 확인
```

### 7-5. 이름 빈 문자열 입력

```
1. 샘플 등록 선택
2. 이름 입력 시 Enter만 누르기
3. "이름은 비워둘 수 없습니다." 메시지 후 재입력 프롬프트 확인
```

### 7-6. 수량 0 또는 음수 입력 (주문)

```
1. 주문 등록 선택
2. 수량 입력 시 0 또는 -5 입력
3. "1 이상의 수량을 입력하세요." 메시지 후 재입력 프롬프트 확인
```

### 7-7. 상태 범위 외 입력 (주문)

```
1. 주문 등록 선택
2. 상태 선택 시 5 입력
3. "1~4 사이의 값을 입력하세요." 메시지 후 재입력 프롬프트 확인
```

성공 기준: 모든 잘못된 입력에서 앱이 중단되지 않고 재입력을 유도한다.

---

## 8. Repository 역할 검증

### 검증 목적

Repository가 Controller와 파일 사이에서 완전히 격리된 역할을 하는지 확인한다.

### 검증 항목

| 항목 | 검증 방법 | 기대 결과 |
|---|---|---|
| Controller가 파일에 직접 접근하지 않는가 | `SampleController.cpp`, `OrderController.cpp` 소스 확인 | `fstream`, `filesystem` include 없음 |
| Repository를 통해서만 데이터 변경이 일어나는가 | 등록 후 파일 즉시 확인 | CRUD 직후 파일 반영됨 |
| findById가 없는 ID에 nullptr 반환하는가 | 수정/삭제 시 없는 ID 입력 | "찾을 수 없습니다" 메시지 출력 |
| 삭제 결과가 bool로 명확히 반환되는가 | 없는 ID 삭제 시 | false 반환 → 실패 메시지 출력 |
| nextId가 파일에 함께 저장되는가 | samples.json 파일 내용 직접 확인 | `"nextId":N` 필드가 존재함 |
| 재실행 후 nextId가 복원되는가 | ID 연번 유지 시나리오 수행 | 재실행 후에도 ID 연번이 이어짐 |

### 계층 접근 경계 확인

```
허용된 접근 경로:
  Controller → ISampleRepository → SampleRepository → JsonFileStorage → 파일

금지된 접근:
  Controller → 파일 (직접)
  View → 파일 (직접)
  View → Repository (직접)
```

소스 코드에서 `fstream` 또는 `filesystem` include가 다음 파일에 없는지 확인한다.

- `src/controllers/SampleController.cpp`
- `src/controllers/OrderController.cpp`
- `src/views/SampleView.cpp`
- `src/views/OrderView.cpp`

---

## 9. MVC 역할 분리 검증

### 검증 목적

Model, View, Controller, Persistence 각 계층이 자신의 책임 범위만 담당하는지 확인한다.

### 계층별 책임 기준

| 계층 | 허용 | 금지 |
|---|---|---|
| View | 프롬프트 출력, 사용자 입력 읽기, 타입 검증 | 파일 접근, 비즈니스 로직, Repository 호출 |
| Controller | 흐름 제어, 성공/실패 분기, 메시지 전달 | 파일 접근, JSON 파싱, UI 상세 출력 |
| Repository | CRUD 로직, JSON 직렬화/역직렬화 | UI 출력, 콘솔 입력 |
| JsonFileStorage | 파일 읽기/쓰기, 디렉터리 생성 | 도메인 로직, UI 출력 |

### 소스 코드 확인 포인트

**View 계층** (`src/views/`)
- `std::cout`, `std::cin`, `std::getline` 만 사용
- Repository include 없음
- 비즈니스 조건문(if id exists 등) 없음

**Controller 계층** (`src/controllers/`)
- `m_repo.add()`, `m_repo.findAll()`, `m_repo.findById()` 등 인터페이스만 호출
- `fstream`, `filesystem` include 없음
- JSON 문자열 조작 코드 없음

**Repository 계층** (`src/repositories/`)
- `m_storage.load()`, `m_storage.save()` 만 파일 접근에 사용
- `std::cout` 출력 없음 (JsonFileStorage 상태 메시지 제외)
- `std::cin` 입력 없음

**Persistence 계층** (`src/persistence/`)
- `JsonFileStorage`만 `fstream`, `filesystem` 사용
- 도메인 구조체(`Sample`, `Order`) include 없음

### 인터페이스 교체 가능성 검증

```
1. SampleController.h 확인
   → 멤버 변수가 ISampleRepository& (인터페이스) 인가
   → SampleRepository (구현체) 를 직접 참조하지 않는가

2. main.cpp 확인
   → SampleRepository를 SampleController에 주입하는 위치가 main.cpp인가
   → Controller 생성자가 인터페이스 타입으로 받는가
```

성공 기준: Controller가 인터페이스에만 의존하므로, `SampleRepository`를 다른 구현체로 교체해도 `SampleController.cpp`를 수정할 필요가 없다.

---

## 파일 상태 직접 확인 방법

테스트 중 파일 내용을 직접 확인할 때는 아래 형식과 비교한다.

**samples.json 정상 형식 예시**
```json
{"nextId":3,"items":[{"id":1,"name":"Alpha","description":"첫 번째"},{"id":2,"name":"Beta","description":"두 번째"}]}
```

**orders.json 정상 형식 예시**
```json
{"nextId":3,"items":[{"id":1,"productName":"Widget","quantity":10,"status":"접수"},{"id":2,"productName":"Gadget","quantity":5,"status":"처리 중"}]}
```

확인 항목:
- 파일이 `{`로 시작하고 `}`로 끝나는가
- `nextId` 필드가 존재하는가
- `items` 배열이 존재하는가
- 각 항목의 필드가 누락 없이 모두 있는가

---

## 향후 자동화 테스트 전환 시 고려사항

PoC 검증 완료 후 자동화 테스트를 추가할 때는 다음 구조를 활용한다.

| 테스트 대상 | 방법 |
|---|---|
| Controller 단위 테스트 | `ISampleRepository` Mock 구현체 주입 |
| Repository 단위 테스트 | 임시 디렉터리에 파일을 생성하여 격리 실행 |
| JsonFileStorage 테스트 | 직렬화/역직렬화 입출력 비교, 손상 파일 시나리오 자동화 |
| 통합 테스트 | 실제 파일 기반으로 CRUD 실행 후 파일 내용 검증 |
