<?php  
require 'database.php';  
$id = null;  
if (!empty($_GET['id'])) {  
    $id = $_REQUEST['id'];  
}  

$pdo = Database::connect();  
$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);  
$sql = "SELECT * FROM table_nodemcu_rfidrc522_mysql WHERE id = ?";  
$q = $pdo->prepare($sql);  
$q->execute(array($id));  
$data = $q->fetch(PDO::FETCH_ASSOC);  
Database::disconnect();  

$msg = null;  

// Verificamos si no se encontraron datos para el ID  
if ($data === false) {  
    $msg = "Por Favor Ingrese los Datos del ID no registrado";  
    $data = [  
        'id' => $id,  
        'name' => "--------",  
        'gender' => "--------",  
        'email' => "--------",  
        'mobile' => "--------"  
    ];  
} else {  
    $msg = null; // Si se encontró el ID, no hay mensaje  
}  
?>  

<!DOCTYPE html>  
<html lang="en">  
<head>  
    <meta charset="utf-8">  
    <link href="css/bootstrap.min.css" rel="stylesheet">  
    <script src="js/bootstrap.min.js"></script>  
    <style>  
        td.lf {  
            padding-left: 15px;  
            padding-top: 12px;  
            padding-bottom: 12px;  
        }  
    </style>  
</head>  

<body>  
<div>  
    <form>  
        <table width="452" border="1" bordercolor="#10a0c5" align="center" cellpadding="0" cellspacing="1" bgcolor="#000" style="padding: 2px">  
            <tr>  
                <td height="40" align="center" bgcolor="#10a0c5">  
                    <font color="#FFFFFF">  
                        <b>User Data</b>  
                    </font>  
                </td>  
            </tr>  
            <tr>  
                <td bgcolor="#f9f9f9">  
                    <table width="452" border="0" align="center" cellpadding="5" cellspacing="0">  
                        <tr>  
                            <td width="113" align="left" class="lf">ID</td>  
                            <td style="font-weight:bold">:</td>  
                            <td align="left"><?php echo htmlspecialchars($data['id']); ?></td>  
                        </tr>  
                        <tr bgcolor="#f2f2f2">  
                            <td align="left" class="lf">Name</td>  
                            <td style="font-weight:bold">:</td>  
                            <td align="left"><?php echo htmlspecialchars($data['name']); ?></td>  
                        </tr>  
                        <tr>  
                            <td align="left" class="lf">Gender</td>  
                            <td style="font-weight:bold">:</td>  
                            <td align="left"><?php echo htmlspecialchars($data['gender']); ?></td>  
                        </tr>  
                        <tr bgcolor="#f2f2f2">  
                            <td align="left" class="lf">Email</td>  
                            <td style="font-weight:bold">:</td>  
                            <td align="left"><?php echo htmlspecialchars($data['email']); ?></td>  
                        </tr>  
                        <tr>  
                            <td align="left" class="lf">Mobile Number</td>  
                            <td style="font-weight:bold">:</td>  
                            <td align="left"><?php echo htmlspecialchars($data['mobile']); ?></td>  
                        </tr>  
                    </table>  
                </td>  
            </tr>  
        </table>  
    </form>  
</div>  
<p style="color:red;"><?php echo $msg; ?></p>  
</body>  
</html>