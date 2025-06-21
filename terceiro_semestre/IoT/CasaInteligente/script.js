// Configuração do Firebase
const firebaseConfig = {
  apiKey: "AIzaSyBrnqehQ_h9MbIcYlDHK4BHbyvgztQZRqs",
  authDomain: "iot-senai-esp32.firebaseapp.com",
  databaseURL: "https://iot-senai-esp32-default-rtdb.firebaseio.com",
  projectId: "iot-senai-esp32",
  storageBucket: "iot-senai-esp32.appspot.com",
  messagingSenderId: "654364028381",
  appId: "1:654364028381:web:9c9307162ba9852422da34"
};

// Inicialização do Firebase
firebase.initializeApp(firebaseConfig);
const database = firebase.database();

const dispositivo = "ESP32-01";
const caminho = `testeCasa/${dispositivo}`;

// Função para enviar comandos
function enviarComando(comando) {
  database.ref(`${caminho}/${comando}`).set(true);
}

// Botões compostos
function ligarTudo() {
  enviarComando("comando_ligar_lcd");
  enviarComando("comando_ligar_led1");
  enviarComando("comando_ligar_led2");
}

function desligarTudo() {
  enviarComando("comando_desligar_lcd");
  enviarComando("comando_desligar_led1");
  enviarComando("comando_desligar_led2");
}

// Alternar modo automático/manual
function alternarModo() {
  const refModo = database.ref(`${caminho}/modo`);
  refModo.once("value").then(snapshot => {
    const modoAtual = snapshot.val();
    const novoModo = modoAtual === "auto" ? "manual" : "auto";
    refModo.set(novoModo);
  });
}

// Atualização de status em tempo real
function atualizarStatus() {
  // LED 1
  database.ref(`${caminho}/led1`).on("value", snapshot => {
    document.getElementById("status-led1").innerText = snapshot.val() ? "Ligada" : "Desligada";
  });

  // LCD
  database.ref(`${caminho}/lcd`).on("value", snapshot => {
    document.getElementById("status-lcd").innerText = snapshot.val() ? "Ligado" : "Desligado";
  });

  // LED 2
  database.ref(`${caminho}/led2`).on("value", snapshot => {
    document.getElementById("status-led2").innerText = snapshot.val() ? "Ligado" : "Desligado";
  });

  // Modo
  database.ref(`${caminho}/modo`).on("value", snapshot => {
    document.getElementById("status-modo").innerText = snapshot.val();
  });
}

// Inicia escuta ao carregar a página
window.onload = atualizarStatus;