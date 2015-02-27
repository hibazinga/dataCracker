# CS249 Proj Data Crackers

## README

## How to run the code:

1. Before make, modify the file path in main.c (macros #define)!

2. 	$ make clean
	$ make
	$ ./a.out

3. The program will generate a trained model and write to ./model.csv





## PreProcessing:

./Pre-processing/step1.py
./Pre-processing/step2.py
./Pre-processing/step3.py
./Pre-processing/step4.py
./Pre-processing/step5.py
./Pre-processing/step6.py
./Pre-processing/step7.py

1.	Filter noise records in the training set

Explanation: In the training set, if the records of a user is all positive or all negative, we regard these records as useless. In this case, all records of this user should be deleted from the file.

Input file: 	rec_log_train.txt
Output file: 	rec_log_train_pre_1.txt


2.	Filter inactive users

Explanation: Dummy users are the users who have neither followers nor followees. Find out all dummy users in user_sns.txt and delete their records from the training set.

Input file:		user_sns.txt	rec_log_train_pre_1.txt
Output file:	rec_log_train_pre_2.txt

      
3.	Time-based session slicing

Explanation: Not all the negative ratings imply that the users rejected to follow the recommended items. Instead, users may be attracted to other interesting things on the Weibo website, and during these sessions, the recommended items were omitted by users. Use the session slicing method described in the paper (Shanda, Section 3.1) to remove noise data. 

Detailed explanation:
Session slicing threshold: 45 seconds (45 ~ 1800 seconds are all tested and they roughly make no difference)
For each user, if two consecutive records? time interval is larger than the threshold, it is the cut-off point. (everything before this point are in the same session)

In each session, only records between the {3 records before first occurrence of positive record} and the {2 records after last occurrence of positive record} are considered. This is because user may have ignored all recommendations until he/she starts to accept some, and he may also ignore all recommendations after he has made all the acceptances in the current session.
Moreover, if a session only contains negative records or contains more than 90% negative records, it is discarded.

Input file:		rec_log_train_pre_2.txt
Output file: 	rec_log_train_pre_3.txt


4.	Data assignment to different groups

Explanation: After the data filtering in the former steps, the remaining data in the training set as well as other supplement data files should be divided in 13 group according to users? gender and age, which can be looked up in user_profile.txt .

Input file:		rec_log_train_pre_3.txt		user_sns.txt	user_key_word.txt
Output file:	rec_log_train_1.txt	--		rec_log_train_13.txt
				user_sns_1.txt		--		user_sns_13.txt
				user_key_word_1.txt	--		user_key_word_13.txt

**How to number the files according to different groups?
Respectively, *1.txt ~*13.txt represent: 0-12 male; 0-12 female; 13-25 male; 13-25 female; 26-35 male; 26-35 female; 36-45 male; 36-45 female; 46-60 male; 46-60 female; 60+ male; 60+ female; Missing age and/or gender.
		
		

5.	Dimension Reduction for user-keyword matrix
Explanation: For each user, we only utilize his top 5 keywords. Get the union of the keywords, and construct a user-keyword matrix K in the output file. if user i has keyword j, K(i, j)= weight; otherwise, K(i, j) = 0.

Input file:		user_key_word_1.txt	--	user_key_word_13.txt
Output file:	user_key_word_1.csv	--	user_key_word_13.csv


6.	Construction for user-user matrix
Explanation: construct the user action matrixes S based on the actions (follow, at, comment, retweet) of each user.

S(i, j) = 10*Follow + 2*Nat + 0.2*Ncomment + 1*Nretweet

Input file:		user_user_1.txt	--	user_user_13.txt
Output file:	user_user_1.csv	--	user_user_13.csv




## LFM Model:

./dataCracker/matrix.h -- helper functions for matrix op

./dataCracker/main.c -- LFM model & Gradient descent


