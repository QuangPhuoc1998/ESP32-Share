const canvas = document.getElementById("colorWheel");
const ctx = canvas.getContext("2d");
const colorValue = document.getElementById("colorValue");
const colorPreview = document.getElementById("colorPreview");

const radius = canvas.width / 2;
const toRad = Math.PI / 180;

// Vẽ vòng tròn màu
for (let angle = 0; angle < 360; angle++) {
  const startAngle = (angle - 1) * toRad;
  const endAngle = angle * toRad;

  ctx.beginPath();
  ctx.moveTo(radius, radius);
  ctx.arc(radius, radius, radius, startAngle, endAngle, false);
  ctx.closePath();
  ctx.fillStyle = `hsl(${angle}, 100%, 50%)`;
  ctx.fill();
}

// Xử lý di chuyển chuột
let isDragging = false;

canvas.addEventListener("mousedown", (e) => {
  isDragging = true;
  handleColorPick(e);
});

canvas.addEventListener("mouseup", () => {
  isDragging = false;
});

canvas.addEventListener("mouseleave", () => {
  isDragging = false;
});

canvas.addEventListener("mousemove", (e) => {
  if (isDragging) {
    handleColorPick(e);
  }
});

function handleColorPick(e) {
  const rect = canvas.getBoundingClientRect();
  const x = e.clientX - rect.left;
  const y = e.clientY - rect.top;

  const imageData = ctx.getImageData(x, y, 1, 1);
  const [r, g, b, a] = imageData.data;

  // Nếu là pixel trong suốt thì bỏ qua (ngoài vùng tròn)
  if (a === 0) return;

  const hex = rgbToHex(r, g, b);
  colorValue.textContent = hex;
  colorPreview.style.backgroundColor = hex;

  var xhr = new XMLHttpRequest();
  let currentString = "/setcolor?color=" + encodeURIComponent(hex);
  xhr.open("GET", currentString, true);
  xhr.send();

}

function rgbToHex(r, g, b) {
  return (
    "#" +
    [r, g, b]
      .map((x) => x.toString(16).padStart(2, "0"))
      .join("")
      .toUpperCase()
  );
}
