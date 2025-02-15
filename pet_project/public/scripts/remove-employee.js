class DeleteEmployeeHandler {
    #deleteButtons = null;

    constructor(deleteButtonSelector) {
        this.#deleteButtons = document.querySelectorAll(deleteButtonSelector);
        this.#init();
    }

    #init() {
        this.#deleteButtons.forEach(button => {
            button.addEventListener('click', (event) => this.#handleDeleteButtonClick(event, button));
        });
    }

    #handleDeleteButtonClick(event, button) {
        event.preventDefault();

        const deleteUrl = button.getAttribute('data-delete-url');
        const firstName = button.getAttribute('data-first-name');
        const lastName = button.getAttribute('data-last-name');

        if (this.#confirmDelete(firstName, lastName)) {
            if (window.fetch) {
                this.#deleteEmployeeViaAjax(deleteUrl);
            } else {
                this.#fallbackToFormSubmission(deleteUrl);
            }
        }
    }

    #confirmDelete(firstName, lastName) {
        return confirm(`Opravdu chcete smazat zaměstnance ${firstName} ${lastName}?`);
    }

    #deleteEmployeeViaAjax(deleteUrl) {
        fetch(deleteUrl, { method: 'POST' })
            .then(response => {
                if (response.ok) {
                    alert('Zaměstnanec byl úspěšně odstraněn.');
                    window.location.href = '/employees';
                } else {
                    alert('Chyba při odstraňování zaměstnance.');
                }
            })
            .catch(error => {
                console.error('Error:', error);
                alert('Nastala chyba při komunikaci se serverem.');
            });
    }

    #fallbackToFormSubmission(deleteUrl) {
        const form = document.createElement('form');
        form.action = deleteUrl;
        form.method = 'POST';

        const csrfToken = document.querySelector('meta[name="csrf-token"]')?.content;
        if (csrfToken) {
            const csrfInput = document.createElement('input');
            csrfInput.type = 'hidden';
            csrfInput.name = '_token';
            csrfInput.value = csrfToken;
            form.appendChild(csrfInput);
        }

        document.body.appendChild(form);
        form.submit();
    }
}

document.addEventListener('DOMContentLoaded', () => {
    new DeleteEmployeeHandler('.delete-employee');
});
