class RoleAutocomplete {
    #select = null;
    #input = null;

    constructor(selectId, inputId, datalistId) {
        this.#select = document.getElementById(selectId);
        this.#input = document.getElementById(inputId);

        if (!this.#select || !this.#input) {
            console.warn(`Element nebyl nalezen: selectId="${selectId}", inputId="${inputId}".`);
            return;
        }

        this.#initialize(inputId, datalistId);
    }

    #initialize(inputId, datalistId) {
        const options = Array.from(this.#select.options).map(option => option.value);

        const datalist = document.getElementById(datalistId);
        datalist.innerHTML = '';
        options.forEach(option => {
            const optionElement = document.createElement('option');
            optionElement.value = option;
            datalist.appendChild(optionElement);
        });

        this.#select.style.display = 'none';
        this.#input.style.display = 'inline-block';

        this.#input.addEventListener('input', () => this.#updateSelectValue());
        this.#input.addEventListener('focusout', () => this.#syncSelect());
    }

    #updateSelectValue() {
        const inputValue = this.#input.value;
        const matchingOption = Array.from(this.#select.options).find(option => option.value === inputValue);
        if (matchingOption) {
            this.#select.value = inputValue;
        } else {
            this.#select.value = '';
        }
    }

    #syncSelect() {
        if (!Array.from(this.#select.options).some(option => option.value === this.#input.value)) {
            this.#input.value = '';
            this.#select.value = '';
        }
    }
}

document.addEventListener('DOMContentLoaded', () => {
    new RoleAutocomplete('role-title-select', 'role-title-input', 'role-titles');
});
