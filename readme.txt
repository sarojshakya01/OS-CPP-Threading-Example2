1. put main.cpp and input30.txt file in a folder

2. goto that folder

3. run following command to compile main.cpp file
g++ -fpermissive main.cpp -lpthread

4. a new file named 'a.out' should be generated after running above command. Now run the output file for nSTAFF value 1,2 and 5.

./a.out 1 < input30.txt > output31.txt
./a.out 2 < input30.txt > output32.txt
./a.out 5 < input30.txt > output35.txt

Now corresponding output file should be generated inside the same folder with given name in above command.