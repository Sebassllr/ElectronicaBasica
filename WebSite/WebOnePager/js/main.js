window.onload = function () {

	$(function(){
		var dps = []; // dataPoints
		var chart = new CanvasJS.Chart("chartContainer", {
					title :{
						text: "¡Tus datos en tiempo real!"
					},
					axisY: {
						includeZero: false
					},      
					data: [{
						lineColor: "Black",

						type: "line",
						dataPoints: dps
					}]
				});

				var xVal = 0;
				var yVal = 100; 
				var updateInterval = 1000;
				var dataLength = 20; // number of dataPoints visible at any point

				var updateChart = function (count) {

					count = count || 1;

					for (var j = 0; j < count; j++) {
						yVal = yVal +  Math.round(5 + Math.random() *(-5-5));
						dps.push({
							x: xVal,
							y: yVal
						});
						xVal++;
					}

					if (dps.length > dataLength) {
						dps.shift();
					}

					chart.render();
				};

				updateChart(dataLength);
				setInterval(function(){updateChart()}, updateInterval);
				$("#chartContainer").CanvasJSChart(options);
			});

	$(function(){
		var chart = new CanvasJS.Chart("ColumnCharts", {
			title: {
				text: "¡Tú consumo diario!"
			},
			axisY: {
				title: "Temperature (°C)",
				suffix: " °C"
			},
			data: [{
				type: "column",	
				yValueFormatString: "#,### °C",
				indexLabel: "{y}",
				dataPoints: [
					{ label: "boiler1", y: 206 },
					{ label: "boiler2", y: 163 },
					{ label: "boiler3", y: 154 },
					{ label: "boiler4", y: 176 },
					{ label: "boiler5", y: 184 },
					{ label: "boiler6", y: 122 }
				]
			}]
		});

		function updateChart() {
			var boilerColor, deltaY, yVal;
			var dps = chart.options.data[0].dataPoints;
			for (var i = 0; i < dps.length; i++) {
				deltaY = Math.round(2 + Math.random() *(-2-2));
				yVal = deltaY + dps[i].y > 0 ? dps[i].y + deltaY : 0;
				boilerColor = yVal > 200 ? "#FF2500" : yVal >= 170 ? "#FF6000" : yVal < 170 ? "#6B8E23 " : null;
				dps[i] = {label: "Boiler "+(i+1) , y: yVal, color: boilerColor};
			}
			chart.options.data[0].dataPoints = dps; 
			chart.render();
		};
		updateChart();

		setInterval(function() {updateChart()}, 500);
		$("#ColumnCharts").CanvasJSChart(options);
	});
}