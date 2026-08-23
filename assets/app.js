// =====================================================
// CINEMATIC MOTOR INTRO
// =====================================================

const motorIntro =
    document.getElementById("motorIntro");

const dashboard =
    document.getElementById("dashboard");

const skipIntro =
    document.getElementById("skipIntro");

const dashboardMotor =
    document.getElementById("dashboardMotor");

const motorLiveDot =
    document.getElementById("motorLiveDot");

let introFinished = false;

function openDashboard()
{
    if (introFinished)
    {
        return;
    }

    introFinished = true;

    motorIntro.classList.add("intro-leaving");

    dashboard.classList.remove("dashboard-hidden");

    window.setTimeout(
        function()
        {
            motorIntro.style.display = "none";
        },
        1100
    );
}

skipIntro.addEventListener(
    "click",
    openDashboard
);

// Auto-enter the dashboard after the motor animation.
window.setTimeout(
    openDashboard,
    4600
);


// =====================================================
// ORIGINAL WEB UI
// =====================================================

const ui = new WebUI();


// =====================================================
// ELEMENTS
// =====================================================

const connection =
    document.getElementById(
        "connection"
    );

const voltage =
    document.getElementById(
        "voltage"
    );

const current =
    document.getElementById(
        "current"
    );

const power =
    document.getElementById(
        "power"
    );

const relay =
    document.getElementById(
        "relay"
    );

const pin7 =
    document.getElementById(
        "pin7"
    );

const armed =
    document.getElementById(
        "armed"
    );

const settling =
    document.getElementById(
        "settling"
    );

const condition =
    document.getElementById(
        "condition"
    );

const conditionCard =
    document.getElementById(
        "conditionCard"
    );

const conditionDescription =
    document.getElementById(
        "conditionDescription"
    );


const chatMessages =
    document.getElementById(
        "chatMessages"
    );

const chatStatus =
    document.getElementById(
        "chatStatus"
    );

const chatInput =
    document.getElementById(
        "chatInput"
    );

const sendButton =
    document.getElementById(
        "sendButton"
    );


// =====================================================
// STREAMING VARIABLES
// =====================================================

let streamingMessage = null;

let streamingParagraph = null;


// =====================================================
// CONNECTION
// =====================================================

ui.on_connect(
    function()
    {
        connection.innerText =
            "● CONNECTED";

        connection.className =
            "online";
    }
);


ui.on_disconnect(
    function()
    {
        connection.innerText =
            "● DISCONNECTED";

        connection.className =
            "offline";
    }
);


// =====================================================
// TELEMETRY
// =====================================================

ui.on_message(
    "telemetry",

    function(data)
    {

        voltage.innerText =
            Number(
                data.voltage
            ).toFixed(2);


        current.innerText =
            Number(
                data.current
            ).toFixed(3);


        power.innerText =
            Number(
                data.power
            ).toFixed(2);


        relay.innerText =
            data.relay
            ? "ON"
            : "OFF";


        pin7.innerText =
            data.pin7 === 0
            ? "LOW"
            : "HIGH";


        armed.innerText =
            data.armed
            ? "ARMED"
            : "NOT ARMED";


        settling.innerText =
            data.settling
            ? "YES"
            : "NO";


        condition.innerText =
            data.condition;


        // Dashboard motor animation follows live condition.
        if (dashboardMotor && motorLiveDot)
        {
            if (data.condition === "NORMAL")
            {
                dashboardMotor.classList.add("motor-running");
                motorLiveDot.classList.add("running");
                motorLiveDot.classList.remove("stopped");
            }
            else if (data.condition === "UNDERVOLTAGE")
            {
                dashboardMotor.classList.remove("motor-running");
                motorLiveDot.classList.remove("running");
                motorLiveDot.classList.add("stopped");
            }
            else
            {
                dashboardMotor.classList.remove("motor-running");
                motorLiveDot.classList.remove("running");
                motorLiveDot.classList.remove("stopped");
            }
        }


        conditionCard.className =
            "condition-card";


        if (
            data.condition ===
            "NORMAL"
        )
        {
            conditionCard.classList.add(
                "normal-condition"
            );


            conditionDescription.innerText =
                "Voltage is above 20.5 V. Motor condition is normal.";
        }


        else if (
            data.condition ===
            "UNDERVOLTAGE"
        )
        {
            conditionCard.classList.add(
                "danger-condition"
            );


            conditionDescription.innerText =
                "Voltage is 20.5 V or below. Undervoltage protection is active.";
        }

    }
);


// =====================================================
// ADD NORMAL CHAT MESSAGE
// =====================================================

function addMessage(
    sender,
    text
)
{

    const box =
        document.createElement(
            "div"
        );


    box.className =
        sender === "user"
        ? "user-message"
        : "gemma-message";


    const title =
        document.createElement(
            "strong"
        );


    title.innerText =
        sender === "user"
        ? "You"
        : "Gemma";


    const paragraph =
        document.createElement(
            "p"
        );


    paragraph.innerText =
        text;


    box.appendChild(
        title
    );


    box.appendChild(
        paragraph
    );


    chatMessages.appendChild(
        box
    );


    chatMessages.scrollTop =
        chatMessages.scrollHeight;
}


// =====================================================
// CREATE STREAMING GEMMA MESSAGE
// =====================================================

function startStreamingMessage()
{

    streamingMessage =
        document.createElement(
            "div"
        );


    streamingMessage.className =
        "gemma-message";


    const title =
        document.createElement(
            "strong"
        );


    title.innerText =
        "Gemma";


    streamingParagraph =
        document.createElement(
            "p"
        );


    streamingParagraph.innerText =
        "";


    streamingMessage.appendChild(
        title
    );


    streamingMessage.appendChild(
        streamingParagraph
    );


    chatMessages.appendChild(
        streamingMessage
    );


    chatMessages.scrollTop =
        chatMessages.scrollHeight;
}


// =====================================================
// SEND QUESTION
// =====================================================

function sendQuestion()
{

    const question =
        chatInput.value.trim();


    if (!question)
    {
        return;
    }


    addMessage(
        "user",
        question
    );


    chatInput.value =
        "";


    chatStatus.innerText =
        "Checking live motor data...";


    sendButton.disabled =
        true;


    ui.send_message(
        "chat",
        {
            "message":
                question
        }
    );
}


// =====================================================
// BUTTON
// =====================================================

sendButton.addEventListener(
    "click",
    sendQuestion
);


// =====================================================
// ENTER KEY
// =====================================================

chatInput.addEventListener(
    "keydown",

    function(event)
    {

        if (
            event.key ===
            "Enter"
        )
        {
            sendQuestion();
        }

    }
);


// =====================================================
// GEMMA STATUS
// =====================================================

ui.on_message(
    "chat_status",

    function(data)
    {

        chatStatus.innerText =
            data.status;

    }
);


// =====================================================
// FAST NON-STREAMED ANSWER
// =====================================================

ui.on_message(
    "chat_answer",

    function(data)
    {

        chatStatus.innerText =
            "";


        addMessage(
            "gemma",
            data.answer
        );


        sendButton.disabled =
            false;

    }
);


// =====================================================
// GEMMA STREAM START
// =====================================================

ui.on_message(
    "chat_stream_start",

    function()
    {

        chatStatus.innerText =
            "Gemma is responding...";


        startStreamingMessage();

    }
);


// =====================================================
// GEMMA STREAM CHUNK
// =====================================================

ui.on_message(
    "chat_chunk",

    function(data)
    {

        if (!streamingParagraph)
        {
            startStreamingMessage();
        }


        streamingParagraph.innerText +=
            data.chunk;


        chatMessages.scrollTop =
            chatMessages.scrollHeight;

    }
);


// =====================================================
// GEMMA STREAM END
// =====================================================

ui.on_message(
    "chat_stream_end",

    function()
    {

        chatStatus.innerText =
            "";


        sendButton.disabled =
            false;


        streamingMessage =
            null;


        streamingParagraph =
            null;

    }
);


// =====================================================
// GEMMA ERROR
// =====================================================

ui.on_message(
    "chat_error",

    function(data)
    {

        chatStatus.innerText =
            "";


        addMessage(
            "gemma",
            "Error: " + data.error
        );


        sendButton.disabled =
            false;


        streamingMessage =
            null;


        streamingParagraph =
            null;

    }
);