ATOTS Simulator

ATO(On-board Automatic Train Operation)와 TS(Trackside) 간의 UDP 기반 메시지 송수신을 테스트하기 위해 개발한 철도 통신 시뮬레이터입니다.

실제 장비 없이도 다양한 철도 통신 메시지를 생성, 송신, 수신 및 분석할 수 있도록 구현하였으며, 현장 시험 및 기능 검증 과정에서 반복적으로 수행되는 테스트를 자동화하기 위해 개발되었습니다.

MFC 기반 GUI 환경에서 다양한 Packet을 구성하고 송수신 결과를 실시간으로 확인할 수 있으며, INI 파일을 이용한 메시지 설정 및 Property Grid 기반 편집 기능을 제공합니다.

📌 Project Overview

본 프로그램은 철도차량(ATO)과 지상장치(TS) 간 통신 프로토콜을 시험하기 위한 Simulator입니다.

주요 목적은 다음과 같습니다.

ATO → TS Packet 생성
TS → ATO Packet 생성
UDP 기반 송수신 테스트
실제 장비 없이 통신 시나리오 검증
각 Packet의 Encoding / Decoding 검증
통신 로그 확인
반복 시험 자동화
📌 Key Features
UDP Communication

UDP Socket을 이용하여 ATO와 TS 간 Packet을 송수신합니다.

송신
수신
Session 분리
실시간 Packet 처리
Packet Encoder

사용자가 입력한 값을 실제 철도 프로토콜 Packet으로 Encoding 합니다.

지원 기능

Header 생성
Bit Packing
Byte Array 생성
CRC 계산
Length 계산
Packet Decoder

수신한 Byte Stream을 Parsing하여 사람이 읽을 수 있는 형태로 표시합니다.

Header 분석
Field Parsing
Packet Type 식별
CRC 검증
Timestamp 표시
Property Grid Editor

Packet의 각 Field를 Property Grid에서 직접 수정할 수 있습니다.

지원 기능

Integer 입력
Enum 선택
Bit Flag 선택
Range 제한
ReadOnly 설정
Dynamic Enable / Disable
Configuration File

Packet 기본값을 INI 파일에서 불러옵니다.

예시

config_HSAck.ini
config_JP.ini
config_SP.ini

이를 통해 코드 수정 없이 다양한 시험 시나리오를 구성할 수 있습니다.

Session Management

다수의 Simulator를 동시에 실행할 수 있도록 Session을 관리합니다.

지원 기능

Session ID 관리
ETCS ID 관리
동일 Session 구분
수신 Packet Routing
Logging

통신 내역을 로그로 저장합니다.

송신 로그
수신 로그
날짜별 저장
시간 기록
Packet Validation

수신 Packet의 유효성을 검증합니다.

CRC 확인
Packet Length 확인
Header 확인
Message Type 확인
📌 Supported Messages

현재 프로젝트에서 지원하는 주요 Packet

HSReq
HSAck
JP
SP
Train Number
Update Indicator
Full Update
Initial Position
No Body Packet

각 Packet은 개별 Dialog 및 Parser를 통해 관리됩니다.

📌 Project Structure
ATOTS_Sim

├── Dialog
│   ├── CTabDlg1
│   ├── CTabDlg2
│   ├── Packet Dialog
│   └── Property Dialog
│
├── Encoder
│   ├── Packet Builder
│   ├── CRC
│   └── Bit Packing
│
├── Decoder
│   ├── Packet Parser
│   ├── Field Parser
│   └── Validation
│
├── Utility
│   ├── Time Conversion
│   ├── File Loader
│   ├── CRC
│   └── Byte Utility
│
└── Config
    ├── HS Ack.ini
    ├── JP.ini
    └── SP.ini
📌 Core Technologies
Language
C++
Framework
Microsoft Foundation Class (MFC)
Communication
UDP Socket
Data Processing
Bit Manipulation
Binary Parsing
Byte Buffer Handling
Configuration
INI File
Development Environment
Microsoft Visual Studio
📌 Architecture
          User

            │

            ▼

     Property Grid

            │

            ▼

     Packet Generator

            │

            ▼

       UDP Sender

            │

============================

      Network

============================

            │

            ▼

      UDP Receiver

            │

            ▼

      Packet Parser

            │

            ▼

     Property Display
📌 Main Components
CTabDlg1 / CTabDlg2

Simulator의 메인 화면입니다.

Packet 생성 및 송수신 기능을 제공합니다.

Packet Encoder

사용자가 입력한 값을 실제 Binary Packet으로 변환합니다.

Packet Decoder

수신한 Packet을 Parsing하여 UI에 표시합니다.

Utility

공통 기능을 담당합니다.

CRC 계산
날짜 변환
Byte 변환
File 처리
Property Grid

Packet Field를 편집하는 UI입니다.

동적으로 입력 가능 여부와 범위를 제어합니다.

📌 Technical Highlights
UDP 기반 실시간 통신 구현
Binary Protocol Encoding / Decoding
CRC Validation
Bit 단위 데이터 처리
Session 기반 Packet Routing
Property Grid를 활용한 Packet 편집 기능
INI 기반 시험 시나리오 구성
실시간 로그 관리
다양한 철도 통신 Packet 지원
📌 Development Purpose

본 프로그램은 실제 철도차량과 지상장치 간 통신을 대체하는 시험용 Simulator로 개발되었습니다.

현장 시험 이전 단계에서 다양한 통신 시나리오를 반복 검증할 수 있도록 하여 개발 및 검증 시간을 단축하고, Packet 생성·분석 과정을 GUI 환경에서 직관적으로 수행할 수 있도록 구현하였습니다.
