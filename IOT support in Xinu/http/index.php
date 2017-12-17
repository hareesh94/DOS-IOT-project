<!doctype html>
<style>
.buttong {
  padding: 15px 25px;
  font-size: 24px;
  text-align: center;
  cursor: pointer;
  outline: none;
  color: #fff;
  background-color: #4CAF50;
  border: none;
  border-radius: 15px;
  box-shadow: 0 9px #999;
}
.buttonr{
  padding: 15px 25px;
  font-size: 24px;
  text-align: center;
  cursor: pointer;
  outline: none;
  color: #fff;
  background-color: #F71E1E;
  border: none;
  border-radius: 15px;
  box-shadow: 0 9px #999;
}
.my-block {
            text-align: center;
            display: table-cell;
            vertical-align: middle;
}

.buttong:hover {background-color: #3e8e41}

.buttong:active {
  background-color: #3e8e41;
  box-shadow: 0 5px #666;
  transform: translateY(4px);
}


.buttonr:hover {background-color: #FF0000}

.buttonr:active {
  background-color: #3e8e41;
  box-shadow: 0 5px #666;
  transform: translateY(4px);
}


</style>
<html lang="en">
<head>
  <meta charset="utf-8">
<meta http-equiv="refresh" content="50" />
    <!-- This will refresh page in every 5 seconds, change content= x to refresh page after x seconds -->
</head>

  <title>The IOT CLOUD</title>
  <meta name="description" content="control your IOT">
  <meta name="author" content="Ashish">
  <script src="jquery.js"></script>
    <script>
        //Usually, you put script-tags into the head
        function glow() {
            //This performs a POST-Request.
            //Use "$.get();" in order to perform a GET-Request (you have to take a look in the rest-API-documentation, if you're unsure what you need)
            //The Browser downloads the webpage from the given url, and returns the data.
            $.post( "http://192.168.11.112:8080?Glowled=1&Status=true", function( data ) {
                 //As soon as the browser finished downloading, this function is called.
                 $('#demo').html(data);
            });
        }

	function off() {
            //This performs a POST-Request.
            //Use "$.get();" in order to perform a GET-Request (you have to take a look in the rest-API-documentation, if you're unsure what you need)
            //The Browser downloads the webpage from the given url, and returns the data.
            $.post( "http://192.168.11.112:8080?Glowled=1&Status=false", function( data ) {
                 //As soon as the browser finished downloading, this function is called.
                 $('#demo').html(data);
            });
        }
	function iotShutdown() {
            //This performs a POST-Request.
            //Use "$.get();" in order to perform a GET-Request (you have to take a look in the rest-API-documentation, if you're unsure what you need)
            //The Browser downloads the webpage from the given url, and returns the data.
            $.post( "http://192.168.11.112:8080?Glowled=9&Status=false", function( data ) {
                 //As soon as the browser finished downloading, this function is called.
                 $('#demo').html(data);
            });
        }
	function iotStartup() {
            //This performs a POST-Request.
            //Use "$.get();" in order to perform a GET-Request (you have to take a look in the rest-API-documentation, if you're unsure what you need)
            //The Browser downloads the webpage from the given url, and returns the data.
            $.post( "http://192.168.11.112:8080?Glowled=9&Status=true", function( data ) {
                 //As soon as the browser finished downloading, this function is called.
                 $('#demo').html(data);
            });
        }

	function checkStatus() {
            //This performs a POST-Request.
            //Use "$.get();" in order to perform a GET-Request (you have to take a look in the rest-API-documentation, if you're unsure what you need)
            //The Browser downloads the webpage from the given url, and returns the data.
            $.post( "http://192.168.11.112:8080?Glowled=8&Status=true", function( data ) {
                 //As soon as the browser finished downloading, this function is called.
                 $('#demo').html(data);
            });
        }
    
    </script>

</head>

<body>

<?php
//phpinfo();
$string = file_get_contents("http://192.168.11.112:8080?Glowled=8&Status=false");
?>



<h1>  IOT COMMAND CENTER : CURRENT LIGHT READING IS <?php echo $string ?>  </h1>

   <button class="buttong" onclick="glow()">GLOW LED</button> 
  <button class="buttonr" onclick="off()">TURN OFF LED</button>

   <button class="buttong" onclick="iotStartup()">START SENSOR</button> 
  <button class="buttonr" onclick="iotShutdown()">STOP SENSOR</button> 
   <button class="buttong" onclick="checkStatus()">Check Status</button> 

</body>
</html>





