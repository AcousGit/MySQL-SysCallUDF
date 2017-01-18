# MySQL-SysCallUDF

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! WARNING !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!	THIS TOOL WAS CREATED FOR PENTEST NOT FOR SYS ADMIN. IT CAN BE VERY DANGEROUS TO USE IT !!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!                           					    FOR OTHER PURPOSE					                               !!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

Here we wanted to create a User defined Function which allows users to execute system calls directly from MySQL clients.
!!! Warning it works only on LINUX based systems !!!

To use it, you have to install libmysqlclient-dev or you will not be able to use MySQL libraries.

Then you can compile it this way:
$> gcc -shared -o libmysyschar.so mysyschar.cc -fPIC

It is important to start the ".so" file’s name with "lib" because some versions of MySQL need it.
Then you must put the ".so" file in /usr/lib/mysql/plugin/ folder.

Finally, open mysql client, and enter: 
mysql> CREATE FUNCTION mysyschar RETURNS STRING SONAME libmysyschar.so;

To use it:
select mysyschar("cmdline"); 

!!! WARNING: If you haven't got the proper rights it can create connection (with mysql server) problems !!! 

And to remove it:
mysql> DROP FUNCTION mysyschar

Now if you want to use it for pentest read my article on SQLInjection (It Will Come Soon ^^). 
 
