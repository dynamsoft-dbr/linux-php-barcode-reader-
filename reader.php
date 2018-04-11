<?php

$filename = "/opt/dynamsoft/dbr/images/AllSupportedBarcodeTypes.tif";
if (file_exists($filename)) {
  echo "Barcode file: $filename \n";
  /*
   * Description:
   * array DecodeBarcodeFile( string $filename , long $type )
   *
   * Returned value:
   * If succeed, it is an array.
   */
  $resultArray = DecodeBarcodeFile($filename, 0x3FF | 0x2000000 | 0x8000000 | 0x4000000);
  if (is_array($resultArray)) {
	$resultCount = count($resultArray);
	echo "Total count: $resultCount\n";
	for($i = 0; $i < $resultCount ; $i++) {
		$result = $resultArray[$i];
        	echo "Barcode format: $result[0], value: $result[1]\n";
	}
  }
  else {
    echo "$resultArray[0]";
  }
} else {
    echo "The file $filename does not exist";
}
?>
