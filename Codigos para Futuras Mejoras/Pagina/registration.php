<!DOCTYPE html>  
<html lang="en">  
<html>  
    <head>  
        <meta name="viewport" content="width=device-width, initial-scale=1.0">  
        <meta charset="utf-8">  
        <link href="css/bootstrap.min.css" rel="stylesheet">  
        <script src="js/bootstrap.min.js"></script>  
        <script src="jquery.min.js"></script>  
        <script>  
            $(document).ready(function(){  
                $("#getUID").load("UIDContainer.php");  
                setInterval(function() {  
                    $("#getUID").load("UIDContainer.php");  
                }, 500);  
            });  
        </script>  
        
		<style>
		html {
			font-family: Arial;
			display: inline-block;
			margin: 0px auto;
		}
		
		textarea {
			resize: none;
		}

		ul.topnav {
			list-style-type: none;
			margin: auto;
			padding: 0;
			overflow: hidden;
			background-color: #4CAF50;
			width: 70%;
		}

		ul.topnav li {float: left;}

		ul.topnav li a {
			display: block;
			color: white;
			text-align: center;
			padding: 14px 16px;
			text-decoration: none;
		}

		ul.topnav li a:hover:not(.active) {background-color: #3e8e41;}

		ul.topnav li a.active {background-color: #333;}

		ul.topnav li.right {float: right;}

		@media screen and (max-width: 600px) {
			ul.topnav li.right, 
			ul.topnav li {float: none;}
		}
		</style>  
        
        <title>Registration : Competencia Labo III</title>  
    </head>  
    
    <body>  

        <h2 align="center">Competencia Labo III</h2>  
        <ul class="topnav">  
            <li><a href="home.php">Home</a></li>  
            <li><a href="user data.php">User Data</a></li>  
            <li><a class="active" href="registration.php">Registration</a></li>  
            <li><a href="read tag.php">Read Tag ID</a></li>  
        </ul>  

        <div class="container">  
            <br>  
            <div class="center" style="margin: 0 auto; width:495px; border-style: solid; border-color: #f2f2f2;">  
                <div class="row">  
                    <h3 align="center">Registrar Personal</h3>  

                    <?php  
                    session_start();  
                    if (isset($_SESSION['error_messages'])) {  
                        echo '<div class="alert alert-danger">';  
                        foreach ($_SESSION['error_messages'] as $msg) {  
                            echo '<p>' . htmlspecialchars($msg) . '</p>';  
                        }  
                        echo '</div>';  
                        unset($_SESSION['error_messages']); // Limpiar los mensajes después de mostrarlos  
                    }  
                    ?>  
                </div>  
                <br>  
                <form class="form-horizontal" action="insertDB.php" method="post" >  
                    <div class="control-group">  
                        <label class="control-label">ID</label>  
                        <div class="controls">  
                            <textarea name="id" id="getUID" placeholder="Ingresar ID de la Tarjeta" rows="1" cols="1" required></textarea>  
                        </div>  
                    </div>  
                    
                    <div class="control-group">  
                        <label class="control-label">Nombre</label>  
                        <div class="controls">  
                            <input id="div_refresh" name="name" type="text" placeholder="" required>  
                        </div>  
                    </div>  
                    
                    <div class="control-group">  
                        <label class="control-label">Genero</label>  
                        <div class="controls">  
                            <select name="gender">  
                                <option value="Male">Hombre</option>  
                                <option value="Female">Mujer</option>  
                            </select>  
                        </div>  
                    </div>  
                    
                    <div class="control-group">  
                        <label class="control-label">Gmail</label>  
                        <div class="controls">  
                            <input name="email" type="text" placeholder="" required>  
                        </div>  
                    </div>  
                    
                    <div class="control-group">  
                        <label class="control-label">Telefono</label>  
                        <div class="controls">  
                            <input name="mobile" type="text" placeholder="" required>  
                        </div>  
                    </div>  
                    
                    <div class="form-actions">  
                        <button type="submit" class="btn btn-success">Guardar</button>  
                    </div>  
                </form>  
                
            </div>               
        </div> <!-- /container -->    
    </body>  
</html>