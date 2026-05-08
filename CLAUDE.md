# CLAUDE.md — DataPersistence

Claude Code가 이 프로젝트에서 작업할 때 따라야 할 지침.

---

## 프로젝트 컨텍스트

- C++17, Windows Console 애플리케이션
- MVC 구조 완료 상태
- JSON 파일 기반 Persistence Layer를 추가하는 PoC 단계

---

## 아키텍처 원칙

### 계층 경계를 엄격히 지킨다

```
Controller → IItemRepository → JsonItemRepository → JsonFileStore → data/items.json
```

- Controller는 파일 I/O 코드를 포함하지 않는다
- `JsonFileStore`는 도메인 로직을 포함하지 않는다
- 각 계층은 인접한 계층에만 의존한다

### 인터페이스 우선

- Controller는 항상 `IItemRepository` 인터페이스에 의존한다
- 구현체(`JsonItemRepository`)를 Controller가 직접 알면 안 된다

---

## 구현 제약

- DB 사용 금지 (SQLite, PostgreSQL, 외부 서버 포함)
- 외부 JSON 라이브러리 사용 금지 (표준 C++17만 사용)
- 외부 라이브러리가 필요하다고 판단되면 이유를 먼저 설명하고 승인받는다

---

## 파일 구조 규칙

| 계층 | 폴더 |
|---|---|
| Controller | `src/controllers/` |
| View | `src/views/` |
| Model / Interface | `src/models/` |
| Persistence 구현 | `src/persistence/` |
| 유틸리티 | `src/utils/` |

- `src/models/`에는 도메인 모델과 Repository 인터페이스만 둔다
- `src/persistence/`에는 JSON 구현체와 FileStore만 둔다
- 두 폴더를 혼용하지 않는다

---

## 데이터 파일 규칙

- 저장 위치: 실행 파일 기준 `data/items.json`
- `data/` 폴더는 git으로 추적하지 않는다 (`.gitignore` 포함)
- 임시 파일 패턴: 쓰기 시 `items.json.tmp` 먼저 생성 후 교체
- 손상 파일 보관: 파싱 실패 시 `items.json.bak`으로 보관

---

## Commit 전략

각 커밋은 하나의 독립된 기능 단위다. 아래 순서를 따른다.

| 단계 | 내용 |
|---|---|
| 1 | 문서 작성 (README, PRD, CLAUDE.md, tests/README.md) |
| 2 | `JsonFileStore` 구현 (파일 읽기/쓰기, JSON 직렬화) |
| 3 | `JsonItemRepository` 구현 (CRUD + FileStore 연동) |
| 4 | `main.cpp` 배선 수정 (JsonItemRepository 주입) |
| 5 | `ItemController` + `ItemView` 완성 |
| 6 | 수동 검증 (재실행 후 데이터 유지 확인) |

- 각 단계가 독립적으로 빌드 가능해야 한다
- 커밋 메시지는 한글로 작성한다

---

## 예외 처리 규칙

- Repository 경계에서 예외를 처리한다
- Controller는 도메인 수준의 성공/실패만 다룬다
- 파일 관련 예외는 `JsonFileStore`에서 발생시키고 `JsonItemRepository`에서 처리한다

---

## 코드 작성 시 주의사항

- 주석은 WHY가 명확하지 않을 때만 작성한다
- 현재 PoC 범위를 초과하는 추상화나 기능을 추가하지 않는다
- 필드가 3개(`id`, `name`, `price`)인 단순 모델 기준으로 구현한다
- 중첩 구조나 배열 필드가 필요해지면 그 시점에 별도 논의한다
