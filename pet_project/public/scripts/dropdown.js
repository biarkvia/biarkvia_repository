class Dropdown {
    #button = null;
    #menu = null;

    constructor(buttonSelector, menuSelector) {
        this.#button = document.querySelector(buttonSelector);
        this.#menu = document.querySelector(menuSelector);

        this.#button.addEventListener('click', (event) => this.#toggleMenu(event));
        document.addEventListener('click', (event) => this.#handleOutsideClick(event));
    }

    #toggleMenu(event) {
        event.preventDefault();
        const isVisible = this.#menu.style.display === 'block';
        this.#menu.style.display = isVisible ? 'none' : 'block';
    }

    #handleOutsideClick(event) {
        if (!event.target.closest('.dropdown')) {
            this.#menu.style.display = 'none';
        }
    }
}

document.addEventListener('DOMContentLoaded', () => {
    new Dropdown('.dropdown-button', '.dropdown-menu');
});