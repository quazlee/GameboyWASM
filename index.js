import { } from "./Gameboy.js"
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

    var romSpace = myModule._malloc(romInput.length * romInput.BYTES_PER_ELEMENT);
    myModule.HEAPU8.set(romInput, romSpace);


    gameboy.readRom(romSpace, romInput.length);
    gameboy.initialize();

    setInterval(gameboyMainLoop.bind(gameboy), 17);
}

function gameboyMainLoop() {
    gameboy.mainLoop();

    // downloadDebugString();
    
    // var heap = gameboy.getBackground();
    // const arrayData = []
    // for (let v = 0; v < 65536; v++) {
    //     arrayData.push(myModule.HEAP32[heap / Int32Array.BYTES_PER_ELEMENT + v])
    // }

    // console.log(arrayData);
}

function downloadDebugString() {
    let debugStringHeap = gameboy.getDebugStringFull();
    const debugStringData = [];
    for (let i = 0; i < debugStringHeap; i++) {
        debugStringData.push(String.fromCharCode(myModule.HEAP8[debugStringHeap / Int8Array.BYTES_PER_ELEMENT + i]));
    }
    var element = document.createElement('a');
    element.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(debugStringData.join("")));
    element.setAttribute('download', "Log");
    element.style.display = 'none';
    document.body.appendChild(element);
    element.click();
    document.body.removeChild(element);
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

backgroundCanvasTileMapSource.addEventListener("change", updateBackgroundViewer);
backgroundCanvasBackgroundSource.addEventListener("change", updateBackgroundViewer);
function updateBackgroundViewer() {
    gameboy.setBackgroundSettings(backgroundCanvasTileMapSource.value, backgroundCanvasTileMapSource.value);
}


