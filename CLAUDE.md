# CLAUDE.md — ConsoleMVC PoC 1

Claude Code가 이 프로젝트에서 따라야 할 역할 경계, 구현 규칙, Commit 전략을 정의한다.

---

## 프로젝트 개요

- C++17 콘솔 MVC 스켈레톤
- Visual Studio 2022 Console Application
- 외부 라이브러리 없음
- 데이터 저장: 인메모리 (`std::vector`) 한정

---

## 레이어 역할 경계 (엄격 준수)

### Model (`model/`)
**할 수 있는 것:**
- 데이터 구조체(`struct Item`) 정의
- CRUD 연산 구현 (`ItemRepository`)
- STL 컨테이너 사용 (`std::vector`, `std::string` 등)

**절대 하면 안 되는 것:**
- `#include` view/ 또는 controller/ 헤더
- `std::cout`, `std::cin` 직접 호출
- 비즈니스 흐름 결정

### View (`view/`)
**할 수 있는 것:**
- `std::cout`, `std::cin` 사용
- `Item` 구조체 포함 및 출력
- 사용자 입력 수신 후 반환

**절대 하면 안 되는 것:**
- `#include` controller/ 헤더
- `ItemRepository` 직접 호출
- 비즈니스 로직 판단

### Controller (`controller/`)
**할 수 있는 것:**
- `ItemRepository`와 `ItemView`를 생성자 주입으로 사용
- CRUD 흐름 조율 (View 호출 → Repository 호출 → 결과 View에 전달)
- private 메서드로 각 CRUD 흐름 분리

**절대 하면 안 되는 것:**
- `std::cout`, `std::cin` 직접 호출 (View에 위임)
- 데이터 저장 로직 직접 구현

---

## 코딩 규칙

- 언어 표준: C++17
- 헤더 가드: `#pragma once`
- 클래스 멤버: `private` 기본, 필요한 것만 `public`
- 순수 가상 함수 사용: `IView`, `IController`는 인터페이스로만 존재
- 소멸자: 가상 인터페이스는 `virtual ~IView() = default;` 형태로 선언
- 주석: 불필요한 주석 작성 금지 (코드가 스스로 설명해야 함)
- 네이밍: `PascalCase` (클래스), `camelCase` (멤버 변수·메서드), `m_` 접두사 (멤버 변수)

---

## 구현 순서 (이 순서를 반드시 따를 것)

| 단계 | 파일 | 의존 대상 |
|---|---|---|
| 1 | `model/Item.h` | 없음 |
| 2 | `model/ItemRepository.h` | `Item.h` |
| 3 | `view/IView.h` | `Item.h` |
| 4 | `view/ItemView.h` | `IView.h` |
| 5 | `controller/IController.h` | 없음 |
| 6 | `controller/ItemController.h` | `ItemRepository.h`, `IView.h` |
| 7 | `main.cpp` | 모든 헤더 |

---

## Commit 전략

- 파일 1개 완성 = Commit 1개
- 컴파일 통과 확인 후 커밋
- 메시지 형식: `feat: add <파일명> <설명>`

| Commit | 메시지 |
|---|---|
| #1 | `feat: add Item entity struct` |
| #2 | `feat: add in-memory ItemRepository` |
| #3 | `feat: add IView abstract interface` |
| #4 | `feat: add ItemView console implementation` |
| #5 | `feat: add IController abstract interface` |
| #6 | `feat: add ItemController with CRUD flow` |
| #7 | `feat: wire up MVC in main, complete PoC 1 skeleton` |

---

## 범위 제한

- 이번 PoC 1 범위를 벗어난 코드 선제 구현 금지
- DB, 파일 IO, JSON, 외부 라이브러리 코드 작성 금지
- 다음 단계 작업은 제안만 하고 구현하지 않음
- 각 단계가 명시적으로 요청된 경우에만 해당 파일을 작성

---

## 응답 언어

- 모든 설명은 한국어로 작성
- 코드 식별자(클래스명, 변수명, 파일명)는 원문 유지
