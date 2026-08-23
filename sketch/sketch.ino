#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Arduino_RouterBridge.h>
#include <Arduino_LED_Matrix.h>
Adafruit_INA219 ina219;

Arduino_LED_Matrix matrix;
const int RELAY_PIN = 6;
// Actual protection threshold
const float VOLTAGE_THRESHOLD = 24.0;
// Visual warning threshold
const float WARNING_VOLTAGE = 23.0;
const int SAMPLE_COUNT = 10; //INA219
bool inaReady = false;
float measuredVoltage = 0.0; //Live measurements
float measuredCurrent = 0.0;
float measuredPower = 0.0;
bool relayState = false; //relay state
uint8_t voltageFrame[104]; // LED matrix
// 3 x 5 DIGITS
// Each digit is 3 columns x 5 rows
const uint8_t DIGITS[10][5][3] =
{
    {
        {1,1,1},
        {1,0,1},
        {1,0,1},
        {1,0,1},
        {1,1,1}
    },
    {
        {0,1,0},
        {1,1,0},
        {0,1,0},
        {0,1,0},
        {1,1,1}
    },
    {
        {1,1,1},
        {0,0,1},
        {1,1,1},
        {1,0,0},
        {1,1,1}
    },
    {
        {1,1,1},
        {0,0,1},
        {1,1,1},
        {0,0,1},
        {1,1,1}
    },

    {
        {1,0,1},
        {1,0,1},
        {1,1,1},
        {0,0,1},
        {0,0,1}
    },

    {
        {1,1,1},
        {1,0,0},
        {1,1,1},
        {0,0,1},
        {1,1,1}
    },
    {
        {1,1,1},
        {1,0,0},
        {1,1,1},
        {1,0,1},
        {1,1,1}
    },
    {
        {1,1,1},
        {0,0,1},
        {0,1,0},
        {0,1,0},
        {0,1,0}
    },
    {
        {1,1,1},
        {1,0,1},
        {1,1,1},
        {1,0,1},
        {1,1,1}
    },

    {
        {1,1,1},
        {1,0,1},
        {1,1,1},
        {0,0,1},
        {1,1,1}
    }
};

// CLEAR MATRIX
void clearMatrix()
{
    for (int i = 0; i < 104; i++)
    {
        voltageFrame[i] = 0;
    }
}
// DRAW DIGIT
// x = starting column
// y = starting row
void drawDigit(int digit,int x,int y)
{
    if (digit < 0 || digit > 9)
    {
        return;
    }
    for (int row = 0; row < 5; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            int px = x + col;
            int py = y + row;
            if (px >= 0 &&px < 13 &&py >= 0 &&py < 8)
            {
                if (DIGITS[digit][row][col])
                {
                    voltageFrame[py * 13 + px] = 1;
                }
            }
        }
    }
}
// DRAW DECIMAL POINT
void drawDecimal(int x,int y)
{
    if (x >= 0 &&x < 13 &&y >= 0 &&y < 8 )
    {
        voltageFrame[y * 13 + x] = 1;
    }
}
// DISPLAY VOLTAGE
void displayVoltage(float voltage)
{
    clearMatrix();
    // Limit displayed voltage
    if (voltage < 0)
    {
        voltage = 0;
    }

    if (voltage > 99.9)
    {
        voltage = 99.9;
    }
    // Convert to one decimal place
    int value =(int)(voltage * 10.0+ 0.5);
    int tens =value / 100;
    int ones =(value / 10) % 10;
    int decimal =value % 10;
    drawDigit( tens,0,1);
    drawDigit(ones,4,1);
    drawDecimal(8,5);
    drawDigit(decimal,10,1);
    matrix.draw(voltageFrame);
}

// RGB LED 
// UNO Q RGB LEDs are active LOW.
// LOW  = ON
// HIGH = OFF
void rgbOff()
{
    digitalWrite(LED3_R,HIGH);
    digitalWrite(LED3_G,HIGH);
    digitalWrite(LED3_B,HIGH);
}
// RGB GREEN
// NORMAL
void rgbGreen()
{
    digitalWrite(LED3_R, HIGH);
    digitalWrite(LED3_G,LOW);
    digitalWrite( LED3_B, HIGH);
}
// RGB YELLOW
void rgbYellow()
{
    digitalWrite(LED3_R,LOW);
    digitalWrite(LED3_G,LOW);
    digitalWrite(LED3_B,HIGH);
}
// RGB RED
// OVERVOLTAGE
void rgbRed()
{
    digitalWrite(LED3_R,LOW);
    digitalWrite(LED3_G,HIGH);
    digitalWrite(LED3_B,HIGH);
}
// UPDATE RGB STATUS
void updateRGB()
{
    if (measuredVoltage>VOLTAGE_THRESHOLD)
    {
        rgbRed();
        return;
    }
    if (measuredVoltage>WARNING_VOLTAGE)
    {
        rgbYellow();
        return;
    }
    rgbGreen();
}
void relayON()
{
    if (relayState)
    {
        return;
    }


    digitalWrite(RELAY_PIN,LOW);
    relayState = true;
    Serial.println();
    Serial.println("===============================);
    Serial.println("OVERVOLTAGE DETECTED");
    Serial.println("RELAY ON");
    Serial.println("D6 = LOW");
    Serial.println("MOTOR STOPPED");
    Serial.println("==============================");
    Serial.println();
}

void relayOFF()
{
    digitalWrite(RELAY_PIN,HIGH);
    if (!relayState)
    {
        return;
    }
    relayState = false;
    Serial.println();
    Serial.println("==============================");
    Serial.println("VOLTAGE NORMAL");
    Serial.println("RELAY OFF");
    Serial.println("D6 = HIGH");
    Serial.println("MOTOR RUNNING");
    Serial.println("==============================");
    Serial.println();
}
void updateINA219()
{
    float voltageSum = 0.0;

    float currentSum = 0.0;


    for (int i = 0;i < SAMPLE_COUNT;i++)
    {
        float busVoltage =
            ina219.getBusVoltage_V();
        float shuntVoltage_mV =
            ina219.getShuntVoltage_mV();
        float currentA =
            ina219.getCurrent_mA()
            / 1000.0;
        float supplyVoltage =busVoltage+(shuntVoltage_mV/ 1000.0);
        voltageSum +=supplyVoltage;
        currentSum +=currentA;
        delay(5);
    }
    measuredVoltage = voltageSum/ SAMPLE_COUNT;
    measuredCurrent = currentSum/ SAMPLE_COUNT;
    measuredPower =measuredVoltage*measuredCurrent;
}
void updateRelay()
{
    if (measuredVoltage>VOLTAGE_THRESHOLD)
    {
        if (!relayState)
        {
            Serial.print("Voltage > 24.0 V: ");
            Serial.print(measuredVoltage, 2);
            Serial.println(" V");
            relayON();
        }
        return;
    }
    if (
        measuredVoltage
        <=
        VOLTAGE_THRESHOLD
    )
    {
        if (relayState)
        {
            Serial.print("Voltage <= 24.0 V: ");
            Serial.print(measuredVoltage,2);
            Serial.println(" V");
            relayOFF();
        }
        else
        {
            digitalWrite(RELAY_PIN,HIGH);
        }
        return;
    }
}
int getCondition()
{
    if (!inaReady)
    {
        return 0;
    }
    if (measuredVoltage>VOLTAGE_THRESHOLD)
    {
        return 2;
    }
    return 1;
}
// TELEMETRY
String getTelemetryRPC()
{
    String packet = "";
    // Voltage
    packet +=
        String(
            measuredVoltage,
            2
        );

     packet += ",";
    // Current
    packet += String(measuredCurrent,3);
    packet += ",";
    // Power
    packet += String(measuredPower,2);
    packet += ",";
    // Relay
    packet += relayState?"1":"0";
    packet += ",";
    // INA219 ready
    packet +=inaReady ? "1" : "0";
    packet += ",";
    // Settling
    packet += "0";
    packet += ",";
    // D6
    packet +=String(digitalRead(RELAY_PIN));
    packet += ",";
    // Condition
    packet += String(getCondition());
    return packet;
}
// SETUP
void setup()
{
    // SERIAL
    Serial.begin(9600);
    delay(2000);
    Serial.println();
    Serial.println("==============================");
    Serial.println("UNO Q MOTOR AI CONTROLLER");
    Serial.println("==============================");
    // RELAY
    pinMode(RELAY_PIN,OUTPUT);
    // Relay OFF initially
    digitalWrite(RELAY_PIN,HIGH);
    relayState = false;
    pinMode(LED3_R,OUTPUT);
    pinMode(LED3_G,OUTPUT);
    pinMode(LED3_B,OUTPUT);
    rgbOff();
    // LED MATRIX
    Serial.println("Starting LED matrix...");
    matrix.begin();
    matrix.setGrayscaleBits(1);
    // Display startup value
    displayVoltage(0.0);
    // I2C
    Serial.println("Starting Wire2...");
    Wire2.begin();
    delay(500);
    // INA219
    Serial.println("Checking INA219...");
    inaReady =ina219.begin(&Wire2);
    if (!inaReady)
    {
        Serial.println();

        Serial.println("==============================");
        Serial.println("INA219 NOT DETECTED");
        Serial.println("SDA = A4");
        Serial.println("SCL = A5");
        Serial.println("I2C = Wire2");
        Serial.println("==============================" );
        // Sensor unavailable
        digitalWrite(RELAY_PIN,HIGH);
        relayState = false;
        // Red indicates sensor problem
        rgbRed();
    }
    else
    {
        Serial.println();

        Serial.println("==============================");
        Serial.println("INA219 CONNECTED");
        Serial.println("SDA = A4");
        Serial.println("SCL = A5");
        Serial.println("I2C = Wire2");
        Serial.println("==============================");
        // INITIAL MEASUREMENT
        updateINA219();
        Serial.print("Initial voltage = ");
        Serial.print( measuredVoltage, 2);
        Serial.println(" V");
        // INITIAL LED DISPLAY
        displayVoltage(measuredVoltage);
        updateRGB();
        // INITIAL RELAY STATE
        if (measuredVoltage>VOLTAGE_THRESHOLD)
        {
            digitalWrite(RELAY_PIN,LOW);
            relayState = true;
            Serial.println("Initial state: OVERVOLTAGE");
            Serial.println("Relay ON / Motor STOPPED");
        }
        else
        {
            digitalWrite(RELAY_PIN, HIGH);
            relayState = false;
            Serial.println("Initial state: NORMAL");
            Serial.println("Relay OFF / Motor RUNNING");
        }
    }
    // ROUTER BRIDGE
    Serial.println("Starting Bridge...");
    Bridge.begin();
    Bridge.provide_safe("get_telemetry",getTelemetryRPC);
    Serial.println("Bridge ready");
    Serial.println();
    Serial.println("MCU READY");
    Serial.println();
}
// LOOP
void loop()
{
    // SENSOR NOT READY
    if (!inaReady)
    {
        // Keep relay OFF
        digitalWrite(RELAY_PIN,HIGH);
        relayState = false;
        // Red LED
        rgbRed();
        static unsigned long lastRetry = 0;
        if (millis()-lastRetry>=2000)
        {
            lastRetry =millis();
            Serial.println("Retrying INA219...");
            inaReady = ina219.begin(&Wire2);
            if (inaReady)
            {
                Serial.println("INA219 CONNECTED");
            }
            else
            {
                Serial.println("INA219 NOT DETECTED");
            }
        }
        delay(10);
        return;
    }
    updateINA219();
    updateRelay();
    // UPDATE MATRIX
    // Update every 250 ms
    static unsigned long lastMatrixUpdate = 0;
    if (millis()-lastMatrixUpdate>=250)
    {
        lastMatrixUpdate =millis();
        displayVoltage(measuredVoltage);
        updateRGB();
    }
    static unsigned long lastPrint = 0;
    if (millis()-lastPrint>=500)
    {
        lastPrint =millis();
        Serial.print("V=");
        Serial.print(measuredVoltage,2);
        Serial.print(" V | I=" );
        Serial.print( measuredCurrent, 3);
        Serial.print(" A | P=");
        Serial.print( measuredPower,2);
        Serial.print(" W | Relay=");
        Serial.print(relayState?"ON":"OFF");
        Serial.print(" | D6=");
        Serial.print(digitalRead(RELAY_PIN)==LOW?"LOW":"HIGH");
        Serial.print(" | Motor=");
        if (relayState)
        {
            Serial.print( "STOPPED");
        }
        else
        {
            Serial.print("RUNNING");
        }
        Serial.print(" | Condition=");
        if (measuredVoltage>VOLTAGE_THRESHOLD)
        {
            Serial.println("OVERVOLTAGE");
        }
        else if ( measuredVoltage>WARNING_VOLTAGE)
        {
            Serial.println("WARNING");
        }
        else
        {
            Serial.println("NORMAL");
        }
    }
}
