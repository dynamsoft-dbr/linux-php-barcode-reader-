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
    sudo make install
    ```
7. Edit **php.ini-production**:

    ```
    extension=/usr/lib/php5/20121212/dbr.so
    ```
8. Run **reader.php**:
    
    ```
    php -c /usr/share/php5/php.ini-production reader.php
    ```
![php barcode reader](http://www.codepool.biz/wp-content/uploads/2016/02/php_extension_result.png)

## Online Barcode Reader with PHP Custom Extension
Get the [source code][3] and deploy it to any web server.
![php online barcode reader](http://www.codepool.biz/wp-content/uploads/2016/02/php_online_barcode_reader.png)

### PHP on Apache
1. Install **php5-curl**, **apache2** and **libapache2-mod-php5**:

    ```
    sudo apt-get install php5-curl apache2 libapache2-mod-php5
    ```
2. Add extension path to php.ini:

    ```
    extension=/usr/lib/php5/20121212/dbr.so
    ```
3. Get write permissions:

    ```
    sudo chgrp -R www-data /var/www/html/reader
    sudo chmod -R g+rw /var/www/html/reader
    ```
4. Start **Apache**:
    
    ```
    sudo service apache2 start
    #sudo service apache2 stop // if you want to stop Apache
    ```
5. Visit **http://localhost/reader.php**.

### PHP on Nginx
1. Install **Nginx** and **php5-cgi**:

    ```
    sudo apt-get install nginx php5-cgi
    ```
2. Enable PHP in configuration file:
    
    ```
    sudo vi /etc/nginx/sites-available/default
    ```
    ![nginx enable php](http://www.codepool.biz/wp-content/uploads/2016/02/nginx_enable_php.png)
3. Get write permissions:
    
    ```
    sudo chgrp -R www-data /usr/share/nginx/html/reader
    sudo chmod -R g+rw /usr/share/nginx/html/reader
    ```
4. Run **Nginx** and **php-cgi**:
    
    ```
    sudo nginx
    # sudo nginx –s stop // if you want to stop Nginx
    sudo php-cgi -b 127.0.0.1:9000 -c /usr/share/php5/php.ini-production
    ```
5. Visit **http://localhost/reader.php**.

## Blog
* [How to Make PHP Barcode Reader on Linux][4]
* [How to Deploy PHP Code to Apache and Nginx on Ubuntu 14.04][5]

[1]:http://labs.dynamsoft.com/linux-barcode-reader-overview.htm
[2]:http://php.net/downloads.php
[3]:https://github.com/dynamsoftlabs/linux-php-barcode-reader-/tree/master/reader
[4]:http://www.codepool.biz/linux-php-barcode-reader.html
[5]:http://www.codepool.biz/deploy-php-nginx-apache-ubuntu.html

