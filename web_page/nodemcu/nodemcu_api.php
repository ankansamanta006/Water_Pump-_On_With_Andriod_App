<?php
ob_start();
include('connection.php');
//echo '<pre>';
//print_r($_POST);
//echo '</pre>';
?>

<?php
//$json = file_get_contents('php://input');
//echo $data = json_decode($json);
$id='';

if(isset($_GET['id']))
  {
  	  $id=$_GET['id'];
      $sql="SELECT * FROM `device_control_app` WHERE id='$id'";
      $qry=mysqli_query($conn,$sql) OR die('error');
  	  $row=mysqli_fetch_array($qry);
      $device_status=$row['device_status'];
  		if($device_status=='1')
          {
            $message="Pump Is On";
          }
       else
         {
           $message="Pump Is Off";
         }
  $data=array('success' => TRUE, 'device_status' => $device_status, 'message' => $message);
  echo json_encode($data);
  
      
  }

else
 {
  $data =   array('success' => FALSE, 'messssage' => 'Data Not Updated!');
  header('Content-Type: application/json; charset=utf-8');
  echo json_encode($data);
 }
?>