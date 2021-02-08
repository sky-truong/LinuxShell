Hello, welcome to the Linux Shell's ReadMe.

To my understanding, I've implemented everything that's listed 
in the assignment description, but the display looks slightly different.
I apologize if something is amiss and I've missed that spec.

To run program: make 
To clean up files: make clean

The following commands and examples are from running my shell program,
following "Expected Program Behaviours" posted on course page:

************** EXAMPLES ***************
/home/undergrad/2/truongy/CIS3110/A1> ls
array.c  array.o  bgSleep.c     inputFile.txt  interactive.c  myShell    myShell.o
array.h  bgSleep  fileList.txt  interactive    Makefile       myShell.c  ReadMe.txt
/home/undergrad/2/truongy/CIS3110/A1> ls -l
total 103
-rw-r--r-- 1 truongy undergrad  3385 Feb  8 00:24 array.c
-rw-r--r-- 1 truongy undergrad   791 Feb  8 00:24 array.h
-rw-r--r-- 1 truongy undergrad 11896 Feb  8 01:01 array.o
-rwxr-xr-x 1 truongy undergrad 16712 Feb  6 18:32 bgSleep
-rw-r--r-- 1 truongy undergrad   684 Feb  2 18:06 bgSleep.c
-rw-r--r-- 1 truongy undergrad   614 Feb  7 15:51 fileList.txt
-rw-r--r-- 1 truongy undergrad    58 Feb  7 15:52 inputFile.txt
-rwxr-xr-x 1 truongy undergrad 16960 Feb  7 15:50 interactive
-rw-r--r-- 1 truongy undergrad  1007 Feb  2 18:06 interactive.c
-rw-r--r-- 1 truongy undergrad   222 Feb  7 22:19 Makefile
-rwxr-xr-x 1 truongy undergrad 39584 Feb  8 01:47 myShell
-rw-r--r-- 1 truongy undergrad 19345 Feb  8 01:39 myShell.c
-rw-r--r-- 1 truongy undergrad 34520 Feb  8 01:47 myShell.o
-rw-r--r-- 1 truongy undergrad     0 Jan 27 09:40 ReadMe.txt
/home/undergrad/2/truongy/CIS3110/A1> ks
ks: command not found
/home/undergrad/2/truongy/CIS3110/A1> ./execNotThere
./execNotThere: No such file or directory
/home/undergrad/2/truongy/CIS3110/A1> ls -0
ls: invalid option -- '0'
Try 'ls --help' for more information.
/home/undergrad/2/truongy/CIS3110/A1> ./bgSleep 2
/home/undergrad/2/truongy/CIS3110/A1> ./bgSleep 5 &
/home/undergrad/2/truongy/CIS3110/A1> [1] 28580
ps
  PID TTY          TIME CMD
 3761 pts/34   00:00:00 bash
 7461 pts/34   00:00:00 myShell
28580 pts/34   00:00:00 bgSleep
28664 pts/34   00:00:00 ps
/home/undergrad/2/truongy/CIS3110/A1> ps
  PID TTY          TIME CMD
 3761 pts/34   00:00:00 bash
 7461 pts/34   00:00:00 myShell
28580 pts/34   00:00:00 bgSleep <defunct>
31195 pts/34   00:00:00 ps
[1] + 28580 Done         ./bgSleep 5 &
/home/undergrad/2/truongy/CIS3110/A1> ./bgSleep 30 &
/home/undergrad/2/truongy/CIS3110/A1> [2] 12679
./bgSleep 20 &
/home/undergrad/2/truongy/CIS3110/A1> [3] 13293
./bgSleep 10 &
/home/undergrad/2/truongy/CIS3110/A1> [4] 13947
ps
  PID TTY          TIME CMD
 3761 pts/34   00:00:00 bash
 7461 pts/34   00:00:00 myShell
12679 pts/34   00:00:00 bgSleep
13293 pts/34   00:00:00 bgSleep
13947 pts/34   00:00:00 bgSleep
14136 pts/34   00:00:00 ps
/home/undergrad/2/truongy/CIS3110/A1> ps
  PID TTY          TIME CMD
 3761 pts/34   00:00:00 bash
 7461 pts/34   00:00:00 myShell
12679 pts/34   00:00:00 bgSleep
13293 pts/34   00:00:00 bgSleep
13947 pts/34   00:00:00 bgSleep
14758 pts/34   00:00:00 ps
/home/undergrad/2/truongy/CIS3110/A1> ps
  PID TTY          TIME CMD
 3761 pts/34   00:00:00 bash
 7461 pts/34   00:00:00 myShell
12679 pts/34   00:00:00 bgSleep <defunct>
13293 pts/34   00:00:00 bgSleep <defunct>
13947 pts/34   00:00:00 bgSleep <defunct>
15801 pts/34   00:00:00 ps
[2] + 12679 Done         ./bgSleep 30 &
[3] + 13293 Done         ./bgSleep 20 &
[4] + 13947 Done         ./bgSleep 10 &
/home/undergrad/2/truongy/CIS3110/A1> ps
  PID TTY          TIME CMD
 3761 pts/34   00:00:00 bash
 7461 pts/34   00:00:00 myShell
18574 pts/34   00:00:00 ps
/home/undergrad/2/truongy/CIS3110/A1> ls -l > fileList.txt
/home/undergrad/2/truongy/CIS3110/A1> more fileList.txt
total 107
-rw-r--r-- 1 truongy undergrad  3385 Feb  8 00:24 array.c
-rw-r--r-- 1 truongy undergrad   791 Feb  8 00:24 array.h
-rw-r--r-- 1 truongy undergrad 11896 Feb  8 01:01 array.o
-rwxr-xr-x 1 truongy undergrad 16712 Feb  6 18:32 bgSleep
-rw-r--r-- 1 truongy undergrad   684 Feb  2 18:06 bgSleep.c
-rw-r--r-- 1 truongy undergrad     0 Feb  8 01:56 fileList.txt
-rw-r--r-- 1 truongy undergrad    58 Feb  7 15:52 inputFile.txt
-rwxr-xr-x 1 truongy undergrad 16960 Feb  7 15:50 interactive
-rw-r--r-- 1 truongy undergrad  1007 Feb  2 18:06 interactive.c
-rw-r--r-- 1 truongy undergrad   222 Feb  7 22:19 Makefile
-rwxr-xr-x 1 truongy undergrad 39584 Feb  8 01:47 myShell
-rw-r--r-- 1 truongy undergrad 19345 Feb  8 01:39 myShell.c
-rw-r--r-- 1 truongy undergrad 34520 Feb  8 01:47 myShell.o
-rw-r--r-- 1 truongy undergrad  2329 Feb  8 01:54 ReadMe.txt
/home/undergrad/2/truongy/CIS3110/A1> ls -l | wc
     15     128     853
/home/undergrad/2/truongy/CIS3110/A1> more inputFile.txt
How are you today?
What is the weather like where you are?
/home/undergrad/2/truongy/CIS3110/A1> ./interactive 2
What is your question? > my Question 1
Question 1: my Question 1
What is your question? > my Q 2
Question 2: my Q 2
Bye!
/home/undergrad/2/truongy/CIS3110/A1> ./interactive 2 < inputFile.txt > outputFile
/home/undergrad/2/truongy/CIS3110/A1> more outputFile
What is your question? > Question 1: How are you today?
What is your question? > Question 2: What is the weather like where you are?
Bye!
/home/undergrad/2/truongy/CIS3110/A1> more outputFile | sort
Bye!
What is your question? > Question 1: How are you today?
What is your question? > Question 2: What is the weather like where you are?
/home/undergrad/2/truongy/CIS3110/A1> ./interactive 2 < inputFile.txt | sort
Bye!
What is your question? > Question 1: How are you today?
What is your question? > Question 2: What is the weather like where you are?
/home/undergrad/2/truongy/CIS3110/A1> ./interactive 2 < inputFile.txt | sort > saveFile
/home/undergrad/2/truongy/CIS3110/A1> more saveFile
Bye!
What is your question? > Question 1: How are you today?
What is your question? > Question 2: What is the weather like where you are?
/home/undergrad/2/truongy/CIS3110/A1> more outputFile
What is your question? > Question 1: How are you today?
What is your question? > Question 2: What is the weather like where you are?
Bye!
/home/undergrad/2/truongy/CIS3110/A1> echo $PATH
/usr/bin:/bin:/home/undergrad/2/truongy/CIS3110/A1
/home/undergrad/2/truongy/CIS3110/A1> echo $HOME
/home/undergrad/2/truongy/CIS3110/A1
/home/undergrad/2/truongy/CIS3110/A1> history
1 q
2 ls &
3 ls
4 ls &
5 ls &
6 ps
7 ./bgSleep 5 &
8 ./bgSleep 4 &
9 ps
10 ps
11 ps
12 q
13 ps &
14 ls &
15 ps
16 history
17 q
18 ls &
19 ps
20 ps &
21 ps
22 ./bgSleep 2 &
23 ./bgSleep 5 &
24 q
25 ./bgSleep 10 &
26 q
27 echo $PATH
28 echo $HOME
29 ./interactive 2
30 q
31 ks
32 ./execNotThere
33 q
34 ks
35 ./execNot
36 ls -0
37 more inputFile.txt | sort
38 q
39 cd ..
40 q
41 ls
42 ls -l
43 ks
44 ./execNotThere
45 ls -0
46 ./bgSleep 2
47 ./bgSleep 5 &
48 ps
49 ps
50 ./bgSleep 30 &
51 ./bgSleep 20 &
52 ./bgSleep 10 &
53 ps
54 ps
55 ps
56 ps
57 ls -l fileList.txt
58 ls -l > fileList.txt
59 more fileList.txt
60 ls -l | wc
61 more inputFile.txt
62 ./interactive 2
63 ./interactive 2 < inputFile.txt > outputFile
64 more outputFile
65 more outFile | sort
66 more outputFile | sort
67 ./interactive 2 < inputFile.txt | sort
68 ./interactive 2 < inputFile.txt | sort > saveFile
69 more saveFile
70 more outputFile
71 echo $PATH
72 echo $HOME
73 history
/home/undergrad/2/truongy/CIS3110/A1> history 3
72 echo $HOME
73 history
74 history 3
/home/undergrad/2/truongy/CIS3110/A1> history -c
History file deleted.
/home/undergrad/2/truongy/CIS3110/A1> history
1 history
/home/undergrad/2/truongy/CIS3110/A1> pwd
/home/undergrad/2/truongy/CIS3110/A1
/home/undergrad/2/truongy/CIS3110/A1> cd ..
/home/undergrad/2/truongy/CIS3110> cd A1
/home/undergrad/2/truongy/CIS3110/A1> cd ..
/home/undergrad/2/truongy/CIS3110> cd ..
/home/undergrad/2/truongy> cd ~
/home/undergrad/2/truongy/CIS3110/A1>

************** FINISH ***************

Notes:
- Most commands require spacing between the words, except for "export",
which has the following format (taken from assignment's description):
export PATH=/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin:$HOME/bin
export PATH=/usr/local/sbin:$PATH

