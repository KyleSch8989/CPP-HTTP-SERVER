console.log("hello world");

document.getElementById('sendRequest').addEventListener('click', function () {
	var xhr = new XMLHttpRequest();
	xhr.open('GET', 'data.xml', true);
	xhr.onload = function () {
		if (xhr.status === 200) {
			console.log(xhr.responseText);
		} else {
			console.error('Request failed with status: ', xhr.status);
		}
	};
	xhr.onerror = function() {
		console.error('Request failed');
	};
	xhr.send();
});
