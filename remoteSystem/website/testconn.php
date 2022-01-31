<?php

$servername='localhost';
$username='root';
$password='Password123#@!';
$database='slipad';

$con = mysqli_connect($servername, $username, $password, $database);

// Checking for connections
if ($con->connect_error) {
    die('Connect Error (' . 
    $con->connect_errno . ') '. 
    $con->connect_error);
}

// echo "Connect successfull\n";

$sql="SELECT latitude, longitude FROM location WHERE id IN (SELECT id FROM
 parking_space WHERE num_vacants=0)";
$result=mysqli_query($con, $sql);

while($rows=$result->fetch_assoc())
{
    $latitude=$rows['latitude'];
    $longitude=$rows['longitude'];
    echo "$latitude $longitude \n";
}

?>
