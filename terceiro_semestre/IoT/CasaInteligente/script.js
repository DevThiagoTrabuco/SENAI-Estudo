// Your web app's Firebase configuration
const firebaseConfig = {
  apiKey: "AIzaSyBrnqehQ_h9MbIcYlDHK4BHbyvgztQZRqs",
  authDomain: "iot-senai-esp32.firebaseapp.com",
  databaseURL: "https://iot-senai-esp32-default-rtdb.firebaseio.com",
  projectId: "iot-senai-esp32",
  storageBucket: "iot-senai-esp32.firebasestorage.app",
  messagingSenderId: "654364028381",
  appId: "1:654364028381:web:9c9307162ba9852422da34"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);

const database = firebase.database();
const dispositivo = "ESP32-01";
const caminho = `testeCasa/${dispositivo}`;

function enviarComando(comando) {
  database.ref(`${caminho}/${comando}`).set(true);
}

function ligarTudo() {
  enviarComando("comando_ligar_lcd");
  enviarComando("comando_ligar_luz");
}

function desligarTudo() {
  enviarComando("comando_desligar_lcd");
  enviarComando("comando_desligar_luz");
}

function alternarModo() {
  database.ref(`${caminho}/modo`).once("value", (snapshot) => {
    const modoAtual = snapshot.val();
    const novoModo = modoAtual === "auto" ? "manual" : "auto";
    database.ref(`${caminho}/modo`).set(novoModo);
  });
}

// Atualiza status em tempo real
function atualizarStatus() {
  database.ref(`${caminho}/estado_luz`).on("value", (snapshot) => {
    document.getElementById("status-luz").innerText = snapshot.val() ? "Ligada" : "Desligada";
  });

  database.ref(`${caminho}/estado_lcd`).on("value", (snapshot) => {
    document.getElementById("status-lcd").innerText = snapshot.val() ? "Ligado" : "Desligado";
  });

  database.ref(`${caminho}/modo`).on("value", (snapshot) => {
    document.getElementById("status-modo").innerText = snapshot.val();
  });
}

// Inicia escuta de status ao carregar a página
window.onload = atualizarStatus;
