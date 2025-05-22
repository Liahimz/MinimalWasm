importScripts("idengine_wasm.js");

let engineInstance = null;
let readyPromise = SmartIDEngine().then(Module => {
  engineInstance = new Module.DummyEngine();
  engineInstance.configure(4); // or any thread count you want
  return Module;
});

onmessage = async function(msg) {
  await readyPromise;
  if (msg.data.requestType === "file") {
    let arr = new Uint8Array(msg.data.imageData);
    // You also need to pass width and height!
    let width = msg.data.width;
    let height = msg.data.height;
    let resultVec = engineInstance.process(arr, width, height); // returns Uint8Vector
    postMessage({ requestType: "result", outImage: resultVec, width, height });
  }
};
