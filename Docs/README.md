# HTS-Lite

# 📈 HTS-Lite (C++ Home Trading System)

> 💹 Windows 기반 C++/MFC로 구현한 간단한 HTS(홈 트레이딩 시스템)  
> 종목코드 입력 후 실시간 시세를 조회할 수 있는 미니 트레이딩 앱입니다.

---

## 🚀 주요 기능

- 🧾 **실시간 주식 시세 조회** (TCP 소켓 기반)
- 🖥️ **Windows GUI (MFC Dialog 기반)**
- 🔒 **보안 처리 구조 설계 가능 (인증서 암호화 등 확장)**
- 🧵 **멀티스레딩 구조로 시세 수신/표시 병렬 처리**
- 🔌 **증권사 API 연동 구조 (키움증권, 미래에셋 등)**

---

## 🖼️ 화면 예시

> 종목코드를 입력하고 [조회] 버튼을 누르면 시세 정보가 나타납니다.

<img src="screenshot.png" alt="HTS Screenshot" width="500"/>

---

## 🛠️ 기술 스택

| 구성 요소 | 사용 기술 |
|-----------|-----------|
| 언어 | C++17 |
| 프레임워크 | MFC (Microsoft Foundation Classes) |
| 네트워크 | WinSock2 (TCP 소켓) |
| 데이터 처리 | STL, string, map |
| 보안/암호화 | OpenSSL (예정) |
| 빌드 환경 | Visual Studio 2022 (Windows) |

---

## 📂 프로젝트 구조
HTS-Lite/
├── UI/ # MFC Dialog 및 View
├── Core/ # QuoteEngine, OrderManager 등 핵심 로직
├── Network/ # SocketClient, ProtocolHandler
├── Resources/ # 설정 파일, 리소스
├── main.cpp # Entry point (WinMain)
└── README.md


---

## ✅ 실행 방법

1. Visual Studio 2022로 프로젝트 열기
2. `x64/Release` 또는 `x86/Debug`로 빌드
3. 실행 후 종목코드 입력 → 시세 조회

---

## ⚙️ 향후 계획

- [ ] 실거래 증권사 API 연동 (키움 OpenAPI+, 미래에셋 API 등)
- [ ] 사용자 인증 및 계좌조회
- [ ] 주문 처리 및 체결 내역 구현
- [ ] 차트 시각화 (Qt 또는 OpenGL)
- [ ] SQLite 기반 로컬 종목DB 연동


