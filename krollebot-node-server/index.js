// Lavet af Buster, Emil og Nikolaj Jehøj-Krogager
// i forbindelse med et eksamensprojekt i Elektronik & IT
// 24/04/2021

// bruges til at snakke med hjemmesiden
const httpServer = require("http").createServer();
const io = require("socket.io")(httpServer, {
    cors: {
        // gør så p5.js (samt alle andre hjemmesider) kan lave
        // forespørgsler til denne server
        origin: '*',
    }
});

console.log("Programmet kører...");

// bruges til at snakke med Arduinoen
const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline')

const port = new SerialPort('COM3', {
    baudRate: 9600
});

// Gør så når der modtages data fra Arduinoen stopper den ved '\n'.
const parser = port.pipe(new Readline())

// modtager data fra Arduino Uno
parser.on('data', (data) => {
    if (data.substring(0, 5) == "Debug") {
        console.log("Debug tilstand: "+ data);
    } else {
        console.log("Sender data til hjemmeside: " + data);
        // videresender data til hjemmeside
        io.emit("krollebot_message", data);
    }
});

// Denne funktion køres når en person tilslutter sig fra hjemmesiden
io.on("connection", (socket) => {
    console.log("En person er forbundet :-)");

    // modtager data fra hjemmesiden
    socket.on("krollebot_message", (message) => {
        port.write(message+'\n');
        console.log("Skriver til Arduino Uno: " + message);
    });
});

httpServer.listen(8080);
