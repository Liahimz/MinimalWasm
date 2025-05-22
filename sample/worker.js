importScripts('idengine_wasm.js'); // Loads SmartIDEngine

let engineInstance = null;
let readyPromise = SmartIDEngine().then(Module => {
    engineInstance = new Module.DummyEngine();
    engineInstance.configure(4); // Example: configure 4 threads
    return Module;
});

onmessage = async function(msg) {
    await readyPromise;
    if (msg.data.requestType === "file") {
        let arr = new Uint8Array(msg.data.imageData);
        // embind will convert JS array to C++ std::vector<uint8_t>
        let result = engineInstance.process(arr);
        postMessage({ requestType: "result", result: result });
    }
};
