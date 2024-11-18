<?php  

require 'database.php';  

if (!empty($_POST)) {  
    // keep track post values  
    $name = $_POST['name'];  
    $id = $_POST['id'];  
    $gender = $_POST['gender'];  
    $email = $_POST['email'];  
    $mobile = $_POST['mobile'];  

    // connect to database  
    $pdo = Database::connect();  
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);  
    
    // Check for existing entries  
    $sqlCheckId = "SELECT COUNT(*) FROM table_nodemcu_rfidrc522_mysql WHERE id = ?";  
    $checkIdStmt = $pdo->prepare($sqlCheckId);  
    $checkIdStmt->execute(array($id));  
    $idExists = $checkIdStmt->fetchColumn() > 0;  

    $sqlCheckEmail = "SELECT COUNT(*) FROM table_nodemcu_rfidrc522_mysql WHERE email = ?";  
    $checkEmailStmt = $pdo->prepare($sqlCheckEmail);  
    $checkEmailStmt->execute(array($email));  
    $emailExists = $checkEmailStmt->fetchColumn() > 0;  

    $sqlCheckMobile = "SELECT COUNT(*) FROM table_nodemcu_rfidrc522_mysql WHERE mobile = ?";  
    $checkMobileStmt = $pdo->prepare($sqlCheckMobile);  
    $checkMobileStmt->execute(array($mobile));  
    $mobileExists = $checkMobileStmt->fetchColumn() > 0;  

    // Prepare error messages  
    $errorMessages = [];  

    if ($idExists) {  
        $errorMessages[] = "ID ya registrado, intente otro.";  
    }  
    if ($emailExists) {  
        $errorMessages[] = "Email ya registrado, intente otro.";  
    }  
    if ($mobileExists) {  
        $errorMessages[] = "Número móvil ya registrado, intente otro.";  
    }  

    // If there are any error messages, redirect with them  
    if (!empty($errorMessages)) {  
        Database::disconnect();  
        
        // Store error messages in session variable  
        session_start();  
        $_SESSION['error_messages'] = $errorMessages;  
        
        header("Location: registration.php"); // Redirigir a la página del formulario  
        exit();  
    } else {  
        // Proceed with the data insertion  
        $sql = "INSERT INTO table_nodemcu_rfidrc522_mysql (name, id, gender, email, mobile) VALUES (?, ?, ?, ?, ?)";  
        $q = $pdo->prepare($sql);  
        $q->execute(array($name, $id, $gender, $email, $mobile));  
        Database::disconnect();  

        header("Location: user_data.php");  
        exit();  
    }  
}  
?>