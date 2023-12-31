<!DOCTYPE html>
<html>
<head>
	<style>
		table {
			border-collapse: collapse;
			width: 100%;
			margin-bottom: 1em;
			color: #333333;
			font-family: Arial, sans-serif;
			font-size: 14px;
			text-align: left;
			background-color: #F5F5F5;
		}

		table td, table th {
			padding: 8px;
			border: 1px solid #DDDDDD;
		}

		table th {
			background-color: #B3B3B3;
			color: #FFFFFF;
			font-weight: bold;
		}

		table tr:nth-child(even) {
			background-color: #EFEFEF;
		}

		table tr:nth-child(odd) {
			background-color: #FFFFFF;
		}

    h1 {
		  text-align: center;
		}
	</style>
</head>  
<body>
<?php
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-php/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

$servername = "localhost";

// REPLACE with your Database name
$dbname = "id20558142_sensors";
// REPLACE with Database user
$username = "id20558142_ceavinrufus";
// REPLACE with Database user password
$password = "]K3}NC<cKzu~-8hg";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT id, sensor, location, temperature, pressure, altitude, reading_time FROM SensorMeasurements ORDER BY id DESC";

echo '<h1>ESP32 Database</h1>
      <table cellspacing="5" cellpadding="5">
      <tr> 
        <td>ID</td> 
        <td>Sensor</td> 
        <td>Location</td> 
        <td>Temperature (*C)</td> 
        <td>Pressure (hPa)</td>
        <td>Altitude (m)</td> 
        <td>Timestamp</td> 
      </tr>';
 
if ($result = $conn->query($sql)) {
    while ($row = $result->fetch_assoc()) {
        $row_id = $row["id"];
        $row_sensor = $row["sensor"];
        $row_location = $row["location"];
        $row_temperature = $row["temperature"];
        $row_pressure = $row["pressure"]; 
        $row_altitude = $row["altitude"]; 
        $row_reading_time = $row["reading_time"];
        // Uncomment to set timezone to - 1 hour (you can change 1 to any number)
        //$row_reading_time = date("Y-m-d H:i:s", strtotime("$row_reading_time - 1 hours"));
      
        // Uncomment to set timezone to + 7 hours (you can change 7 to any number)
        $row_reading_time = date("Y-m-d H:i:s", strtotime("$row_reading_time + 7 hours"));
      
        echo '<tr> 
                <td>' . $row_id . '</td> 
                <td>' . $row_sensor . '</td> 
                <td>' . $row_location . '</td> 
                <td>' . $row_temperature . '</td> 
                <td>' . $row_pressure . '</td>
                <td>' . $row_altitude . '</td> 
                <td>' . $row_reading_time . '</td> 
              </tr>';
    }
    $result->free();
}

$conn->close();
?> 
</table>
</body>
</html>