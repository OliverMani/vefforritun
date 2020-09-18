function clear() {
	document.body.innerHTML = '';
}

function loadsite(site) {
	rerenderMenu();

}

function rerenderMenu() {
	clear();
	let context = document.createElement('div');
	context.className = 'menu';

	let logo = document.createElement('img');
	logo.src = '/static/logo.svg';
	logo.width = 250;
	logo.height = 50;
	logo.className = 'logo';

	context.appendChild(logo);
	document.body.appendChild(context);
}

(function(){
	clear();
	rerenderMenu();

})();