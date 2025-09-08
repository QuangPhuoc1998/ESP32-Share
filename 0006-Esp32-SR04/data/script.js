const distanceValue = document.getElementById("distanceValue");
const refreshBtn = document.getElementById("refreshBtn");

function updateDistance() {
  fetch(`/distance`)
    .then(response => response.text())
    .then(data => {
      distanceValue.textContent = data;
      console.log("Distance:", data, "cm");
    })
    .catch(err => {
      console.error("Error fetching distance:", err);
    });
}

refreshBtn.addEventListener("click", updateDistance);

setInterval(updateDistance, 500);
