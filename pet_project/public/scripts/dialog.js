class DialogHandler {
    #dialog = null;
    #dialogMessage = null;
    #confirmButton = null;

    constructor(button, options = {}) {
        this.#dialog = document.getElementById(options.dialogId || 'dialog');
        this.#dialogMessage = document.getElementById(options.messageId || 'dialog-message');
        this.#confirmButton = document.getElementById(options.confirmId || 'confirm-button');
        this.#initDialog(button, options);
    }

    #initDialog(button, options) {
        const data = {
            messageTemplate: options.messageTemplate || 'Opravdu chcete pokračovat s uživatelem {name}?',
            actionUrl: button.getAttribute(options.dataUrlAttribute || 'data-url'),
            name: button.getAttribute(options.dataNameAttribute || 'data-name') || button.textContent.trim(),
        };

        button.addEventListener('click', (event) => {
            this.#openDialog(event, data, options.onConfirm || null);
        });

        document.getElementById(options.cancelId || 'cancel-button').addEventListener('click', () => {
            this.#closeDialog();
        });
    }

    #openDialog(event, data, onConfirm) {
        event.preventDefault();

        const message = data.messageTemplate.replace('{name}', data.name);
        this.#dialogMessage.textContent = message;
        this.#confirmButton.setAttribute('href', data.actionUrl);

        if (onConfirm) {
            this.#confirmButton.addEventListener('click', (event) => {
                event.preventDefault();
                onConfirm(data.actionUrl);
                this.#closeDialog();
            }, { once: true });
        }

        this.#dialog.showModal();
    }

    #closeDialog() {
        this.#dialog.close();
    }
}

// Usage
document.querySelectorAll('.open-dialog').forEach((button) => {
    button.dialogHandler = new DialogHandler(button, {
        dialogId: 'user-dialog',
        messageId: 'dialog-message',
        confirmId: 'confirm-dialog',
        cancelId: 'cancel-dialog',
        dataUrlAttribute: 'data-detail-url',
        dataNameAttribute: 'data-user-name',
        messageTemplate: 'Opravdu chcete přejít na detail uživatele {name}?',
        onConfirm: (url) => {
            console.log(`Confirmed action for: ${url}`);
            window.location.href = url;
        },
    });
});
