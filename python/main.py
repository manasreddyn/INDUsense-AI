from arduino.app_utils import App, Bridge
from arduino.app_bricks.web_ui import WebUI
from arduino.app_bricks.llm import LargeLanguageModel

import time
import threading


# ============================================================
# SETTINGS
# ============================================================

WEB_PORT = 7000
MAX_HISTORY = 3


# ============================================================
# WEB UI
# ============================================================

ui = WebUI(
    addr="0.0.0.0",
    port=WEB_PORT
)


# ============================================================
# GEMMA
# ============================================================

llm = LargeLanguageModel(
    system_prompt="""
Motor monitor.

V > 20.5 V = NORMAL.
V <= 20.5 V = UNDERVOLTAGE.

MCU controls the relay.
You only monitor and explain.

Use supplied measurements only.
Do not invent values.

Reply in 1 or 2 short sentences.
"""
)


# ============================================================
# GEMMA LOCK
# ============================================================

gemma_lock = threading.Lock()


# ============================================================
# LIVE VALUES
# ============================================================

latest = {
    "voltage": 0.0,
    "current": 0.0,
    "power": 0.0,

    "relay": False,
    "armed": False,
    "settling": False,

    "pin7": 1,
    "condition": 0
}


# ============================================================
# SMALL RAM HISTORY
# ============================================================

recent_readings = []


# ============================================================
# WEB CONNECTION
# ============================================================

def on_connect(client):
    print(
        "Web client connected:",
        client
    )


def on_disconnect(client):
    print(
        "Web client disconnected:",
        client
    )


ui.on_connect(
    on_connect
)

ui.on_disconnect(
    on_disconnect
)


# ============================================================
# CONDITION TEXT
# ============================================================

def condition_text():

    if latest["condition"] == 1:
        return "NORMAL"

    if latest["condition"] == 2:
        return "OVERVOLTAGE"

    return "WAITING"


# ============================================================
# READ ONE TELEMETRY PACKET
# ============================================================

def read_mcu():

    try:

        packet = Bridge.call(
            "get_telemetry"
        )

        packet = str(
            packet
        ).strip()

        parts = packet.split(
            ","
        )

        if len(parts) != 8:

            print(
                "Invalid telemetry packet:",
                packet
            )

            return False


        latest["voltage"] = float(
            parts[0]
        )

        latest["current"] = float(
            parts[1]
        )

        latest["power"] = float(
            parts[2]
        )

        latest["relay"] = (
            parts[3] == "1"
        )

        latest["armed"] = (
            parts[4] == "1"
        )

        latest["settling"] = (
            parts[5] == "1"
        )

        latest["pin7"] = int(
            parts[6]
        )

        latest["condition"] = int(
            parts[7]
        )

        return True


    except Exception as error:

        print(
            "Telemetry error:",
            error
        )

        return False


# ============================================================
# UPDATE SMALL RAM HISTORY
# ============================================================

def update_history():

    recent_readings.append(
        {
            "voltage":
                latest["voltage"],

            "current":
                latest["current"]
        }
    )


    if len(recent_readings) > MAX_HISTORY:

        recent_readings.pop(
            0
        )


# ============================================================
# SEND DASHBOARD
# ============================================================

def send_telemetry():

    ui.send_message(
        "telemetry",
        {
            "voltage":
                latest["voltage"],

            "current":
                latest["current"],

            "power":
                latest["power"],

            "relay":
                latest["relay"],

            "armed":
                latest["armed"],

            "settling":
                latest["settling"],

            "pin7":
                latest["pin7"],

            "condition":
                condition_text()
        }
    )


# ============================================================
# FAST LOCAL ANSWERS
# ============================================================

def fast_answer(question):

    q = question.lower().strip()


    # --------------------------------------------------------
    # MOTOR STATUS
    # --------------------------------------------------------

    if (
        "how is the motor" in q
        or "motor status" in q
        or "motor condition" in q
        or "is the motor normal" in q
        or "working normally" in q
    ):

        if latest["condition"] == 1:

            return (
                f"Motor is operating normally. "
                f"Voltage is "
                f"{latest['voltage']:.2f} V."
            )

        else:

            return (
                f"Motor is in overvoltage protection. "
                f"Voltage is "
                f"{latest['voltage']:.2f} V and "
                f"relay is "
                f"{'ON' if latest['relay'] else 'OFF'}."
            )


    # --------------------------------------------------------
    # VOLTAGE
    # --------------------------------------------------------

    if (
        "voltage" in q
        and "why" not in q
        and "drop" not in q
        and "fall" not in q
        and "trend" not in q
    ):

        return (
            f"Supply voltage is "
            f"{latest['voltage']:.2f} V."
        )


    # --------------------------------------------------------
    # CURRENT
    # --------------------------------------------------------

    if (
        "current" in q
        and "why" not in q
    ):

        return (
            f"Motor current is "
            f"{latest['current']:.3f} A."
        )


    # --------------------------------------------------------
    # POWER
    # --------------------------------------------------------

    if (
        "power" in q
        and "why" not in q
    ):

        return (
            f"Motor power is "
            f"{latest['power']:.2f} W."
        )


    # --------------------------------------------------------
    # RELAY
    # --------------------------------------------------------

    if (
        "relay" in q
        and "why" not in q
    ):

        return (
            f"The relay is currently "
            f"{'ON' if latest['relay'] else 'OFF'}."
        )


    # --------------------------------------------------------
    # D7
    # --------------------------------------------------------

    if "d7" in q:

        return (
            f"Pin D7 is currently "
            f"{'LOW' if latest['pin7'] == 0 else 'HIGH'}."
        )


    return None


# ============================================================
# SHORT GEMMA PROMPT
# ============================================================

def build_prompt(question):

    history = ""

    for item in recent_readings:

        history += (
            f"{item['voltage']:.2f}V/"
            f"{item['current']:.3f}A "
        )


    return f"""
Rule:
>20.5V=NORMAL
<=20.5V=UNDERVOLTAGE

Live:
V={latest["voltage"]:.2f}V
I={latest["current"]:.3f}A
P={latest["power"]:.2f}W
R={"ON" if latest["relay"] else "OFF"}
S={condition_text()}

Recent:
{history}

Question:
{question}

Reply briefly.
"""


# ============================================================
# GEMMA WARM-UP
# ============================================================

def warm_up_gemma():

    print()
    print(
        "Warming up Gemma..."
    )

    start_time = time.time()


    try:

        # Very small inference.
        # Purpose: load/initialize model before user asks.

        response = llm.chat(
            "Reply only: READY"
        )


        elapsed = (
            time.time()
            -
            start_time
        )


        print(
            "Gemma warm-up complete"
        )

        print(
            "Warm-up response:",
            response
        )

        print(
            f"Warm-up time: "
            f"{elapsed:.2f} seconds"
        )


    except Exception as error:

        print(
            "Gemma warm-up error:",
            error
        )


    print()


# ============================================================
# GEMMA STREAMING
# ============================================================

def run_gemma(question):

    # --------------------------------------------------------
    # ALLOW ONLY ONE GEMMA REQUEST
    # --------------------------------------------------------

    if not gemma_lock.acquire(
        blocking=False
    ):

        ui.send_message(
            "chat_answer",
            {
                "answer":
                    "Gemma is already answering another question."
            }
        )

        return


    try:

        # Latest MCU data immediately before inference

        read_mcu()


        prompt = build_prompt(
            question
        )


        ui.send_message(
            "chat_stream_start",
            {}
        )


        # ----------------------------------------------------
        # STREAM RESPONSE
        # ----------------------------------------------------

        for chunk in llm.chat_stream(
            prompt
        ):

            if chunk:

                ui.send_message(
                    "chat_chunk",
                    {
                        "chunk":
                            str(chunk)
                    }
                )


        ui.send_message(
            "chat_stream_end",
            {}
        )


    except Exception as error:

        print(
            "Gemma error:",
            error
        )


        ui.send_message(
            "chat_error",
            {
                "error":
                    str(error)
            }
        )


    finally:

        gemma_lock.release()


# ============================================================
# CHAT
# ============================================================

def on_chat(
    client,
    data
):

    question = str(
        data.get(
            "message",
            ""
        )
    ).strip()


    if not question:
        return


    # --------------------------------------------------------
    # GET CURRENT MCU DATA
    # --------------------------------------------------------

    if not read_mcu():

        ui.send_message(
            "chat_answer",
            {
                "answer":
                    "Unable to read motor telemetry."
            }
        )

        return


    # --------------------------------------------------------
    # TRY FAST ANSWER FIRST
    # --------------------------------------------------------

    answer = fast_answer(
        question
    )


    if answer is not None:

        ui.send_message(
            "chat_answer",
            {
                "answer":
                    answer
            }
        )

        return


    # --------------------------------------------------------
    # USE GEMMA
    # --------------------------------------------------------

    ui.send_message(
        "chat_status",
        {
            "status":
                "Gemma is thinking..."
        }
    )


    thread = threading.Thread(
        target=run_gemma,
        args=(question,),
        daemon=True
    )


    thread.start()


ui.on_message(
    "chat",
    on_chat
)


# ============================================================
# MAIN LOOP
# ============================================================

def loop():

    if read_mcu():

        update_history()

        send_telemetry()


    # Two dashboard updates per second

    time.sleep(
        0.5
    )


# ============================================================
# STARTUP
# ============================================================

print()
print(
    "======================================"
)

print(
    " FAST GEMMA MOTOR MONITOR"
)

print(
    "======================================"
)

print()

print(
    "Bridge telemetry: ONE RPC"
)

print(
    "Database: OFF"
)

print(
    "Continuous Gemma: OFF"
)

print(
    "Fast local answers: ON"
)

print(
    "Gemma streaming: ON"
)

print(
    "Gemma warm-up: ON"
)

print()


# ============================================================
# WARM GEMMA BEFORE WEB APP LOOP
# ============================================================

warm_up_gemma()


print(
    "Gemma ready"
)

print(
    "WebUI port:",
    WEB_PORT
)

print()


# ============================================================
# RUN APP
# ============================================================

App.run(
    user_loop=loop
)