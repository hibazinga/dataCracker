import sys
# python step7.py number
r = sys.argv[1]

path_in1 = "preprocessing/user_profile_%s.txt"%r
path_in2 = "../track1/item.txt"
path_in3 = "preprocessing/rec_log_train_%s.txt"%r
path_in4 = "preprocessing/Rui_%s_nm.txt"%r

#n_u*m
fp = open(path_in1, "r")
userId_dict = {}
userId_list = fp.readline().split()
i = 0
for id in userId_list:
    userId_dict[id] = i
    i = i + 1
fp.close()

itemId_dict = {}
itemId_list = []
fp = open(path_in2, "r")
i = 0
for line in fp:
    l = line.split()
    itemId_list.append(l[0])
    itemId_dict[l[0]] = i
    i = i + 1
fp.close()

print len(itemId_list)
print len(userId_list)

fp = open(path_in3, "r")
record = set([])
record_dict = {}
for line in fp:
    l = line.split()
    index = l[0]+"-"+l[1]
    if index in record_dict:
        t, result = record_dict[index]
        if t < l[3]:
            record_dict[index] = (l[3], int(l[2]))
    else:
        record_dict[index] = (l[3], int(l[2]))

fp.close()

for rec in record_dict:
    r = rec.split('-')
    t, res = record_dict[rec]
    entry = (userId_dict[r[0]],itemId_dict[r[1]], res)
    record.add(entry)

record = sorted(record, key=lambda x: (x[0], x[1]))
print "write"
# write ever row into the matrix
# because Rui does not need the tranverse one, only one file is enough
fp = open(path_in4, "w")
fp.write(str(len(userId_list))+"\t"+str(len(itemId_list))+"\n")
for entry in record:
    i, j, w = entry
    line = str(i)+"\t"+str(j)+"\t"+str(w)+"\n"
    fp.write(line)
fp.close()
