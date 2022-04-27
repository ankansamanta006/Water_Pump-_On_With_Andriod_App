<?php

$servername = "localhost";
$username = "student1_nodemcu";
$password = "nodeMCU@#1";
$database = "student1_nodemcu";

// Create connection
$conn = mysqli_connect($servername, $username, $password, $database);

// Check connection
if (!$conn) {
  die("Connection failed: " . mysqli_connect_error());
  
}
else{
  "Connected successfully";
 
}
?>
