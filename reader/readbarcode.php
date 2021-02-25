<?php
function readBarcode($path, $type) {
  try {  	
	DBRInitLicense("LICENSE-KEY");
  	$resultArray = DecodeBarcodeFile($path, $type);
  } catch(Exception $exp) {
  	echo '<p>' . $exp . '</p>';
  	exit;
  }

  if (is_array($resultArray[0])) {
	$resultCount = count($resultArray);
	echo '<p>Total barcode(s) found:' . $resultCount . '.</p><br/>';
	for($i = 0; $i < $resultCount ; $i++) {
		$result = $resultArray[$i];
		echo '<p>Barcode ' . ($i+1) . ':</p>';
  	 	echo "<p>Type: $result[0]</p>";
  	 	echo "<p>Value: $result[1] </p><br/>"; 
		echo "<p>Localization: $result[3] </p><br/>"; 
	}
  }
  else {
    echo '<p>No barcodes found.</p>';
  }
}

function imagefromURL($image,$rename){
	$ch = curl_init($image);

	
	$timeout = 5;
	curl_setopt($ch, CURLOPT_HEADER, 0);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt($ch, CURLOPT_BINARYTRANSFER, 1);
	curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, $timeout); 
	$rawdata=curl_exec ($ch);
	
	$code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
	if($code > 200) {
		curl_close($ch);
		return FALSE;
	}
	
	curl_close ($ch);
		
	$fp = fopen($rename,'wb');
	fwrite($fp, $rawdata); 
	fclose($fp);

	return TRUE;
}
	
function return_bytes($val) {
    $val = trim($val);
    $last = strtolower($val[strlen($val)-1]);
    switch($last) {

        case 'g':
            $val *= 1024;
        case 'm':
            $val *= 1024;
        case 'k':
            $val *= 1024;
    }

    return $val;
}

ini_set('display_errors',1);
error_reporting(E_ALL);

$post_max_size = ini_get("post_max_size");
$maxsize = return_bytes($post_max_size);

if($_SERVER['CONTENT_LENGTH'] > $maxsize) {
	echo "Post data size is bigger than " . $post_max_size;
	exit;
}

if(!array_key_exists("uploadFlag", $_POST))	{
		echo "The input file is not specificed.";
		exit;
}

$flag = (int)$_POST["uploadFlag"];
$btype = (int)$_POST["barcodetype"];

// get current working directory
$root = getcwd();
// tmp dir for receiving uploaded barcode images
$tmpDir = $root . "/uploads/";
if (!file_exists($tmpDir)) {
	mkdir($tmpDir);
}

if ($flag) {
	if(!empty($_FILES["fileToUpload"]["tmp_name"]))	{
		$file = basename($_FILES["fileToUpload"]["tmp_name"]);
		$tmpname = date("Y_m_d_H_i_s_") . rand()%1000;
		
		if ($file != NULL && $file != "") {
			$target_file = $tmpDir . $tmpname;
			if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {     
				readBarcode($target_file, $btype);					
			  unlink($target_file);
			} else {
				echo "Fail to upload file.";
			}
		} else {
		  echo "Fail to upload file.";
		}
	}
	else {
		echo "Fail to upload file.";
	}
} else {	
	if (!empty($_POST["fileToDownload"]) && $_POST["fileToDownload"] != "") {
		$url_file = $_POST["fileToDownload"];
		$tmpname = date("Y_m_d_H_i_s_") . rand()%1000;
		$target_file = $tmpDir . $tmpname;	
		
		if( imagefromURL($url_file, $target_file)) {
			readBarcode($target_file, $btype);		
			#unlink($target_file);
		} else {
			echo "Fail to download file.";
		}
	} else {
		echo "Fail to download file.";
	}
}

?>
