# PHP Barcode Reader for Linux

The sample demonstrates how to make PHP barcode reader on Ubuntu 14.04 with DBR (Dynamsoft Barcode Reader for Linux).

## Downloads
* [DBR v4.0.0-pre-alpha.tar.gz][1]
* [PHP Source Code][2]

## Getting Started
1. Unzip PHP source code and then change directory to **ext**:

    ```
    cd ~/Downloads/<php-version-number>/ext
    ```

2. Create an extension **dbr**:

    ```
    ./ext_skel --extname=dbr
    cd dbr
    ```

3. Add paths of include and library to **config.m4**:

    ```
    PHP_ARG_ENABLE(dbr, whether to enable dbr support,
 
    dnl Make sure that the comment is aligned:
    
    [  --enable-dbr           Enable dbr support])
    
    if test "$PHP_DBR" != "no"; then
    
        PHP_ADD_LIBRARY_WITH_PATH(DynamsoftBarcodeReaderx64, /home/xiao/Dynamsoft/BarcodeReader4.0/Redist, DBR_SHARED_LIBADD) 
        
        PHP_ADD_INCLUDE(/home/xiao/Dynamsoft/BarcodeReader4.0/Include)
        
        PHP_SUBST(DBR_SHARED_LIBADD)
        
        PHP_NEW_EXTENSION(dbr, dbr.c, $ext_shared)
    
    fi
    ```

4. Edit **dbr.c**.
5. Install **phpize**:

    ```
    sudo apt-get install php5-dev
    ```
6. Build the extension **dbr.so**:
    
    ```
    phpize
    ./configure
    make
    ```
7. Run **reader.php**:
    
    ```
    php -c /usr/share/php5/php.ini-production reader.php
    ```
![php barcode reader](http://www.codepool.biz/wp-content/uploads/2016/02/php_extension_result.png)




[1]:http://labs.dynamsoft.com/linux-barcode-reader-overview.htm
[2]:http://php.net/downloads.php


