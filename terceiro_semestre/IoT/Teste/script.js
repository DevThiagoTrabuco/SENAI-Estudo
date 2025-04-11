// Configuração do Firebase
const firebaseConfig = {
    apiKey: "AIzaSyBrnqehQ_h9MbIcYlDHK4BHbyvgztQZRqs",
    authDomain: "iot-senai-esp32.firebaseapp.com",
    databaseURL: "https://iot-senai-esp32-default-rtdb.firebaseio.com",
    projectId: "iot-senai-esp32",
    storageBucket: "iot-senai-esp32.firebasestorage.app",
    messagingSenderId: "654364028381",
    appId: "1:654364028381:web:9c9307162ba9852422da34"
  };

// Inicializa o Firebase
firebase.initializeApp(firebaseConfig);
const database = firebase.database();

const ledStatusElement = document.getElementById("ledStatus");
const toggleSwitch = document.getElementById("toggleSwitch");

// Atualiza o status do LED em tempo real
database.ref("/statusLed/estado").on("value", (snapshot) => {
    const estado = snapshot.val();
    if (estado === 1) {
        ledStatusElement.textContent = "Ligado";
        toggleSwitch.classList.add("on");
    } else {
        ledStatusElement.textContent = "Desligado";
        toggleSwitch.classList.remove("on");
    }
});

// Alterna o estado do LED ao clicar no interruptor
toggleSwitch.addEventListener("click", () => {
    database.ref("/statusLed/estado").once("value").then((snapshot) => {
        const estadoAtual = snapshot.val();
        const novoEstado = (estadoAtual === 1) ? 0 : 1;
        database.ref("/statusLed/").set({ estado: novoEstado });
    });
});

