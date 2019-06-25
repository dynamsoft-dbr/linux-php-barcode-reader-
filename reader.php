<?php

$filename = "AllSupportedBarcodeTypes.tif";
if (file_exists($filename)) {
  echo "Barcode file: $filename \n";

  // https://www.dynamsoft.com/CustomerPortal/Portal/Triallicense.aspx
  DBRInitLicense("LICENSE-KEY");

  /*
   * Description:
   * array DecodeBarcodeFile( string $filename , long $type )
   *
   * Returned value:
   * If succeed, it is an array.
   */
  $resultArray = DecodeBarcodeFile($filename, 0x3FF | 0x2000000 | 0x4000000 | 0x8000000 | 0x10000000); // 1D, PDF417, QRCODE, DataMatrix, Aztec Code

  if (is_array($resultArray)) {
    $resultCount = count($resultArray);
    echo "Total count: $resultCount\n";
    for ($i = 0; $i < $resultCount; $i++) {
      $result = $resultArray[$i];
      echo "Barcode format: $result[0], ";
      echo "value: $result[1], ";
      echo "raw: ", bin2hex($result[2]), "\n";
    }
  } else {
    echo "$resultArray[0]";
  }
} else {
  echo "The file $filename does not exist";
}
?>
