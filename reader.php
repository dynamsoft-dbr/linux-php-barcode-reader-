<?php

$filename = "AllSupportedBarcodeTypes.tif";
if (file_exists($filename)) {
  echo "Barcode file: $filename \n";

  // https://www.dynamsoft.com/CustomerPortal/Portal/Triallicense.aspx
  DBRInitLicense("LICENSE-KEY");
  // DBRInitLicenseFromServer("Server-Site", "License-Key");

  //Best coverage settings
  DBRInitRuntimeSettingsWithString("{\"ImageParameter\":{\"Name\":\"BestCoverage\",\"DeblurLevel\":9,\"ExpectedBarcodesCount\":512,\"ScaleDownThreshold\":100000,\"LocalizationModes\":[{\"Mode\":\"LM_CONNECTED_BLOCKS\"},{\"Mode\":\"LM_SCAN_DIRECTLY\"},{\"Mode\":\"LM_STATISTICS\"},{\"Mode\":\"LM_LINES\"},{\"Mode\":\"LM_STATISTICS_MARKS\"}],\"GrayscaleTransformationModes\":[{\"Mode\":\"GTM_ORIGINAL\"},{\"Mode\":\"GTM_INVERTED\"}]}}");		
  //Best speend settings
  //DBRInitRuntimeSettingsWithString("{\"ImageParameter\":{\"Name\":\"BestSpeed\",\"DeblurLevel\":3,\"ExpectedBarcodesCount\":512,\"LocalizationModes\":[{\"Mode\":\"LM_SCAN_DIRECTLY\"}],\"TextFilterModes\":[{\"MinImageDimension\":262144,\"Mode\":\"TFM_GENERAL_CONTOUR\"}]}}");
  //Balance settings
  //DBRInitRuntimeSettingsWithString("{\"ImageParameter\":{\"Name\":\"Balance\",\"DeblurLevel\":5,\"ExpectedBarcodesCount\":512,\"LocalizationModes\":[{\"Mode\":\"LM_CONNECTED_BLOCKS\"},{\"Mode\":\"LM_STATISTICS\"}]}}");

  //Or you can init runtime settings with file.
  //$templateFile = "template.json";
  //DBRInitRuntimeSettingsWithFile($templateFile);

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
      echo "Localization : ", $result[3], "\n";
    }
  } else {
    echo "$resultArray[0]";
  }
} else {
  echo "The file $filename does not exist";
}
?>
