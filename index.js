import { } from "./gameboy.js"
import { Debug } from "./debug.js";
var gameboy;
// var Module2 = {
//     onRuntimeInitialized: function () {
//         gameboy = new Module2.Gameboy();
//     }
// };

var myModule = await Module();
gameboy = new myModule.Gameboy();

let selectedRom = document.getElementById("romInput");
selectedRom.addEventListener("change", startGameboy, false);

async function readRom(rom) {
    let fileReader = new FileReader();

    return new Promise((resolve, reject) => {
        let result = null;
        fileReader.onload = (e) => {
            result = e.target.result;
            resolve(new Uint8Array(result));
        }
        fileReader.readAsArrayBuffer(rom);
    });
}


async function startGameboy() {
    let rom = this.files[0];
    let romInput = await readRom(rom);

    var heapSpace = myModule._malloc(romInput.length * romInput.BYTES_PER_ELEMENT);
    myModule.HEAPU8.set(romInput, heapSpace);

    gameboy.readRom(heapSpace, romInput.length)
    gameboy.initialize();

    setInterval(gameboy.mainLoop.bind(gameboy), 17);
}

window.addEventListener("keydown", function (e) {
    if (["Space", "ArrowUp", "ArrowDown", "ArrowLeft", "ArrowRight"].indexOf(e.code) > -1) {
        e.preventDefault();
    }
}, false);

// -------------------

var debug = new Debug();

// -------------------
var backgroundCanvasTileMapSource = document.getElementById("background-canvas-tile-map-source");
var backgroundCanvasBackgroundSource = document.getElementById("background-canvas-background-source");

backgroundCanvasTileMapSource.addEventListener("change",updateBackgroundViewer);
backgroundCanvasBackgroundSource.addEventListener("change", updateBackgroundViewer);
function updateBackgroundViewer() {
    gameboy.setBackgroundSettings(backgroundCanvasTileMapSource.value, backgroundCanvasTileMapSource.value);
}


