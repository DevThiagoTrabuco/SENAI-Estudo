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

// Referência para a tabela de tentativas
const tabela = document.getElementById("tabela-tentativas");

// Escuta as alterações no banco de dados e atualiza a tabela
firebase.database().ref("/login/tentativas").on("value", (snapshot) => {
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

const cadastroForm = document.getElementById("cadastro-form");

cadastroForm.addEventListener("submit", (event) => {
    event.preventDefault();

    const username = document.getElementById("username").value;
    const password = document.getElementById("password").value;

    const usernameRegex = /^[ABCD0-9]+$/i;
    if (!usernameRegex.test(username)) {
        alert("O nome de usuário só pode conter números e as letras A, B, C e D.");
        return;
    }

    const usuario = {
        usuario: username,
        senha: password
    };

    firebase.database().ref("/login/usuarios").push(usuario, (error) => {
        if (error) {
            alert("Erro ao salvar os dados de cadastro.");
        } else {
            alert("Cadastro realizado com sucesso.");
            cadastroForm.reset();
        }
    });
});