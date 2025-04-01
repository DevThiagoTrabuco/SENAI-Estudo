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

  // Referência para os dados no Realtime Database
  const tempRef = database.ref("/status/temperature");
  const umidRef = database.ref("/status/humidity");
  const statusRef = database.ref("/status/led");
  const chibi = document.getElementById("chibi");
  const chibiSad = document.getElementById("chibiSad");

  // Atualiza os dados na tela em tempo real
  tempRef.on("value", snapshot => {
    document.getElementById("temp").textContent = snapshot.val().toFixed(1);
  });

  umidRef.on("value", snapshot => {
    document.getElementById("umid").textContent = snapshot.val().toFixed(1);
  });

  statusRef.on("value", snapshot => {
    const status = snapshot.val();
    const statusSpan = document.getElementById("statusLed");
    statusSpan.textContent = status;
    statusSpan.className = "status-led " + status;
  
    // Muda o estado do checkbox de acordo com o valor do statusLED
    const toggleSwitch = document.getElementById("toggle-switch");
    
    if (status === true) {
      toggleSwitch.checked = true;
      chibi.style.display = "flex";
      chibiSad.style.display = "none";  // Marca o checkbox se o LED estiver ligado
    } else{
      toggleSwitch.checked = false;
      chibi.style.display = "none";
      chibiSad.style.display = "flex"; // Desmarca o checkbox se o LED estiver desligado
    }
  });

  document.getElementById("toggle-switch").addEventListener("change", (event) => {
    const status = event.target.checked; // Se o checkbox estiver marcado, será true (LED ligado); caso contrário, false (LED desligado)
  
    // Envia o status para o Firebase
    statusRef.set(status); // Atualiza o valor do statusLED no Firebase (true ou false)
  });