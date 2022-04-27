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


if(isset($_POST['device_status']))
  {
  	if($_POST['device_status']==1)
    {
      $device_status=$_POST['device_status'];
      $sql="UPDATE `device_control_app` SET `device_status`='$device_status' WHERE `id`='1'";
      $qry=mysqli_query($conn,$sql) OR die('error');
      $successdata =   array('success' => TRUE, 'status' => 'Light Is On','messssage' => 'Data Successfully Updated!');
      header('Content-Type: application/json; charset=utf-8');
      echo json_encode($successdata);
    }
  	else
    {
      $device_status=$_POST['device_status'];
      $sql="UPDATE `device_control_app` SET `device_status`='$device_status' WHERE `id`='1'";
      $qry=mysqli_query($conn,$sql) OR die('error');
      $successdata =   array('success' => TRUE, 'status' => 'Light Is Off','messssage' => 'Data Successfully Updated!');
      header('Content-Type: application/json; charset=utf-8');
      echo json_encode($successdata);  
    }
  	
  //$data =   array('success' => TRUE, 'messssage' => 'Data Successfully Updated!');
  //header('Content-Type: application/json; charset=utf-8');
  //echo json_encode($data);
}
else{
  $data =   array('success' => FALSE, 'messssage' => 'Data Not Updated!');
  header('Content-Type: application/json; charset=utf-8');
  echo json_encode($data);
}
?>