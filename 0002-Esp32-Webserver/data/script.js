function toggleLED(checkbox) {
    const ledNumber = checkbox.getAttribute("data-led");
    const xhr = new XMLHttpRequest();
    const url = "/toggle?led=" + ledNumber;
    xhr.open("GET", url, true);
    xhr.send();
}

window.onload = function () {
    [1, 2].forEach(function (led) {
        const xhr = new XMLHttpRequest();
        xhr.open("GET", "/status?led=" + led, true);
        xhr.onload = function () {
            if (xhr.status === 200) {
                const state = xhr.responseText.trim();
                const checkbox = document.querySelector(`input[data-led="${led}"]`);
                if (checkbox) {
                    checkbox.checked = (state === "ON");
                }
            }
        };
        xhr.send();
    });
};
