const temp = document.querySelector('.temp');
const humid = document.querySelector('.humid');
const press = document.querySelector('.press');
const gas = document.querySelector('.gas');
const light = document.querySelector('.light');
const date = document.querySelector('.date');

const btn = document.querySelector('button');

const setData = async () => {
	try {
		const req = await fetch('http://weather.chickencurious.com/weather/current/');

		const data = await req.json();

		temp.textContent = data.temperature;
		humid.textContent = data.humidity;
		press.textContent = data.press;
		gas.textContent = data.gas;
		light.textContent = data.light;
		date.textContent = data.date;
	} catch (error) {
		console.log(error);
	}
};

setData(); // On initial load
