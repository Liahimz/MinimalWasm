const selectFile = document.getElementById("select_file");
const startButton = document.getElementById("button_start");
const outputDiv = document.getElementById("output");
let selectedFile = null;

let worker = new Worker("worker.js");

selectFile.addEventListener("change", (e) => {
  selectedFile = e.target.files[0];
  outputDiv.textContent = "";
});

startButton.addEventListener("click", () => {
  if (!selectedFile) {
    outputDiv.textContent = "Please select a file!";
    return;
  }
  let reader = new FileReader();
  reader.onload = function(e) {
    // Load into an Image object to get dimensions and draw to canvas for grayscale
    let img = new Image();
    img.onload = function() {
      let canvas = document.createElement("canvas");
      canvas.width = img.width;
      canvas.height = img.height;
      let ctx = canvas.getContext("2d");
      ctx.drawImage(img, 0, 0);
      // Get RGBA, convert to grayscale
      let imageData = ctx.getImageData(0, 0, img.width, img.height);
      let gray = new Uint8Array(img.width * img.height);
      for (let i = 0; i < img.width * img.height; ++i) {
        let r = imageData.data[i*4 + 0];
        let g = imageData.data[i*4 + 1];
        let b = imageData.data[i*4 + 2];
        // Standard grayscale conversion
        gray[i] = Math.round(0.299*r + 0.587*g + 0.114*b);
      }
      worker.postMessage({
        requestType: "file",
        imageData: gray.buffer,
        width: img.width,
        height: img.height
      }, [gray.buffer]);
      outputDiv.textContent = "Processing...";
    };
    img.src = URL.createObjectURL(selectedFile);
  };
  reader.readAsArrayBuffer(selectedFile);
});

worker.onmessage = function(e) {
  if (e.data.requestType === "result") {
    // Display processed image
    let { outImage, width, height } = e.data;
    let canvas = document.createElement("canvas");
    canvas.width = width;
    canvas.height = height;
    let ctx = canvas.getContext("2d");
    // OutImage is Uint8Array (grayscale)
    let out = new Uint8ClampedArray(width * height * 4);
    for (let i = 0; i < width * height; ++i) {
      out[i*4 + 0] = out[i*4 + 1] = out[i*4 + 2] = e.data.outImage[i];
      out[i*4 + 3] = 255;
    }
    let imageData = new ImageData(out, width, height);
    ctx.putImageData(imageData, 0, 0);
    outputDiv.innerHTML = "";
    outputDiv.appendChild(canvas);
  }
};
