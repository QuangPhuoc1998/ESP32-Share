const tempValue = document.getElementById("tempValue");
const humiValue = document.getElementById("humiValue");

const tempCanvas = document.getElementById("tempGauge");
const humiCanvas = document.getElementById("humiGauge");

const tempCtx = tempCanvas.getContext("2d");
const humiCtx = humiCanvas.getContext("2d");

function drawGauge(ctx, value, max, color) {
  const centerX = ctx.canvas.width / 2;
  const centerY = ctx.canvas.height / 2;
  const radius = 80;

  ctx.clearRect(0, 0, ctx.canvas.width, ctx.canvas.height);

  // background arc
  ctx.beginPath();
  ctx.arc(centerX, centerY, radius, 0, 2 * Math.PI);
  ctx.strokeStyle = "#ddd";
  ctx.lineWidth = 25;
  ctx.stroke();

  // value arc
  const endAngle = (value / max) * 2 * Math.PI;
  ctx.beginPath();
  ctx.arc(centerX, centerY, radius, -Math.PI / 2, endAngle - Math.PI / 2);
  ctx.strokeStyle = color;
  ctx.lineWidth = 25;
  ctx.lineCap = "round";
  ctx.stroke();

  // text value
  ctx.fillStyle = "#333";
  ctx.font = "20px Arial";
  ctx.textAlign = "center";
  ctx.textBaseline = "middle";
  ctx.fillText(value.toFixed(1), centerX, centerY);
}

function updateSensorData() {
  fetch("/sensor")
    .then((response) => response.json())
    .then((data) => {
      renderData(data.temperature, data.humidity);
    })
    .catch(() => {
      // fallback (fake data for test)
      // const fakeTemp = Math.random() * 40;
      // const fakeHumi = Math.random() * 100;
      // renderData(fakeTemp, fakeHumi);
    });
}

function renderData(temp, humi) {
  tempValue.textContent = temp.toFixed(1);
  humiValue.textContent = humi.toFixed(1);

  drawGauge(tempCtx, temp, 50, "#ff5733"); // max 50°C
  drawGauge(humiCtx, humi, 100, "#3399ff"); // max 100%
}

setInterval(updateSensorData, 2000);
updateSensorData();
