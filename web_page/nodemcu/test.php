<?php



    $host = "localhost";		         // host = localhost because database hosted on the same server where PHP files are hosted
    $dbname = "student1_nodemcu";              // Database name
    $username = "student1_nodemcu";		// Database username
    $password = "nodeMCU@#1";	        // Database password


// Establish connection to MySQL database
$conn = new mysqli($host, $username, $password, $dbname);


// Check if connection established successfully
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

else { echo "Connected to mysql database. "; }

   


  if(!empty($_POST['sendval']))
    {
		echo $val = $_POST['sendval'];
                $val2 = $_POST['sendval2'];


// Update your tablename here
	        $sql = "INSERT INTO test (val,val2) VALUES ('".$val."', '".$val2."')"; 
 


		if ($conn->query($sql) === TRUE) {
		    echo "Values inserted in MySQL database table.";
		} else {
		    echo "Error: " . $sql . "<br>" . $conn->error;
		}
	}


// Close MySQL connection
$conn->close();



?>
