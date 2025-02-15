document.addEventListener("DOMContentLoaded", () => {
    const input = document.querySelector("#search-input");
    const datalist = document.querySelector("#employee-names");

    if (!input || !datalist) return;

    let debounceTimeout;

    input.addEventListener("input", () => {
        clearTimeout(debounceTimeout);

        const query = input.value;

        debounceTimeout = setTimeout(() => {

            fetch(`/api/employees/autocomplete?query=${encodeURIComponent(query)}`, {
                method: "GET",
                headers: { Accept: "application/json" },
            })
                .then((response) => response.json())
                .then((data) => {
                    datalist.innerHTML = "";

                    if (Array.isArray(data.data) && data.data.length > 0) {
                        data.data.slice(0, 10).forEach((employee) => {
                            const option = document.createElement("option");
                            option.value = employee.fullName;
                            datalist.appendChild(option);
                        });
                    }
                })
                .catch(() => {
                    datalist.innerHTML = "";
                });
        }, 300);
    });
});
