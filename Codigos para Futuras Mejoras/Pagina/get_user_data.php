<?php
// Conexión a la base de datos
$dbHost = 'localhost';
$dbName = 'nodemcu_rfidrc522_mysql';
$dbUsername = 'root';
$dbPassword = 'mypassword';

$conn = new mysqli($dbHost, $dbUsername, $dbPassword, $dbName);

// Verificar conexión
if ($conn->connect_error) {
    die(json_encode(['error' => 'Database connection failed']));
}

// Obtener el ID desde la solicitud
$id = isset($_GET['id']) ? $_GET['id'] : '';

// Consultar la base de datos
$sql = "SELECT name FROM table_nodemcu_rfidrc522_mysql WHERE id = ?";
$stmt = $conn->prepare($sql);
$stmt->bind_param("s", $id);
$stmt->execute();
$result = $stmt->get_result();

if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();
    echo json_encode(['success' => true, 'name' => $row['name']]);
} else {
    echo json_encode(['success' => false, 'message' => 'ID not found']);
}

$stmt->close();
$conn->close();


/*
$servername = "localhost";
$username = "root";
$password = "mypassword";
$dbname = "nodemcu_rfidrc522_mysql";

// Conectar a la base de datos
$conn = new mysqli($servername, $username, $password, $dbname);

// Verificar la conexión
if ($conn->connect_error) {
    die("Conexión fallida: " . $conn->connect_error);
}

if ($_SERVER["REQUEST_METHOD"] == "GET" && isset($_GET["UIDresult"])) {
    $UIDresult = $_GET["UIDresult"];

    // Inicializar variable para la respuesta
    $response = "";

    // Verificar si el UID está en la base de datos
    $sql = "SELECT name FROM table_nodemcu_rfidrc522_mysql WHERE id='$UIDresult'";
    $result = $conn->query($sql);

    if ($result->num_rows > 0) {
        // Si el UID está en la base de datos, obtener el nombre
        $row = $result->fetch_assoc();
        $name = $row['name'];
        $response = "Hola: " . $name;
    } else {
        // Si no está en la base de datos, enviar un mensaje para registrar el UID
        $response = "Registrar UID: " . $UIDresult;
    }

    // Enviar la respuesta
    echo $response;
}

// Cerrar la conexión
$conn->close();*/
?>
