# ATOTS Simulator

ATO(On-board Automatic Train Operation)와 TS(Trackside) 간 UDP 기반
통신을 시험하기 위해 개발한 철도 통신 시뮬레이터입니다.

실제 차량 및 지상장비 없이도 다양한 철도 통신 메시지를 생성하고
송·수신할 수 있도록 구현하였으며, 프로토콜 검증 및 반복 시험 자동화를
목적으로 개발되었습니다.

MFC 기반 GUI 환경에서 Packet을 생성·수정·전송하고 수신된 데이터를
실시간으로 분석할 수 있으며, Property Grid와 INI 파일을 활용하여 다양한
시험 시나리오를 손쉽게 구성할 수 있습니다.

------------------------------------------------------------------------

## 📌 Overview

본 프로젝트는 철도 차량(ATO)과 지상장치(TS) 간 UDP 통신을 모의 시험하기
위한 Simulator입니다.

### 주요 목적

-   실제 장비 없이 철도 통신 프로토콜 검증
-   ATO ↔ TS 메시지 송수신 테스트
-   Packet Encoding / Decoding 검증
-   반복 시험 자동화
-   프로토콜 개발 및 디버깅 지원

------------------------------------------------------------------------

## 🚀 Key Features

### UDP Communication

-   UDP Socket 기반 실시간 송수신
-   ATO ↔ TS 양방향 통신 지원
-   Session 기반 Packet Routing
-   다중 Simulator 동시 실행 지원

### Packet Encoding

-   Header 생성
-   Length 계산
-   Bit Packing
-   Byte Buffer 생성
-   CRC 계산

### Packet Decoding

-   Header Parsing
-   Message Type 판별
-   Field Parsing
-   CRC Validation
-   Timestamp 표시

### Property Grid Editor

-   Integer 입력
-   Enum 선택
-   Bit Flag 편집
-   Range 제한
-   ReadOnly 설정
-   동적 Enable / Disable

### Configuration File

INI 파일을 이용하여 기본 Packet 값을 로드합니다.

예시

-   config_HSAck.ini
-   config_JP.ini
-   config_SP.ini

### Session Management

-   Session ID 관리
-   ETCS ID 관리
-   Packet Routing
-   다중 Simulator 지원

### Logging

-   송신 로그
-   수신 로그
-   날짜별 로그 저장
-   시간 기록

### Packet Validation

-   CRC 확인
-   Packet Length 확인
-   Header 확인
-   Message Type 확인

------------------------------------------------------------------------

## 📦 Supported Messages

-   HSReq
-   HSAck
-   JP
-   SP
-   Train Number
-   Update Indicator
-   Full Update
-   Initial Position
-   No Body Packet

------------------------------------------------------------------------

## 📁 Project Structure

``` text
ATOTS_Sim
│
├── Dialog
├── Encoder
├── Decoder
├── Utility
└── Config
```

------------------------------------------------------------------------

## 🛠 Core Technologies

-   C++
-   Microsoft Foundation Class (MFC)
-   UDP Socket
-   Binary Protocol
-   Bit Manipulation
-   INI Configuration
-   Microsoft Visual Studio

------------------------------------------------------------------------

## 🏗 Architecture

``` text
User
  │
Property Grid
  │
Packet Generator
  │
UDP Sender
  │
======== Network ========
  │
UDP Receiver
  │
Packet Parser
  │
Property Grid
```

------------------------------------------------------------------------

## 📚 Main Components

### CTabDlg1 / CTabDlg2

Packet 생성, 송신, 수신 및 상태 표시를 담당하는 메인 화면입니다.

### Packet Encoder

입력 데이터를 Binary Packet으로 변환합니다.

### Packet Decoder

수신 Packet을 Parsing하여 화면에 출력합니다.

### Utility

CRC 계산, Byte 변환, 시간 변환, 파일 처리 등 공통 기능을 제공합니다.

### Property Grid

Packet Field를 GUI에서 편집할 수 있도록 지원합니다.

------------------------------------------------------------------------

## ⭐ Technical Highlights

-   UDP 기반 실시간 통신
-   Binary Protocol Encoding / Decoding
-   CRC Validation
-   Bit 단위 데이터 처리
-   Session Routing
-   Property Grid 기반 Packet 편집
-   INI 기반 시험 시나리오 구성
-   실시간 로그 관리

------------------------------------------------------------------------

## 🎯 Development Purpose

본 프로그램은 실제 철도 차량과 지상장치 간 통신을 대체하는 시험용
Simulator입니다.

실제 장비 없이 다양한 통신 시나리오를 반복 검증할 수 있도록 구현하여
개발 및 시험 효율을 높이고, GUI 환경에서 Packet 생성과 분석을 직관적으로
수행할 수 있도록 개발하였습니다.
