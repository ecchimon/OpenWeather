<!DOCTYPE html>
<html>
  <head>
    <title>Controle de Temperatura</title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">    
    <link rel="stylesheet" href="../admin/style.css">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css" integrity="sha512-iecdLmaskl7CVkqkXNQ/ZH/XLlvWZOJyj7Yy7tcenmpD1ypASozpmT/E0iPtmFIB46ZmdtAc9eNBvH0H/ZpiBw==" crossorigin="anonymous" referrerpolicy="no-referrer" />

    <style>
        table {
            font-family: arial, sans-serif;
            border-collapse: collapse;
            width: 100%;
        }

        td, th {
            border: 1px solid #dddddd;
            text-align: center;
            padding: 8px;
        }

        tr:nth-child(even) {
            background-color: #dddddd;
        }
    </style>

  </head>
<body>
<div class="content">
	<div class="separador">
            <img src="../intranet/img/petala.png" width="30" height="30" alt"">
            <h1 style="text-align:center;">CONTROLE DE TEMPERATURA</h1>
        </div>
	<div classs="mat">


<?php


$pdo = require_once 'connect.php';


echo '<table cellspacing="5" cellpadding="5">
      <tr bgcolor="#00CCFF"> 
        <td align="center" bgcolor="#999999">ID</td> 
	<td align="center" bgcolor="#999999">Data</td>
	<td align="center" bgcolor="#999999">Hora</td>
        <td align="center" bgcolor="#999999">Sensor</td> 
        <td align="center" bgcolor="#999999">Localização</td> 
        <td align="center" bgcolor="#999999">Temperatura</td> 
        <td align="center" bgcolor="#999999">Umidade</td>
        <td align="center" bgcolor="#999999">Pressão</td> 
      </tr>';


$stmt = $pdo->query('SELECT * FROM sensordata WHERE extract(month from (sensordata."timestamp")  ) = extract (month from now()) order by id desc');
//$stmt = $pdo->query('SELECT id, sensor, location, temp, humidity, other, timestamp FROM SensorData ORDER BY id DESC');
        $linhas = [];
        while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
		
        	$row_timestamp = $row["timestamp"];
		$data = substr($row_timestamp, 0, 10);
		$hora = substr($row_timestamp, 11, 5);
		$data = explode("-", $data);
		$data = $data[2]."/".$data[1]."/".$data[0];

	        $row_id = $row["id"];
        	$row_sensor = $row["sensor"];
        	$row_location = $row["location"];
        	$row_value1 = $row["temp"];
        	$row_value2 = $row["humidity"]; 
        	$row_value3 = $row["other"]; 
		
	        echo '<tr> 
                <td>' . $row_id . '</td>
                <td>' . $data . '</td>
                <td>' . $hora . '</td>
                <td>' . $row_sensor . '</td> 
                <td>' . $row_location . '</td> 
                <td>' . $row_value1 ."ºC". '</td> 
                <td>' . $row_value2 ."%". '</td>
                <td>' . $row_value3 . '</td> 
              </tr>';
        }

 
$pdo = null;
?> 
</table>
</div>
</div>
</body>
</html>
