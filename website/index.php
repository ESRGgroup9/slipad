<?php

// load secret variables
require_once 'vendor/autoload.php';
$dotenv = Dotenv\Dotenv::createImmutable(__DIR__);
$dotenv->load();

$servername =$_ENV['SERVER'];
$username   =$_ENV['USERNAME'];
$password   =$_ENV['PASSWORD'];
$database   =$_ENV['DATABASE'];

$con = mysqli_connect($servername, $username, $password, $database);

// Checking for connections
if ($con->connect_error) {
    die('Connect Error (' . 
    $con->connect_errno . ') '. 
    $con->connect_error);
}

$query=
"SELECT latitude, longitude, l.post_code, l.street_name, p.num_vacants, parish, county, district
FROM location l, parking_space p, region r
WHERE l.id=p.id AND r.post_code=l.post_code AND p.id IN

(SELECT id
FROM parking_space
WHERE num_vacants>=0)";

// execute query
$result=mysqli_query($con, $query);
?>

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <meta http-equiv="X-UA-Compatible" content="ie=edge">
  <title>SLiPaD</title>
  <style>
    #map{
      height:800px;
      width:100%;
    }
  </style>
</head>
<body>
  <h1>SLiPaD</h1>
  <h5>Smart Lighting with Parking Detection</h5>
  <div id="map"></div>
  <script>

    function initMap(){
      // Map options - default zoom and center location
      var options = {
        zoom:15,
        center:{lat:41.457564,lng:-8.279383} , 
      }

      // New map
      var map = new google.maps.Map(document.getElementById('map'), options);

       // Array of markers
      var markers = [];

      // fill array of markers
      // get latitude, longitude, post_code, street_name, num_vacants, parish, county, district
      var j = 0;
      <?php
      while($rows=$result->fetch_assoc())
      {
      ?>
        // get rows
        var latitude    =<?php echo $rows['latitude'] ?>;
        var longitude   =<?php echo $rows['longitude'] ?>;
        var post_code   ='<?php echo $rows['post_code'] ?>';
        var street_name ='<?php echo $rows['street_name'] ?>';
        var num_vacants =<?php echo $rows['num_vacants'] ?>;
        var parish      ='<?php echo $rows['parish'] ?>';
        var county      ='<?php echo $rows['county'] ?>';
        var district    ='<?php echo $rows['district'] ?>';
      
        var label ='<h2>'+num_vacants+' Available parking spots</h2>';
            label+='<h3>'+street_name+', ' +parish+'</h3>';
            label+='<h4>'+county+' '+post_code+'</h4>';

        markers[j] =
        {
          coords:{lat:latitude, lng:longitude},
          content:label
        };

        // fill next marker
        j++;
      <?php
      }
      ?>

      // Loop through markers
      for(var i = 0;i < markers.length;i++){
        // Add marker
        addMarker(markers[i]);
      }

      // Add Marker Function
      function addMarker(props){
        var marker = new google.maps.Marker({
          position:props.coords,
          map:map,
          //icon:props.iconImage
        });

        // Check for customicon
        // if(props.iconImage){
        //   // Set icon image
        //   marker.setIcon(props.iconImage);
        // }

        // Check content
        if(props.content){
          var infoWindow = new google.maps.InfoWindow({
            content:props.content
          });

          marker.addListener('click', function(){
            infoWindow.open(map, marker);
          });
        }
      }
    }
  </script>
  <script async defer
    src="https://maps.googleapis.com/maps/api/js?key=<?php echo $_ENV['API_KEY'] ?>&callback=initMap">
    </script>
</body>
</html>
