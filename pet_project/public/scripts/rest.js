"use strict";

function getDataFromForm(form) {
    const formData = new FormData(form);
    const data = {};
    formData.forEach((value, key) => {
        const strippedKey = key.replace(/^create_account\[(.*)\]$/, "$1");
        data[strippedKey] = value;
    });

    if (data.expiration) {
        const date = new Date(data.expiration);
        data.expiration = date.toISOString().split("T")[0];
    }
    return data;
}

document.addEventListener("DOMContentLoaded", () => {
    const form = document.getElementById("create-account-form");

    function refreshAccounts(employeeId) {
        fetch(`/api/employees/${employeeId}/accounts`, {
            method: "GET",
            headers: { Accept: "application/json" },
        })
            .then((response) => response.json())
            .then((data) => {
                const tableBody = document.querySelector(".accounts-table tbody");
                tableBody.innerHTML = "";
                data.data.forEach((account) => {
                    let formattedExpiration = "Permanentní";
                    if (account.expiration) {
                        const date = new Date(account.expiration);
                        formattedExpiration = date.toISOString().split("T")[0];
                    }

                    const row = document.createElement("tr");
                    row.innerHTML = `
                    <td><strong>${account.name}</strong></td>
                    <td><strong>${account.type}</strong></td>
                    <td><strong>${formattedExpiration}</strong></td>
                    <td class="actions">
                        <a href="/accounts/${employeeId}/edit/${account.id}" class="action-link">
                            <img src="/icons/edit.png" alt="Upravit účet" class="icon">
                        </a>
                        <a href="#" class="action-link delete-account" data-id="${account.id}" data-employee-id="${employeeId}">
                            <img src="/icons/delete.png" alt="Smazat účet" class="icon">
                        </a>
                    </td>
                `;
                    tableBody.appendChild(row);
                });
                setupDeleteListeners();
            })
            .catch((error) => console.error("Error refreshing accounts:", error));
    }

    function setupDeleteListeners() {
        const deleteLinks = document.querySelectorAll(".delete-account");
        deleteLinks.forEach((link) => {
            link.addEventListener("click", (event) => {
                event.preventDefault();
                const accountId = link.dataset.id;
                const employeeId = link.dataset.employeeId;

                console.log("Delete clicked for accountId:", accountId, "employeeId:", employeeId);

                if (!accountId || !employeeId) {
                    console.error("Missing accountId or employeeId");
                    alert("Chybí identifikátor účtu nebo zaměstnance.");
                    return;
                }

                if (confirm("Opravdu chcete smazat tento účet?")) {
                    fetch(`/api/employees/${employeeId}/accounts/${accountId}`, {
                        method: "DELETE",
                        headers: { Accept: "application/json" },
                    })
                        .then((response) => {
                            console.log("Delete response status:", response.status);
                            if (!response.ok) {
                                return response.json().then((error) => {
                                    console.error("Delete error:", error);
                                    throw new Error(error.error || "Nastala chyba při mazání účtu.");
                                });
                            }
                            alert("Účet byl úspěšně smazán.");
                            refreshAccounts(employeeId);
                        })
                        .catch((error) => {
                            console.error("Error deleting account:", error.message);
                            alert(error.message);
                        });
                }
            });
        });
    }

    if (form) {
        form.addEventListener("submit", (event) => {
            event.preventDefault();

            const data = getDataFromForm(form);
            const employeeId = form.dataset.create.split("/")[3];

            console.log("Submitting form data:", data);

            fetch(form.dataset.create, {
                method: "POST",
                headers: {
                    Accept: "application/json",
                    "Content-Type": "application/json",
                },
                body: JSON.stringify(data),
            })
                .then((response) => {
                    console.log("Create account response status:", response.status);
                    if (!response.ok) {
                        return response.json().then((error) => {
                            console.error("Create account error:", error);
                            throw new Error(error.error || "Nastala chyba při vytváření účtu.");
                        });
                    }
                    return response.json();
                })
                .then(() => {
                    alert("Účet byl úspěšně vytvořen.");
                    form.reset();
                    refreshAccounts(employeeId);
                })
                .catch((error) => {
                    console.error("Error:", error.message);
                    alert(error.message);
                });
        });
    }

    setupDeleteListeners();
});
