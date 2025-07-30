<?php

$pdo = require_once 'connect.php';

// Keep this API Key value to be compatible with the ESP32 code provided in the project page. 
// If you change this value, the ESP32 sketch needs to match
// Look at /src/postdata.h
$api_key_value = "your api key";
$api_key= $sensor = $location = $value1 = $value2 = $value3 = "";
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
        $sensor = test_input($_POST["sensor"]);
        $location = test_input($_POST["location"]);
        $value1 = test_input($_POST["value1"]);
        $value2 = test_input($_POST["value2"]);
        $value3 = test_input($_POST["value3"]);
      // prepare statement for insert
        $sql = "INSERT INTO SensorData (sensor, location, temp, humidity, other) VALUES (:sensor, :location, :temp, :humidity, :other)";
        $stmt = $pdo->prepare($sql);
        
        $value1 = strtr($value1, '.', ',');
        // pass values to the statement
	$stmt->bindValue(':sensor', $sensor);
	$stmt->bindValue(':location', $location);
	$stmt->bindValue(':temp', $value1);
	$stmt->bindValue(':humidity', $value2);
	$stmt->bindValue(':other', $value3);
        
        // execute the insert statement
        $stmt->execute();
	$pdo= null;
    }
    else {
        echo "Wrong API Key provided.";
    }

}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
