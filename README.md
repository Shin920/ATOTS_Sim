# ATOTS Simulator

ATO(On-board Automatic Train Operation)와 TS(Trackside) 간 UDP 기반 철도
통신을 시험하기 위해 개발한 MFC 기반 Simulator입니다.

실제 차량 또는 지상장비 없이 다양한 철도 프로토콜 Packet을 생성, 송신,
수신 및 분석할 수 있도록 구현하였으며 개발 및 검증 단계에서 반복되는
시험을 자동화하기 위한 목적으로 개발되었습니다.

------------------------------------------------------------------------

# 📌 Overview

본 프로젝트는 철도 차량과 지상장치 간의 통신 프로토콜을 GUI 환경에서
시험하기 위한 Simulator입니다.

## 주요 기능

-   UDP 기반 실시간 송수신
-   Packet Encoding / Decoding
-   Property Grid 기반 Packet 편집
-   Session 관리
-   CRC 검증
-   INI 기반 시험 시나리오
-   Packet 로그 확인

------------------------------------------------------------------------

# 🏗 System Architecture

``` mermaid
flowchart LR

User --> UI[MFC UI]

UI --> PropertyGrid

PropertyGrid --> PacketBuilder

PacketBuilder --> Encoder

Encoder --> UDP

UDP --> Network

Network --> UDP2[UDP Receiver]

UDP2 --> Decoder

Decoder --> PacketParser

PacketParser --> PropertyGrid2[Packet Viewer]
```

------------------------------------------------------------------------

# 🔄 Packet Processing Flow

``` mermaid
sequenceDiagram

participant User
participant UI
participant Encoder
participant UDP
participant Receiver
participant Decoder

User->>UI: Edit Packet

UI->>Encoder: Build Packet

Encoder->>UDP: Send

UDP->>Receiver: Receive

Receiver->>Decoder: Parse

Decoder-->>UI: Display Result
```

------------------------------------------------------------------------

# 🚆 Supported Messages

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

# 📦 Project Structure

``` text
ATOTS_Sim

├── Main Dialog
├── CTabDlg1
├── CTabDlg2
├── Packet Dialogs
├── Encoder
├── Decoder
├── Utility
├── Config(INI)
├── Resource
└── jsoncpp
```

------------------------------------------------------------------------

# 🧩 Component Diagram

``` mermaid
graph TD

Main --> CTabDlg1
Main --> CTabDlg2

CTabDlg1 --> Encoder
CTabDlg2 --> Decoder

Encoder --> Utility
Decoder --> Utility

Utility --> CRC
Utility --> FileLoader
Utility --> TimeConverter

Encoder --> UDP
UDP --> Decoder
```

------------------------------------------------------------------------

# ⚙ Session Management

``` mermaid
flowchart LR

SessionID --> Packet

ETCSID --> Packet

Packet --> Routing

Routing --> Tab1

Routing --> Tab2
```

------------------------------------------------------------------------

# 🔐 Packet Encoding

``` mermaid
flowchart TD

Input --> Header

Header --> Payload

Payload --> CRC

CRC --> BinaryPacket

BinaryPacket --> UDP
```

------------------------------------------------------------------------

# 📥 Packet Decoding

``` mermaid
flowchart TD

BinaryPacket --> HeaderCheck

HeaderCheck --> CRCCheck

CRCCheck --> ParseFields

ParseFields --> PropertyGrid

PropertyGrid --> User
```

------------------------------------------------------------------------

# 📁 Configuration

프로젝트는 여러 INI 파일을 이용하여 Packet 기본값을 구성합니다.

-   config_HSAck.ini
-   config_JP.ini
-   config_SP.ini

------------------------------------------------------------------------

# 🛠 Technology Stack

-   C++
-   Microsoft Foundation Class (MFC)
-   UDP Socket
-   Binary Protocol
-   CRC
-   Bit Manipulation
-   jsoncpp
-   Visual Studio

------------------------------------------------------------------------

# ⭐ Technical Highlights

-   UDP 기반 실시간 통신
-   Binary Packet Encoding / Decoding
-   Property Grid 기반 Packet 편집
-   Session Routing
-   CRC Validation
-   INI 기반 시험 환경
-   철도 통신 프로토콜 검증
-   GUI 기반 Simulator

------------------------------------------------------------------------

# 🎯 Development Purpose

본 프로젝트는 실제 철도 차량과 지상장치 간 통신을 대체하는 시험용
Simulator입니다.

실제 장비 없이 다양한 Packet을 생성하고 송수신하며 통신 프로토콜을
검증할 수 있도록 개발되었습니다.
