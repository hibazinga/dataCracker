path_in1 = '../predata/rec_log_train_pre_1.txt'
path_in2 = '../track1/user_sns.txt'
path_out = '../predata/rec_log_train_pre_2.txt'
f_in1 = open(path_in1, "r")
f_in2 = open(path_in2, "r")
userID = set([])

for line in f_in2:
	l = line.split()
	userID.add(l[0])
	userID.add(l[1])

f_out = open(path_out, "w")
for line in f_in1:
	l = line.split()
	if l[0] in userID:
		f_out.write(line)

f_out.close()
f_in2.close()
f_in1.close()


