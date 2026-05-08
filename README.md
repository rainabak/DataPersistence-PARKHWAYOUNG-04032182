# ConsoleMVC

C++17 기반 콘솔 MVC 패턴 스켈레톤 프로젝트 (PoC 1)

---

## 개요

Model / View / Controller 레이어 역할 분리를 콘솔 애플리케이션으로 시연하는 PoC 1 프로젝트입니다.  
단일 엔티티(Item)의 CRUD 흐름을 통해 MVC 구조를 구현합니다.

---

## 기술 스택

| 항목 | 내용 |
|---|---|
| 언어 | C++17 |
| 빌드 환경 | Visual Studio 2022, Console Application |
| 외부 라이브러리 | 없음 |
| 데이터 저장 | 인메모리 (`std::vector`) |

---

## 폴더 구조

```
FinalProject/
├── FinalProject.slnx
├── .gitignore
├── README.md
├── PRD.md
├── CLAUDE.md
│
├── ConsoleMVC/
│   ├── ConsoleMVC.vcxproj
│   ├── ConsoleMVC.vcxproj.filters
│   ├── main.cpp
│   │
│   ├── model/
│   │   ├── Item.h
│   │   └── ItemRepository.h
│   │
│   ├── view/
│   │   ├── IView.h
│   │   └── ItemView.h
│   │
│   └── controller/
│       ├── IController.h
│       └── ItemController.h
│
└── tests/
    └── README.md
```

---

## MVC 역할 분리

### Model
- 데이터 구조와 상태만 소유
- `Item` : 엔티티 구조체 (`id`, `name`, `price`)
- `ItemRepository` : 인메모리 CRUD 연산 제공
- View / Controller 헤더를 `#include`하지 않음

### View
- 사용자 입출력만 담당
- `IView` : 입출력 추상 인터페이스
- `ItemView` : 콘솔 기반 구체 구현
- `std::cout` / `std::cin`은 View 레이어에만 존재

### Controller
- Model과 View를 조율
- `IController` : `run()` 진입점 인터페이스
- `ItemController` : CRUD 흐름 처리
- 직접 `std::cout` / `std::cin` 호출 금지

---

## 빌드 방법

1. Visual Studio 2022에서 `FinalProject.slnx` 파일 열기
2. 솔루션 탐색기에서 `ConsoleMVC` 프로젝트 선택
3. `Ctrl+Shift+B` 로 빌드
4. `Ctrl+F5` 로 실행

---

## 실행 흐름

```
프로그램 시작
  └─ main.cpp
       └─ ItemRepository 생성 (인메모리)
       └─ ItemView 생성
       └─ ItemController 생성 (Repository + View 주입)
       └─ controller.run() 호출
            └─ 메뉴 출력 → 사용자 입력 → CRUD 처리 → 결과 출력 → 반복
```

---

## 구현 순서 (PoC 1)

| 단계 | 파일 | 설명 |
|---|---|---|
| 1 | `model/Item.h` | 엔티티 구조체 |
| 2 | `model/ItemRepository.h` | 인메모리 저장소 |
| 3 | `view/IView.h` | View 추상 인터페이스 |
| 4 | `view/ItemView.h` | 콘솔 View 구현 |
| 5 | `controller/IController.h` | Controller 추상 인터페이스 |
| 6 | `controller/ItemController.h` | CRUD 흐름 구현 |
| 7 | `main.cpp` | 조립 및 실행 |
