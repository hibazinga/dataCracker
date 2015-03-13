import re
import math

IN_FILE1 = '../track1/user_key_word.txt'
IN_FILE2 = '../track1/user_profile.txt'
IN_FILE3 = '../predata/rec_log_train_pre_3.txt'
TOP_KEYWORD_NUM = 5
CATEGORY_NUM = 11

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

	print "valid user number: " + str(len(valid_user_set))

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

def writeToFiles():
	keyword_index_dict = dict()
	for i in range(0, len(keyword_list)):
		keyword_index_dict[keyword_list[i]] = i

	for i in range(0, CATEGORY_NUM):
		f_in = open("../predata/user_key_word_%s.txt" % (i+1), "r")
		f_out_1 = open("../predata/user_key_word_%s_ij.txt" % (i+1), "w")
		f_out_2 = open("../predata/user_key_word_%s_ji.txt" % (i+1), "w")

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
	readRawFile()
	writeToFiles()

