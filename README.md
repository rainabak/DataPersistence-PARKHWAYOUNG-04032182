# DataPersistence

C++17 기반 Console MVC 애플리케이션.  
DB 없이 JSON 파일로 데이터를 저장하고 불러오는 Persistence Layer를 검증하는 PoC 프로젝트.

---

## 프로젝트 목표

- MVC 구조에서 Persistence 계층을 명확히 분리한다.
- 애플리케이션을 재실행한 후에도 데이터가 유지되는지 검증한다.
- DB 없이 JSON 파일만으로 CRUD가 가능한지 확인한다.
- Repository 패턴이 Controller와 저장 방식을 얼마나 잘 격리하는지 검증한다.

---

## 기술 스택

| 항목 | 내용 |
|---|---|
| 언어 | C++17 |
| 빌드 | Visual Studio / MSBuild |
| 플랫폼 | Windows x64 |
| UI | Console (텍스트 메뉴) |
| 저장소 | JSON 파일 (`data/items.json`) |
| 외부 라이브러리 | 없음 (표준 라이브러리만 사용) |

---

## 아키텍처 개요

```
[Controller]
    ↓ IItemRepository 인터페이스 호출
[JsonItemRepository]      ← Persistence 계층
    ↓ 파일 I/O 위임
[JsonFileStore]
    ↓
[data/items.json]         ← 실제 저장 파일
```

Controller는 저장 방식을 모른다. Repository 인터페이스만 사용한다.  
JSON 파일이 뒤에 있다는 사실은 Persistence 계층이 독점적으로 안다.

---

## 폴더 구조

```
src/
├── controllers/        # MVC Controller 계층
├── views/              # MVC View 계층
├── models/             # 도메인 모델 및 Repository 인터페이스
├── persistence/        # JSON 기반 Persistence 구현체
└── utils/              # 공통 유틸리티

data/                   # 런타임 생성 (git 추적 제외)
└── items.json

tests/                  # 테스트 전략 및 수동 검증 시나리오
└── README.md
```

---

## JSON 저장 파일 구조

저장 파일(`data/items.json`)은 다음 형태를 따른다.

- 최상위 객체 안에 `nextId`와 `items` 배열을 함께 저장한다.
- `nextId`는 재실행 후에도 ID 연번이 유지되도록 파일에 함께 보관한다.
- 필드는 `id`, `name`, `price` 세 가지다.

---

## DB를 사용하지 않는 이유

이 프로젝트는 PoC 단계다. 목표는 저장 방식 자체가 아니라 **Persistence 계층 분리 구조**를 검증하는 것이다.

- DB를 쓰면 서버 설치, 연결 설정, 스키마 관리가 필요하다.
- 구조 검증에 불필요한 복잡성이 추가된다.
- Repository 인터페이스 덕분에 나중에 DB로 교체할 때 Controller 코드를 건드리지 않아도 된다.

---

## Repository 패턴

`IItemRepository`는 CRUD 연산의 인터페이스다.  
Controller는 이 인터페이스만 의존한다. 구현체가 메모리인지, 파일인지, DB인지 모른다.

`JsonItemRepository`가 이번 PoC에서 인터페이스를 구현한다.  
나중에 `SqliteItemRepository`로 교체해도 Controller 코드는 변하지 않는다.

---

## 데이터 흐름

**시작 시**: JSON 파일 → 파싱 → 메모리 벡터 적재  
**변경 시**: 메모리 벡터 수정 → JSON 직렬화 → 파일 전체 덮어쓰기  
**종료 시**: 마지막 쓰기 시점 상태가 파일에 보존됨

---

## 실행 방법

1. Visual Studio에서 빌드한다.
2. `x64/Debug/DataPersistence.exe`를 실행한다.
3. 메뉴에서 Item을 추가, 수정, 삭제한다.
4. 프로그램을 종료했다가 다시 실행한다.
5. 이전에 입력한 데이터가 그대로 남아 있으면 Persistence 검증 성공이다.
