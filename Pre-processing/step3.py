import time

in_file = '../predata/rec_log_train_pre_2.txt'
out_file = '../predata/rec_log_train_pre_3.txt'
records = set()
threshold = 45
separator = "\t"
mac_or_windows = 0  # mac: 0; windows: 1 (mac and windows have different \n)

def preprocess():

	start = time.time()

	f = open(in_file, 'r')

	# count = 0
	pos = 0

	for line in f:
		# count += 1
		# if (count > 100000):
		# 	break

		if (line not in records):
			filter_records(pos, threshold)
		else:
			records.remove(line)

		pos += len(line) + mac_or_windows

	f.close()

	end = time.time()
	print "%f seconds" % (end - start)



def compute_threshold(user, pos):

	f = open(in_file, 'r')
	f.seek(pos)

	count = 0
	mySum = 0
	prevTime = 0
	for line in f:
		if (count > 4000):
			print line
			break
		recomm = line.strip().split("\t")
		currUser = recomm[0]
		if (user == currUser):
			currTime = int(recomm[3])
			interval = currTime - prevTime
			if (prevTime != 0 and interval > 0 and interval < 3600):
				count += 1
				mySum += interval
			prevTime = currTime

	average = mySum / count
	threshold = (90 + average) / 2
	print "threshold = "
	print threshold

	f.close()

	return threshold



def filter_records(pos, threshold):

	fr = open(in_file, 'r')
	fr.seek(pos)

	fw = open(out_file, 'a')

	session_buffer = []
	session_count = 0
	positive_buffer = []
	prevTime = 0
	for line in fr:
		if (line in records):
			continue
		recomm = line.strip().split("\t")
		if (len(recomm) != 4):		# if record length is wrong, skip it
			print len(recomm)
			continue
		currUser = recomm[0]
		currAR = recomm[2]
		currTime = int(recomm[3])
		interval = currTime - prevTime
		if (prevTime == 0):
			user = currUser
			prevTime = currTime
			session_buffer.append(recomm)
			if (currAR == "1"):
				positive_buffer.append(session_count)	
			session_count += 1						
		elif (interval > threshold):	# if current time - previous time > threshold, no need to check the rest of file
			break
		elif (user == currUser):
			prevTime = currTime			
			session_buffer.append(recomm)	# if in the current session, add to session_buffer
			if (currAR == "1"):
				positive_buffer.append(session_count)    # if positive, add to positive buffer
			session_count += 1				
			records.add(line)

	if (positive_buffer == []):		# meaning that posLen / sesLen > 0
		return
	posLen = len(positive_buffer)
	sesLen = len(session_buffer)
	ratio = 0.1
	condition3 = posLen >= sesLen * ratio	# check condition 3: if not satisfied, session is discarded
	if (not condition3):
		return

	# for record in session_buffer:
	for index, record in enumerate(session_buffer):

		condition1 = index >= positive_buffer[0] - 3
		condition2 = index <= positive_buffer[posLen - 1] + 2

		if (condition1 and condition2):
			res = separator.join(record)
			res += "\n"
			fw.write(res)

	fr.close()
	fw.close()



preprocess()
