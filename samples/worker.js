console.log("Worker.js started")


importScripts("idengine_wasm.js");

// if (typeof Module !== "undefined") {
//   // Worker-specific: set mainScriptUrlOrBlob to current script
//   console.log("Worker.js in undefined")
//   Module['mainScriptUrlOrBlob'] = 'idengine_wasm.js';
// }

console.log("SmartIDEngine in worker:", typeof SmartIDEngine);
let engineInstance = null;
let readyPromise = SmartIDEngine({
  mainScriptUrlOrBlob: 'idengine_wasm.js',
  // you can also add locateFile if needed
}).then(Module => {
  engineInstance = new Module.DummyEngine();
  engineInstance.configure(4);
  moduleObject = Module;

  // Module._start_tbb_session();

  return Module;
});

onmessage = async function(msg) {
  await readyPromise;
  if (msg.data.requestType === "file") {
    let arr = msg.data.imageData;
    // let arr = new Uint8Array([1, 2, 3, 4]);
    if (!(arr instanceof Uint8Array)) arr = new Uint8Array(arr);
    console.log(arr, arr instanceof Uint8Array, Array.isArray(arr), arr.buffer);
    let width = msg.data.width;
    // let width = 2;
    let height = msg.data.height;
    // let height = 2;
    let channels = msg.data.channels || 4; // default to RGBA if not specified
    // let arrForCpp = new moduleObject.Uint8Vector(arr);
    let targetWidth = msg.data.targetWidth || width;

    let cppVec = new moduleObject.Uint8Vector();
    for (let i = 0; i < arr.length; ++i) {
        cppVec.push_back(arr[i]);
    }

    moduleObject._stop_keepalive_mainloop();
    // console.log(arrForCpp instanceof Uint8Array, Array.isArray(arrForCpp));
    let resultVec = engineInstance.process(cppVec, width, height, channels, targetWidth);

    
    let outArr = [];
    for (let i = 0; i < resultVec.image.size(); ++i) {
        outArr.push(resultVec.image.get(i)); // .get() or .at() depending on embind
    }
    let outImage = new Uint8Array(outArr);

    console.log(outImage, outImage.buffer);
    postMessage({ requestType: "result", outImage, width: resultVec.width, height: resultVec.height }, [outImage.buffer]);
    moduleObject._start_keepalive_mainloop();
  }
};