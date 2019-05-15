const TelegramBot = require('node-telegram-bot-api');
const token = '889153866:AAEMvVUPI4xNikUcwnyndcigO03PgLIMUnY'; //Cambiar por el token de telegram
const bot = new TelegramBot(token, {
  polling: true
});

var IdMiChat = 714112464; //cambiar por tu ID del chat

var SerialPort = require('serialport');
var MiPuerto = new SerialPort('/dev/ttyACM0', {
  baudRate: 9600,
  autoOpen: true
});

bot.on('message', (msg) => {
  const chatId = msg.chat.id;
  console.log("El ID del chat" + chatId);
  var Mensaje = msg.text;
  if (Mensaje == "Regar") {
    console.log("Regando....");
    bot.sendMessage(chatId, 'Regando....');
    MiPuerto.write("H");
  } else if (Mensaje == "Temperatura") {
    console.log("Temperatura");
    MiPuerto.write("T");
  } else {
    console.log("Instruccion no Valida");
    bot.sendMessage(IdMiChat, 'Instruccion no Valida');
  }
});


MiPuerto.setEncoding('utf8');

let Mensaje = "";
let Buscar = false;

MiPuerto.on('data', function(data) {

  console.log("Lo que entro es " + data);
  for (var i = 0; i < data.length; i++) {
    if (data[i] == 'H') {
      console.log("Riego Manual Activado");
      bot.sendMessage(IdMiChat, 'Riego Manual Activado');
    } else if (data[i] == 'M') {
      console.log("Empezando a llegar mensaje");
      Mensaje = "";
      Buscar = true;
      //  bot.sendMessage(IdMiChat, "Temperatura es");
    } else if (data[i] == 'X') {
      bot.sendMessage(IdMiChat, "Estado actual " + Mensaje);
      Buscar = false;
    } else if (data[i] == "P") {
      bot.sendMessage(IdMiChat, "Termino de Regar");
    } else if (Buscar) {
      Mensaje = Mensaje + data[i];
    }
  }
});
