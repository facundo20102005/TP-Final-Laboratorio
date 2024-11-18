
<?php

	$UIDresult=$_POST["UIDresult"];
	$Write="<?php $" . "UIDresult='" . $UIDresult . "'; " . "echo $" . "UIDresult;" . " ?>";
	file_put_contents('UIDContainer.php',$Write);




/* codigo para mostrar los name en el tft
// Configuración de la base de datos
$servername = "192.168.0.87";
$username = "root";
$password = "mypassword";
$dbname = "nodemcu_rfidrc522_mysql";

// Crear la conexión
$conn = new mysqli($servername, $username, $password, $dbname);

// Verificar la conexión
if ($conn->connect_error) {
    die("Conexión fallida: " . $conn->connect_error);
}

// Realizar la consulta
$sql = "SELECT name FROM table_nodemcu_rfidrc522_mysql"; // Ajusta el nombre de la tabla y columna si es necesario
$result = $conn->query($sql);

$data = array();

// Verificar si hay resultados y guardarlos en el arreglo
if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        $data[] = $row["name"];
    }
}

// Devolver los datos en formato JSON
header('Content-Type: application/json');
echo json_encode($data);

// Cerrar la conexión
$conn->close();
*/
?>
