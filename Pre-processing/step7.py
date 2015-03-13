# python step7.py number

path_in1 = "../track1/item.txt"

itemId_dict = {}
itemId_list = []
fp = open(path_in1, "r")
i = 0
for line in fp:
    l = line.split()
    itemId_list.append(l[0])
    itemId_dict[l[0]] = i
    i = i + 1
fp.close()

for g in range(1,12):
    path_in2 = "../predata/user_profile_%s.txt"%g
    path_in3 = "../predata/rec_log_train_%s.txt"%g
    path_in4 = "../predata/Rui_%s_nm.txt"%g

    print "generating Rui for group"+str(g)
    #n_u*m
    fp = open(path_in2, "r")
    userId_dict = {}
    userId_list = fp.readline().split()
    i = 0
    for id in userId_list:
        userId_dict[id] = i
        i = i + 1
    fp.close()

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
    fp = open(path_in4, "w")
    n = len(userId_list)
    m = len(itemId_list)
    fp.write(str(n)+"\t"+str(m)+"\n")
    for entry in record:
        i, j, w = entry
        line = str(i)+"\t"+str(j)+"\t"+str(w)+"\n"
        fp.write(line)
    fp.close()
