# codingchallange

Uma shankar- ushankar@usc.edu
Checklist:
a. How i have tested my solution?
Ans- I looked for conditions where either external ip and port can be both *(wildcard entry) ,In that case I am printing the warning message and not adding that nat entry.I have checked all the possible scenarios of wildcard entries for port and ip and whether it is able to map it correctly and display .However, assumption is there that ip (ipv4) is provided correctly.

b.any interesting coding, design, or algorithmic choices you’d like to point out
Ans- As we have both completely valid ip,nat entries and wildcard ip and entry .I have created a tree datastructure to hold the nat map of the valid ip and port entries which will save serach time and for wild card ip and port number i have created a linked list as there is not criterion of arrangement.Also, first i am searching in the tree so i believe it will first look for all the valid arrangement and then go for special ip and port arrangement.

c.any refinements or optimizations that you would’ve implemented if you had more time
Ans- I have done coding in C so i would have tried hash maps given more time.

d.how you modified or made the challenge more well defined if you did
Ans- No, I have not modified the challange. I have just added one warning message if the both external ip and port numbers are wild card entry.

e.anything else you’d like the reviewer to know
Ans- To build the code : Please enter make command in terminal, please enter make clean to delete the exe and .o files.
	To run the code : Please enter sh test.sh where argument1 is NAT file, argument2 is FLOW file and argument3 is OP File.I have already hardcoded these filenames in script file.To change the names of the file,please update the test.sh or discard the script.
	 
