import { } from "./Gameboy.js"
import { Debug } from "./debug.js";
var debug = new Debug();
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

let bigDebugString = [];
function gameboyMainLoop() {
    gameboy.mainLoop();
    drawViewPort();

    // buildDebugString(bigDebugString);
    // if (bigDebugString.length == 10) {
    //     // for (let i = 0; i < 6; i++) {
    //     //     downloadDebugString([bigDebugString[i]]);

    //     // }
    //     downloadDebugString([bigDebugString.join("")]);
    // }

    backgroundMapDrawer();
    tileMapDrawer();


    // console.log(arrayData);
}

var viewportCanvas = document.getElementById("viewport-canvas");
var viewportCanvasCtx = viewportCanvas.getContext("2d");

function drawViewPort() {
    let viewPortHeap = gameboy.getViewPort();
    let viewPortData = [];
    for (let i = 0; i < 160 * 144; i++) {
        viewPortData.push(myModule.HEAP32[viewPortHeap / Int32Array.BYTES_PER_ELEMENT + i]);
        drawToCanvas(i % 160, Math.floor(i / 160), colorPalette[myModule.HEAP32[viewPortHeap / Int32Array.BYTES_PER_ELEMENT + i]], viewportCanvasCtx);
    }
}


function buildDebugString(bigDebugString) {
    let debugStringHeap = gameboy.getDebugStringFull();
    const debugStringData = [];
    for (let i = 3; i < gameboy.getDebugStringLength() + 3; i++) {
        debugStringData.push(String.fromCharCode(myModule.HEAP8[debugStringHeap / Int8Array.BYTES_PER_ELEMENT + i]));
    }
    bigDebugString.push(debugStringData.join(""));
}

function downloadDebugString(bigDebugString) {

    var element = document.createElement('a');
    element.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(bigDebugString.join("")));
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
var backgroundCanvasTileMapSource = document.getElementById("background-canvas-tile-map-source");
var backgroundCanvasBackgroundSource = document.getElementById("background-canvas-background-source");
var backgroundCanvas = document.getElementById("background-canvas");
var backgroundCanvasCtx = backgroundCanvas.getContext("2d");
var tileMapCanvas = document.getElementById("tile-map-canvas");
var tileMapCanvasCtx = tileMapCanvas.getContext("2d");

backgroundCanvasTileMapSource.addEventListener("change", updateBackgroundViewer);
backgroundCanvasBackgroundSource.addEventListener("change", updateBackgroundViewer);
function updateBackgroundViewer() {
    gameboy.setBackgroundSettings(backgroundCanvasTileMapSource.value, backgroundCanvasTileMapSource.value);
}

var colorPalette = ["rgba(255, 255, 255, 1)",
    "rgba(172, 172, 172, 1)",
    "rgba(86, 86, 86, 1)",
    "rgba(0, 0, 0, 1)"];

function drawToCanvas(x, y, color, ctx) {
    ctx.strokeStyle = color;
    ctx.fillStyle = color;
    ctx.fillRect(x, y, 1, 1);
}
function tileMapDrawer() {
    var heap = gameboy.getTileMap();
    var tileMapData = [];
    for (let v = 0; v < 24576; v++) {
        tileMapData.push(myModule.HEAP32[heap / Int32Array.BYTES_PER_ELEMENT + v])
    }

    for (let y = 0; y < 24; y++) {
        for (let x = 0; x < 16; x++) {
            for (let i = 0; i < 8; i++) {
                for (let j = 0; j < 8; j++) {
                    drawToCanvas((x * 8) + j, (y * 8) + i, colorPalette[tileMapData.pop()], tileMapCanvasCtx);
                }
            }
        }
    }


}
function backgroundMapDrawer() {

    var heap = gameboy.getBackground();
    var backgroundData = [];
    for (let v = 0; v < 65536; v++) {
        backgroundData.push(myModule.HEAP32[heap / Int32Array.BYTES_PER_ELEMENT + v])
    }

    for (let y = 0; y < 32; y++) {
        for (let x = 0; x < 32; x++) {
            for (let i = 0; i < 8; i++) {
                for (let j = 0; j < 8; j++) {
                    drawToCanvas((x * 8) + j, (y * 8) + i, colorPalette[backgroundData.pop()], backgroundCanvasCtx);
                }
            }
        }
    }
}