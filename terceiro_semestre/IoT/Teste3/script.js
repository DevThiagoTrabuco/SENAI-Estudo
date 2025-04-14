// Configuração do Firebase (substitua com os seus dados!)
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
  const app = firebase.initializeApp(firebaseConfig);
  const database = firebase.database();

    const tabela = document.getElementById("tabela-tentativas");

    firebase.database().ref("/tentativas").on("value", (snapshot) => {
        tabela.innerHTML = "";
        const dados = snapshot.val();

        if (dados) {
            const lista = Object.values(dados).sort((a, b) => new Date(b.timestamp) - new Date(a.timestamp));

            lista.forEach((item) => {
                const linha = document.createElement("tr");
                linha.innerHTML = `
                    <td>${item.timestamp}</td>
                    <td>${item.resultado}</td>
                `;
                tabela.appendChild(linha);
            });
        } else {
            tabela.innerHTML = "<tr><td colspan='2'>Nenhuma tentativa registrada ainda.</td></tr>";
        }
    });