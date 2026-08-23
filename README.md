# ⚡ INDUsense AI

### Predictive Maintenance, Protection & Interactive Local AI for Industrial Assets

<p align="center">
  <b>Arduino UNO Q • INA219 • Real-Time Protection • Digital Twin • Gemma 3 • Edge AI</b>
</p>

---

## 🚀 What is INDUsense AI?

**INDUsense AI** is a low-cost **Physical AI platform** designed to monitor, protect, and explain the behavior of industrial assets.

The current prototype monitors a **24 V DC motor** and integrates:

* ⚡ Live voltage monitoring
* 🔌 Current measurement
* 📊 Real-time power calculation
* 🛡️ Automatic relay-based protection
* 🖥️ Live web dashboard
* 🤖 Local Gemma AI assistant
* 💬 Technician–machine interaction
* 🧠 Edge intelligence with reduced cloud dependency

### 🎯 Long-Term Goal

The long-term vision is to expand the architecture to multiple industrial assets and create an **intelligent maintenance assistant for technicians**.

---

# 🎯 The Problem

Modern industries contain large numbers of machines, making continuous manual inspection difficult, expensive, and time-consuming.

### Existing challenges

* 🏭 Continuous monitoring of multiple machines is difficult.
* ⚠️ Conventional monitoring systems often raise alarms without explaining the likely cause.
* 📚 Technicians may need to search through manuals, datasheets, and technical documents to troubleshoot faults.
* 💬 Most monitoring systems cannot directly interact with technicians.
* ☁️ Heavy cloud dependency can introduce latency, connectivity, and operational costs.

### 💡 The Key Question

> **What if a machine could monitor itself, protect itself, and explain its condition directly to the technician?**

---

# 💡 Our Solution

INDUsense AI combines **embedded sensing, real-time protection, visualization, and local AI** into a single architecture.

The system:

1. Continuously monitors important motor parameters.
2. Detects abnormal electrical conditions.
3. Protects the motor through a relay.
4. Displays live operating conditions on a dashboard.
5. Explains abnormal conditions in simple language.
6. Allows technicians to directly ask questions.
7. Reduces dependence on cloud-based AI systems.

### 💬 Example Technician Queries

```text
Why did the motor stop?

What is the current voltage?

Is the motor operating normally?

What is the relay state?

What is the current power consumption?
```

---

# 🧠 Why Arduino UNO Q?

The **Arduino UNO Q** enables the system to combine real-time embedded control with Linux-based application computing on one platform.

### 1. ⚙️ Real-Time Control

The MCU side independently:

* Reads the INA219 sensor
* Processes measurements
* Executes protection logic
* Controls the relay

### 2. 🐍 Local Application Computing

The Linux/application side runs:

* Python
* Web dashboard
* RouterBridge
* Gemma AI
* Telemetry processing

### 3. 🤖 One-Board Physical AI

```text
Sensing
   ↓
Processing
   ↓
Decision
   ↓
Physical Action
   ↓
Visualization
   ↓
AI Explanation
```

---

# 🏗️ System Architecture

```text
                24 V DC Supply
                      │
                      ▼
                   INA219
              Voltage + Current
                      │
                      ▼
               Arduino UNO Q
                MCU Control
              ┌───────┴────────┐
              │                │
              ▼                ▼
            Relay         RouterBridge
              │                │
              ▼                ▼
            Motor            Python
                               │
                       ┌───────┴────────┐
                       ▼                ▼
                   Dashboard        Gemma AI
```

---

# 🔧 Hardware

| Component                | Purpose                        |
| ------------------------ | ------------------------------ |
| **Arduino UNO Q**        | Central Physical AI platform   |
| **INA219**               | Voltage and current monitoring |
| **Relay Module**         | Motor protection and switching |
| **24 V DC Motor**        | Monitored industrial asset     |
| **24 V DC Power Supply** | Motor power source             |
| **Jumper Wires**         | Electrical interconnection     |
| **Web Browser**          | Dashboard and AI interaction   |

---

# 🔌 Pin Connections

| Signal         | Arduino UNO Q |
| -------------- | ------------- |
| INA219 SDA     | A4            |
| INA219 SCL     | A5            |
| Relay Input    | D6            |
| Serial Monitor | 9600 baud     |

### Relay Logic

```text
D6 LOW
   ↓
Relay ON
   ↓
Protection Active


D6 HIGH
   ↓
Relay OFF
   ↓
Motor Allowed to Run
```

> **Note:** The project uses **Wire2** for INA219 communication.

---

# 📐 Measurement Logic

### Voltage

The supply voltage is calculated using the INA219 bus and shunt measurements:

```text
V_supply = V_bus + (V_shunt_mV / 1000)
```

### Current

```text
I_A = I_mA / 1000
```

### Power

```text
P = V × I
```

### Noise Reduction

To reduce short-term fluctuations, the system averages 10 INA219 samples:

```text
V_avg = (V1 + V2 + ... + V10) / 10
```

This helps reduce short spikes and provides a more stable measurement for monitoring and visualization.

---

# ⚙️ MCU Control Flow

```text
                         START
                           │
                           ▼
                    Initialize UNO Q
                           │
              ┌────────────┼────────────┐
              │            │            │
              ▼            ▼            ▼
           Relay D6     LED Matrix    RGB LED
                           │
                           ▼
                         Wire2
                           │
                           ▼
                         INA219
                           │
                           ▼
                Read Voltage + Current
                           │
                           ▼
                    Average Samples
                           │
                           ▼
                    Calculate Power
                           │
                           ▼
             Compare With Protection Limit
                       ┌───┴───┐
                       │       │
                    Normal    Fault
                       │       │
                       ▼       ▼
                  Relay OFF  Relay ON
                       │       │
                       ▼       ▼
                Motor Running  Motor Protected
                       │       │
                       └───┬───┘
                           ▼
                 Update Display
                           │
                           ▼
                  Update Dashboard
                           │
                           ▼
              Send Telemetry via RouterBridge
                           │
                           ▼
                         LOOP
```

---

# 📡 Telemetry Architecture

The MCU sends a compact telemetry packet:

```text
voltage,current,power,relay,ready,settling,D6,condition
```

### Telemetry Fields

| Field | Parameter               |
| ----: | ----------------------- |
|     1 | Voltage                 |
|     2 | Current                 |
|     3 | Power                   |
|     4 | Relay state             |
|     5 | Sensor/controller ready |
|     6 | Settling state          |
|     7 | D6 output               |
|     8 | Motor condition         |

The compact telemetry structure minimizes communication overhead while providing the application layer with the information required for monitoring and AI interaction.

---

# 🐍 Python + Web UI

Python acts as the bridge between the embedded system, dashboard, and local AI.

### Python Responsibilities

* 🔄 RouterBridge communication
* 📡 Telemetry parsing
* 📊 Live dashboard updates
* ⚡ Fast local answers
* 🤖 Gemma request handling
* 💬 AI response streaming

### Dashboard Update Rate

```text
Update interval = 0.5 seconds

≈ 2 updates per second
```

This provides a responsive real-time visualization of the motor's operating state.

---

# 🤖 Smart AI Request Strategy

Not every question needs to be processed by the AI model.

INDUsense AI uses a simple routing strategy.

```text
                    User Question
                         │
                         ▼
              Can Python answer directly?
                    ┌────┴────┐
                   YES        NO
                    │          │
                    ▼          ▼
              Fast Answer   Gemma 3
                             Local AI
                                │
                                ▼
                           Explanation
```

### ⚡ Simple Questions

Python responds directly to questions such as:

```text
"What is the voltage?"

"What is the current?"

"What is the power?"

"What is the relay state?"
```

### 🧠 Complex Questions

Gemma handles questions requiring interpretation or explanation:

```text
"Why did the motor behave abnormally?"

"What could have caused the motor to stop?"

"Explain the current motor condition."
```

This reduces unnecessary AI inference while keeping the interaction responsive.

---

# 🛡️ Safety Architecture

A key design principle of INDUsense AI is:

> **The AI does not control the motor protection system.**

The MCU remains the **safety authority**.

```text
              INA219
                 │
                 ▼
                MCU
                 │
          Protection Logic
                 │
                 ▼
               Relay
                 │
                 ▼
               Motor


              Gemma AI
                 │
                 ▼
            Explanation
                 │
                 ▼
             Technician
```

### Safety Principle

Even if the AI model is unavailable, the embedded protection mechanism continues to operate independently.

This separates:

**Safety-critical control**

from

**AI-based explanation.**

---

# 🖥️ Digital Twin Dashboard

The dashboard provides a digital representation of the physical motor and its current operating state.

### Live Parameters

* ⚡ Voltage
* 🔌 Current
* 🔋 Power
* 🛡️ Relay state
* 📍 D6 output
* ⚙️ Motor condition
* 💡 Controller state
* 🤖 Gemma AI assistant

### Interface Features

* 🎬 Cinematic industrial motor introduction
* ⚙️ Animated motor graphics
* 📊 Live motor-state visualization
* 📱 Responsive desktop/mobile layout
* 💬 Integrated AI chat

---

# 💬 Interactive Maintenance Assistant

Traditional monitoring systems generally follow:

```text
Machine
   ↓
Alarm
   ↓
Technician
   ↓
Manual / Documentation
   ↓
Diagnosis
```

INDUsense AI introduces an interactive approach:

```text
Machine
   │
   ▼
Live Monitoring
   │
   ▼
Fault / Condition
   │
   ▼
Local AI
   │
   ▼
Technician asks:
"What happened?"
   │
   ▼
Simple Explanation
```

The objective is to turn machine data into information that is easier for technicians to understand and act upon.

---

# ✨ What Makes INDUsense AI Different?

### 🔹 Hybrid Edge Architecture

Combines real-time MCU control with Linux-based local application computing.

### 🔹 One-Board Physical AI

Sensing, processing, decision-making, physical action, visualization, and AI interaction are integrated into one platform.

### 🔹 Interactive Asset Monitoring

The technician can interact directly with the machine's live data.

### 🔹 Digital Twin Visualization

The physical motor is represented through a live digital interface.

### 🔹 Reduced Cloud Dependency

Local processing and AI reduce dependence on continuous cloud connectivity.

### 🔹 Explainable Protection

The system combines automatic protection with human-readable explanations.

### 🔹 Compact Telemetry

A lightweight telemetry packet efficiently transfers the required operating information.

### 🔹 Multi-Level Feedback

The system provides feedback through multiple interfaces:

```text
Serial Monitor
      +
LED Matrix
      +
RGB LED
      +
Web Dashboard
      +
Gemma AI
```

---

# 🧩 Physical AI Mapping

| Physical AI Stage | INDUsense AI                     |
| ----------------- | -------------------------------- |
| 👁️ **Sense**     | INA219                           |
| 🧠 **Process**    | Arduino UNO Q MCU                |
| ⚖️ **Decide**     | Protection logic                 |
| ⚙️ **Act**        | Relay                            |
| 📊 **Visualize**  | LED Matrix + RGB LED + Dashboard |
| 💬 **Explain**    | Gemma AI                         |

---

# 🧪 Current Prototype

### Current Monitored Asset

**24 V DC Motor**

The prototype establishes the core architecture for monitoring and protecting an electrical asset while enabling local AI-based interaction.

### 🔮 Potential Future Assets

```text
Motors
Pumps
Compressors
Conveyors
Fans
Industrial Drives
Production Equipment
```

---

# 🔮 Future Scope

The platform can be extended with additional sensing and intelligence capabilities.

### 📈 Predictive Maintenance

* Edge Impulse anomaly detection
* Predictive-maintenance scoring
* Long-term trend analysis
* Event and fault logging

### 📡 Additional Sensors

* 📳 Vibration monitoring
* 🌡️ Motor temperature sensing
* 🔄 Real RPM measurement

### 🤖 Intelligent Assistance

* 🔔 Maintenance notifications
* 🎙️ Voice-based assistant
* 📄 Automatic maintenance reports
* 🧠 Asset-specific AI models

### 🏭 Multi-Asset Monitoring

Expand from a single motor to an entire industrial environment.

---

# 🌍 Our Vision

### Today

```text
                    1 Motor
                       │
                       ▼
                 INDUsense AI
                       │
                       ▼
              Interactive Technician
                   Assistant
```

### Tomorrow

```text
                    1 Factory
                       │
        ┌──────────────┼──────────────┐
        │              │              │
        ▼              ▼              ▼
     Motor 1         Motor 2         Pump
        │              │              │
        └──────────────┼──────────────┘
                       │
                       ▼
                  Compressor
                       │
                       ▼
                   Conveyor
                       │
                       ▼
              Production Assets
                       │
                       ▼
                 INDUsense AI
                       │
                       ▼
            Interactive Technician
                  Assistant
```

The ultimate goal is to build a scalable intelligent asset-monitoring platform capable of supporting multiple machines across an industrial environment.

---

# 👥 Team NitteNova

**Institution:**
Nitte Meenakshi Institute of Technology, Bengaluru

### Team Members

* **Dr. Singaravelan A**
* **Keerthana Bhat**
* **Manas Reddy**
* **Sai Vikas PS**

---

# 🏆 Arduino Physical AI Challenge 2026

INDUsense AI demonstrates the integration of:

* ⚡ Physical sensing
* 🔧 Embedded intelligence
* ⚙️ Real-time actuation
* 🧠 Edge AI
* 💬 Human–machine interaction
* 🔍 Local AI explanation
* 🛡️ Real-time protection

---

# 🎥 Demo

▶️ **Watch the Demo**

*Add your demo video/link here.*

---

# 🔗 Repository

**INDUsense AI Repository**

*Add your GitHub repository link here.*

---

# 📁 Project Structure

```text
INDUsense-AI/
│
├── sketch/
│   └── sketch.ino
│
├── python/
│   └── main.py
│
├── assets/
│   ├── index.html
│   ├── style.css
│   ├── app.js
│   └── libs/
│
├── app.yaml
│
└── README.md
```

---

# ⚠️ Safety Note

**INDUsense AI is currently a research and competition prototype.**

For industrial deployment, additional safety and protection mechanisms would be required, including:

* Electrical isolation
* Correct relay/contact ratings
* Appropriate fusing
* Industrial-grade enclosure
* Emergency-stop system
* EMI protection
* Certified motor-protection hardware
* Appropriate electrical safety standards and testing

The prototype should **not be treated as a certified industrial protection system**.

---

# ⭐ Final Vision

INDUsense AI aims to transform industrial monitoring from simple **fault detection** into **intelligent, interactive, and explainable asset maintenance**.

```text
┌─────────────────────────────────────────┐
│                                         │
│       SENSE • PROTECT • EXPLAIN         │
│               • INTERACT                │
│                                         │
└─────────────────────────────────────────┘
```

<p align="center">
  <b>Built with ❤️ using Arduino UNO Q</b>
</p>

<p align="center">
  <i>INDUsense AI — Making industrial assets more observable, protected, and interactive.</i>
</p>
