⚡ INDUsense AI

Predictive Maintenance, Protection & Interactive Local AI for Industrial Assets

<p align="center">
  <b>Arduino UNO Q • INA219 • Real-Time Protection • Digital Twin • Gemma 3 • Edge AI</b>
</p>

<p align="center">


🚀 What is INDUsense AI?

INDUsense AI is a low-cost Physical AI platform for monitoring, protecting and explaining the behavior of industrial assets.

The current prototype monitors a 24 V DC motor and combines:

⚡ Live voltage monitoring

🔌 Current measurement

📊 Real-time power calculation

🛡️ Automatic relay-based protection

🖥️ Live web dashboard

🤖 Local Gemma AI assistant

💬 Technician-machine interaction

🧠 Edge intelligence with reduced cloud dependency

The long-term goal is to expand this architecture to multiple industrial assets and create an intelligent maintenance assistant for technicians.

🎯 The Problem

🏭 Industries contain many machines, making continuous manual inspection difficult and costly.

⚠️ Most monitoring systems only raise alarms instead of clearly explaining the likely problem.

📚 Technicians often need to search large manuals, datasheets or PDFs to troubleshoot a fault.

💬 Existing monitoring systems usually cannot directly interact with a technician.

☁️ Heavy cloud dependency can introduce cost, latency and connectivity limitations.

What if a machine could monitor itself, protect itself and explain its condition directly to the technician?

💡 Our Solution

Continuously monitor important motor parameters.

Detect abnormal electrical conditions.

Protect the motor through a relay.

Display live condition on a dashboard.

Explain faults in simple language.

Allow technicians to directly ask questions.

Reduce dependence on cloud-based AI systems.

Example questions:

Why did the motor stop?
What is the current voltage?
Is the motor operating normally?
What is the relay state?
What is the current power consumption?

🧠 Why Arduino UNO Q?

1. Real-Time Control

The MCU side reads the INA219 sensor and controls the relay independently from the AI.

2. Local Application Computing

The Linux/application side runs Python, the web dashboard, RouterBridge and Gemma AI.

3. One-Board Physical AI

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

🏗️ System Architecture

24 V DC Supply
      │
      ▼
   INA219
Voltage + Current
      │
      ▼
 Arduino UNO Q
   MCU Control
  ┌────┴─────┐
  │          │
  ▼          ▼
Relay      RouterBridge
  │          │
  ▼          ▼
Motor      Python
             │
      ┌──────┴──────┐
      ▼             ▼
Dashboard        Gemma AI

🔧 Hardware Used

Component

Purpose

Arduino UNO Q

Central Physical AI platform

INA219

Voltage and current monitoring

Relay Module

Motor protection / switching

24 V DC Motor

Monitored industrial asset

24 V DC Power Supply

Motor power source

Jumper Wires

Electrical interconnection

Web Browser

Dashboard and AI interaction

🔌 Pin Connections

Signal

Arduino UNO Q

INA219 SDA

A4

INA219 SCL

A5

Relay Input

D6

Serial Monitor

9600 baud

Relay Logic

D6 LOW  → Relay ON  → Protection Active
D6 HIGH → Relay OFF → Motor Allowed to Run

The project uses Wire2 for INA219 communication.

📐 Measurement Logic

Supply Voltage

V_supply = V_bus + (V_shunt_mV / 1000)

Current

I_A = I_mA / 1000

Power

P = V × I

Noise Reduction

V_avg = (V1 + V2 + ... + V10) / 10

The system averages 10 INA219 samples to reduce short spikes and improve stability.

⚙️ MCU Control Flow

START
  │
  ▼
Initialize UNO Q
  │
  ├── Relay D6
  ├── LED Matrix
  ├── RGB LED
  ├── Wire2
  └── INA219
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
  │
  ├── Normal → Relay OFF → Motor Running
  │
  └── Fault  → Relay ON  → Motor Protected
  │
  ▼
Update Display + Dashboard
  │
  ▼
Send Telemetry through RouterBridge
  │
  └──────── LOOP

📡 Telemetry Architecture

The MCU sends one compact telemetry packet:

voltage,current,power,relay,ready,settling,D6,condition

Field

Value

1

Voltage

2

Current

3

Power

4

Relay state

5

Sensor/controller ready

6

Settling state

7

D6 output

8

Motor condition

🐍 Python + WebUI

Python handles:

RouterBridge communication

Telemetry parsing

Live dashboard updates

Fast local answers

Gemma request handling

AI response streaming

Dashboard update interval:

0.5 seconds

That gives about:

2 updates per second

🤖 Smart AI Request Strategy

Simple question:

"What is the voltage?"

Python answers immediately.

Complex question:

"Why did the motor behave abnormally?"

Gemma handles the explanation.

User Question
      │
      ▼
Can Python answer directly?
      │
   ┌──┴──┐
  YES    NO
   │      │
   ▼      ▼
Fast     Gemma 3
Answer   Local AI
           │
           ▼
      Explanation

🛡️ Safety Architecture

AI does not control the motor protection relay.

The MCU makes the protection decision.

INA219 → MCU → Relay → Motor
          │
          └── Safety Authority

Gemma AI → Explanation Only → Technician

This ensures protection still works even if the AI model is unavailable.

🖥️ Digital Twin Dashboard

The dashboard displays:

⚡ Voltage

🔌 Current

🔋 Power

🛡️ Relay state

📍 D6 output

⚙️ Motor condition

💡 Controller state

🤖 Gemma assistant

The interface also includes:

Cinematic industrial motor introduction

Animated motor graphics

Live motor-state visualization

Responsive desktop/mobile layout

Integrated AI chat

💬 Interactive Maintenance Assistant

Traditional monitoring:

Machine → Alarm → Technician → Manual → Diagnosis

INDUsense AI:

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

✨ What Makes INDUsense AI Unique?

🔹 Hybrid edge architecture

🔹 One-board Physical AI

🔹 Interactive asset monitoring

🔹 Digital-twin visualization

🔹 Reduced cloud dependency

🔹 Explainable protection

🔹 Compact telemetry

🔹 Multi-level feedback

Serial Monitor
     +
LED Matrix
     +
RGB LED
     +
Web Dashboard
     +
Gemma AI

🧩 Physical AI Mapping

Physical AI Stage

INDUsense AI

Sense

INA219

Process

Arduino UNO Q MCU

Decide

Protection logic

Act

Relay

Visualize

LED Matrix + RGB + Dashboard

Explain

Gemma AI

🧪 Current Prototype

Currently monitored asset:

24 V DC Motor

Future expansion:

Motors
Pumps
Compressors
Conveyors
Fans
Industrial Drives
Production Equipment

🔮 Future Scope

📈 Edge Impulse anomaly detection

📳 Vibration monitoring

🌡️ Motor temperature sensing

🔄 Real RPM measurement

📊 Predictive-maintenance scoring

🗃️ Event and fault logging

📉 Long-term trend analysis

🔔 Maintenance notifications

🎙️ Voice assistant

📄 Automatic maintenance reports

🏭 Multi-asset monitoring

🧠 Asset-specific AI models

🌍 Our Vision

Today:
1 Motor

Tomorrow:
1 Factory
   │
   ├── Motor 1
   ├── Motor 2
   ├── Pump
   ├── Compressor
   ├── Conveyor
   └── Production Asset
          │
          ▼
      INDUsense AI
          │
          ▼
   Interactive Technician
        Assistant

👥 Team NitteNova

Institution: Nitte Meenakshi Institute of Technology

Team Members

Dr. Singaravelan A

Keerthana Bhat

Manas Reddy

Sai Vikas PS

🏆 Arduino Physical AI Challenge 2026

This project demonstrates:

Physical sensing

Embedded intelligence

Real-time actuation

Edge AI

Human-machine interaction

Local AI explanation

🎥 Demo

▶️ Watch the Demo

🔗 Repository

INDUsense AI Repository

📁 Project Structure

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

⚠️ Safety Note

This project is a research and competition prototype.

For industrial deployment, additional protection should include:

Electrical isolation

Correct relay/contact ratings

Fusing

Industrial enclosure

Emergency-stop system

EMI protection

Certified motor protection hardware

⭐ Final Vision

INDUsense AI transforms industrial monitoring from simple fault detection into intelligent, interactive and explainable asset maintenance.

<p align="center">
  <b>Sense • Protect • Explain • Interact</b>
</p>

<p align="center">
  Built with ❤️ using Arduino UNO Q
</p>**
