### How it patches

it just replaces the ```mov ax, cx``` line with ```xor ax, ax```

this leads to collision in hash count so the hash for your line and password will be always equal zero.

the program will compare them and give you ```whole lotta red rights``` bcs they are equal.


### How this binpatcher works
![image](https://github.com/user-attachments/assets/5fdf203a-e86f-495e-bc41-e6a8f49ca365)
```
Gif image with a guy throwing money running and some playboi carti 8bit type beat playing
```
- just press ```patch it!```

- you will see a dilogue window opened (i used tinyfiledialogs library to open it)

![image](https://github.com/user-attachments/assets/5142d4e4-e765-4c6f-8d71-75c95df57e69)

- here you **choose** a file you need to patch.
----
if you enter the right version of the program it will write
```
choosing the bytes for patching...
File Successfully Patched!
```

if the was already patched it won' patch it.
```
File has been already patched!
```

if there are some errors occured it will just close your file and write
```
Unknown error!
```
---
This is all you need to know about this simple patcher!
