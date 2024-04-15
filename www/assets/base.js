function updateDashboard() {
    fetch("/api/dashboard")
        .then((res) => {
            // TODO handle bad status code
            return res.json();
        })
        .then((json) => {
            console.log(json);
        });
}

updateDashboard();
setInterval(updateDashboard, 30000);
