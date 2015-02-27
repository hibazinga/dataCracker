import re
import math

IN_FILE1 = 'track1/user_key_word.txt'
IN_FILE2 = 'track1/user_profile.txt'
IN_FILE3 = 'preprocessing/rec_log_train_pre_3.txt'
TOP_KEYWORD_NUM = 5
CURRENT_YEAR = 2012
CATEGORY_NUM = 13

valid_user_set = set([])	# a set to store all valid user id
keyword_list = list()	#a list to store sorted keywords (after dimension reduction)
user_record = dict()	#a dict to store each user's top 5 keywords with weights
user_profile = dict()	#a dict to store each user's gender and age

def filterValidUsers():
	print "filter all valid users from previous preprocessing files..."
	global valid_user_set
	f = open(IN_FILE3, "r")
	data = f.read().split()
	data = filter(len, data)
	f.close()

	for i in range(0, len(data)):
		if i % 4 == 0:
			valid_user_set.add(data[i])

	# users = [line.split()[0] for line in data]
	# valid_user_set = set(users)
	print "valid user number: " + str(len(valid_user_set))

# get user_profile
# def readUserProfile():
# 	print "Reading user profiles into memory..."
# 	global user_profile
# 	f = open(IN_FILE2, "r")
# 	data = f.read().split('\n')
# 	data = filter(len, data)
# 	f.close()

# 	for line in data:
# 		entries = line.split('\t')
# 		uid = entries[0]
# 		if uid not in valid_user_set:
# 			continue
# 		if entries[1].find('-') != -1:
# 			age = -1
# 		else:
# 			age = CURRENT_YEAR - int(entries[1])
# 		gender =  int(entries[2])
# 		user_profile[uid] = (age, gender)

# 	print user_profile

# get user_record
def readRawFile():
	print "Reading user keyword records into memory..."
	global keyword_list
	global user_record

	f = open(IN_FILE1, "r")
	data = f.read().split('\n')
	data = filter(len, data)
	f.close()

	keyword_set = set([])	#a set to store all distinct keywords

	for line in data:
		uid = line.split()[0]
		if uid not in valid_user_set:
			continue
		# print uid
		keyword_weights = re.split(';',line.split()[1])
		keywords = [int(i.split(":")[0]) for i in keyword_weights]
		weights = [float(i.split(":")[1]) for i in keyword_weights]
		keyword_weights = zip(keywords, weights)
		if len(keyword_weights) <= TOP_KEYWORD_NUM:
			user_record[uid] = dict((k, v) for k, v in keyword_weights)
		else:
			keyword_weights = sorted(keyword_weights, key=lambda entry: entry[1], reverse=True)
			keyword_weights = keyword_weights[0: TOP_KEYWORD_NUM-1]
			user_record[uid] = dict((k, v) for k, v in keyword_weights)

		keyword_set.update([entry[0] for entry in keyword_weights])
		
		# Normalization
		mode = math.sqrt(sum([w*w for w in user_record[uid].values()]))
		for kw in user_record[uid]:
			user_record[uid][kw]/= mode
		# print user_record[uid]

	keyword_list = sorted(list(keyword_set))

	print "total keyword number: " + str(len(keyword_list))

# select from category 1 ~ category 13 for uid
def selectCategory(uid):
	age, gender = user_profile[uid]
	if age == -1 or gender == 0:
		return 13

	index = 0
	if age <= 12:
		index = 1
	elif age <= 25:
		index = 3
	elif age <= 35:
		index = 5
	elif age <= 45:
		index = 7
	elif age <= 60:
		index = 9
	else:
		index = 11

	if gender == 2:
		index += 1

	return index

# def writeMatrixToFiles():
# 	print "constructing user-keyword matrixes..."
# 	files = [None]*13
# 	for i in range(0, 13):
# 		f = open("preprocessing/user_key_word_%s.csv" % (i+1), "a")
# 		files[i] = f

# 	count = 0
# 	for uid in user_record:
# 		if count % 10000 == 0:
# 			print "record" + str(count) + "..."
# 		count+=1
# 		row = ""

# 		for kw in keyword_list:
# 			if kw in user_record[uid]:
# 				row += str(user_record[uid][kw]) + ","
# 			else:
# 				row += "0,"
# 		row = row[0: len(row)-1]
# 		row += "\n"
# 		files[selectCategory(uid)-1].write(row);

# 	for i in range(0, 13):
# 		files[i].close()

def writeToFiles():
	keyword_index_dict = dict()
	for i in range(0, len(keyword_list)):
		keyword_index_dict[keyword_list[i]] = i

	for i in range(0, CATEGORY_NUM):
		f_in = open("preprocessing/user_key_word_%s.txt" % (i+1), "r")
		f_out_1 = open("preprocessing/user_key_word_%s_ij.txt" % (i+1), "a")
		f_out_2 = open("preprocessing/user_key_word_%s_ji.txt" % (i+1), "a")

		data = f_in.read().split('\n')
		data = filter(len, data)
		users = [line.split()[0] for line in data]
		# users.sort()

		user_keyword_weight_list = []

		for j in range(0, len(users)):
			uid = users[j]
			if uid not in valid_user_set:
				continue
			for keyword in user_record[uid]:
				user_index = j
				keyword_index = keyword_index_dict[keyword]
				weight = user_record[uid][keyword]
				entry = (user_index, keyword_index, weight)
				user_keyword_weight_list.append(entry)

		user_keyword_weight_list = sorted(user_keyword_weight_list, key=lambda x: (x[0], x[1]))
		row = len(users)
		col = len(keyword_list)
		f_out_1.write("%s\t%s\n" % (row, col))
		for entry in user_keyword_weight_list:
			i, j, w = entry
			outline = "%s\t%s\t%s\n" % (i, j, w)
			f_out_1.write(outline)


		user_keyword_weight_list = sorted(user_keyword_weight_list, key=lambda x: (x[1], x[0]))
		row = len(keyword_list)
		col = len(users)
		f_out_2.write("%s\t%s\n" % (row, col))
		for entry in user_keyword_weight_list:
			i, j, w = entry
			outline = "%s\t%s\t%s\n" % (j, i, w)
			f_out_2.write(outline)	


		f_in.close()
		f_out_1.close()
		f_out_2.close()

if __name__ == "__main__":
	filterValidUsers()
	# readUserProfile()
	readRawFile()
	writeToFiles()

