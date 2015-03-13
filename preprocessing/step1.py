path_in = '../track1/rec_log_train.txt'
path_out = '../predata/rec_log_train_pre_1.txt'
f_in = open(path_in, "r")

dict = {}

print("step1")
count = 0
for line in f_in:
	count = count + 1
	if count % 1000000 == 0:
		print(count)
	l = line.split()
	if l[0] in dict:
		if int(l[2]) != dict[l[0]] and dict[l[0]] != 0:
			dict[l[0]] = 0
	else:
		dict[l[0]] = int(l[2])

f_in.seek(0)
f_out = open(path_out, "w")
for line in f_in:
	l = line.split()
	if dict[l[0]] == 0:
		f_out.write(line)

f_out.close()
f_in.close()
