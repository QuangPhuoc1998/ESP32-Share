const servoSlider = document.getElementById("servoSlider");
const angleValue = document.getElementById("angleValue");

servoSlider.addEventListener("input", () => {
  const angle = servoSlider.value;
  angleValue.textContent = angle;

  fetch(`/servo?angle=${angle}`)
    .then(response => response.text())
    .then(data => {
      console.log("Servo set to:", data);
    })
    .catch(err => {
      console.error("Error sending servo command:", err);
    });
});
