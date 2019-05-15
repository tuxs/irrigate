const TelegramBot = require('node-telegram-bot-api');
const token = '889153866:AAEMvVUPI4xNikUcwnyndcigO03PgLIMUnY';//Cambiar por el token de telegram
const bot = new TelegramBot(token, {
  polling: true
});

var IdMiChat = 714112464;//cambiar por tu ID del chat

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
  } /*else if (Mensaje != "Regar") {
    console.log("Instruccion no reconocida");
    bot.sendMessage(chatId, 'Instruccion no reconocida');*/
  else if (Mensaje == "Temperatura") {
    console.log("Temperatura");
    bot.sendMessage(IdMiChat, 'Temperatura actual: ' + MiPuerto.write('T'));
    MiPuerto.write("T");
  }  else{
      console.log("Instruccion no Valida");
      bot.sendMessage(IdMiChat, 'Instruccion no Valida');
    }
});


MiPuerto.setEncoding('utf8');

MiPuerto.on('data', function(data) {
  console.log("Lo que entro es " + data);
  if (data[0] == 'H') {
    console.log("Riego Manual Activado");
    bot.sendMessage(IdMiChat, 'Riego Manual Activado');
  }else if(data[0] == 'T') {
      console.log("Temperatura es" + data);
      bot.sendMessage(IdMiChat, "Temperatura es");
    }
});
