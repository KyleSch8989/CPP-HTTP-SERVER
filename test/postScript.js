function submitForm () {
	const textFieldValue = document.getElementById('textField').value;

	const data = {
		textField: textFieldValue
	};

	const xhr = new XMLHttpRequest();

	xhr.open('POST', '/feedback');
	xhr.setRequestHeader('Content-Type', 'text/plain');

	xhr.onload = function() {
		if (xhr.status >= 200 && xhr.status < 300) {
			console.log('Sucessful response from server: ', xhr.status);
		} else {
			console.log('request failed with status: ', xhr.status);
		}
	};

	xhr.send(textFieldValue);
}
