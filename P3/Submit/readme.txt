Readme

Subdirectories do not work. Expect "/" for root directory check.

./diskinfo

For this part, after mapping the file, all the different function calls take you to a specific address based on the superblock table for the superblock information. Addresses are calculted
based on how many bytes each field takes up and so that much is added to the "address" variable. Then based on the size of the field, that many bytes are copied using memcpy and the value is read and converted to an int
which is returned and outputed. For the FAT information, we find where the FAT block starts and move in 4 byte increments to check if it's free, reserved or allocated based on its value.

./disklist

Functions from the previous part are utilized here. The program really just checks for the argument after specifying the file to open to make sure it is only "/" since the program is not designed to work for subdirectories. So 
it checks to make sure the root directory is specified. From the previous program, we can quickly find our way to the block with the information for each file. We then go to the part with the modified time information. 2 bytes are for 
the year and each other field is 1 byte. So the right amount of bytes are grabbed accordingly and are passed to new functions that convert to integers and are then displayed accordingly for the date and time.

./diskget

In this program, we find the starting point and the number of blocks for the file to be copied. Knowing that, we move to the first block and use fwrite to copy the whole block into a file in the local directory. Variables
are used to check how many blocks remaining and also where in the FAT block to find the next block of the file is. We move forward in the "for" loop, which ends after hitting the amount of blocks of the file, and 
get the value of where the next block of the file is located and proceed to write that data to the file.