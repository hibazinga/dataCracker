import math
import sys

r = sys.argv[1]
path_in1 = 'rec_log_train_pre_3.txt'
path_in2 = '../track1/user_profile.txt'
path_in3 = "preprocessing/user_profile_%s.txt"%r
path_in4 = "preprocessing/user_sns_%s.txt"%r
path_in5 = "preprocessing/user_action_%s.txt"%r
path_in6 = "preprocessing/S_%s_nu_n.txt"%r
path_in7 = "preprocessing/S_%s_n_nu.txt"%r


fp = open(path_in1, "r")
userID_set = set([])
for line in fp:
    l = line.split()
    i = 0
    while i<len(l):
        userID_set.add(l[i])
        i = i+4
fp.close()
userId = []
userId_dict = {}
fp = open(path_in2, "r")
i = 0
for line in fp:
    l = line.split()
    if l[0] in userID_set:
        userId.append(l[0])
        userId_dict[l[0]] = i
        i = i + 1
fp.close()
#column of matrix
n = len(userId)
print "total user: "+str(n)

fp = open(path_in3, "r")
group_userId = fp.readline().split()
fp.close()
group_userId_dict = {}
i = 0
for id in group_userId:
    group_userId_dict[id] = i
    i = i + 1

m = len(group_userId)
print "group user: "+ str(m)

combine_dict = {}
fp = open(path_in4, "r")
for line in fp:
    l = line.split()
    if l[0] in combine_dict:
        combine_dict[l[0]][l[1]]=5
    else:
        combine_dict[l[0]] = {l[1]:5}
fp.close()

fp = open(path_in5, "r")
for line in fp:
    l = line.split()
    index = l[0]+"-"+l[1]
    score = 2*float(l[2])+0.2*float(l[3])+float(l[4])
    if l[0] in combine_dict:
        if l[1] in combine_dict[l[0]]:
            combine_dict[l[0]][l[1]] += score
        else:
            combine_dict[l[0]][l[1]] = score
    else:
        combine_dict[l[0]] = {l[1]:score}
fp.close()

user_user_list = []
for record in combine_dict:
    mode = math.sqrt(sum([t*t for t in combine_dict[record].values()]))
    for u in combine_dict[record]:
        if mode != 0:
            combine_dict[record][u]/=mode
        entry = (group_userId_dict[record], userId_dict[u],combine_dict[record][u]) 
        user_user_list.append(entry)

user_user_list = sorted(user_user_list, key=lambda x: (x[0], x[1]))

print "write1"
fp = open(path_in6, "w")
fp.write(str(m)+"\t"+str(n)+"\n")               
for entry in user_user_list:
    i, j, w = entry
    line = str(i)+"\t"+str(j)+"\t"+str(w)+"\n"
    fp.write(line)
fp.close()
print "write2"
user_user_list = sorted(user_user_list, key=lambda x: (x[1], x[0]))
fp = open(path_in7, "w")
fp.write(str(n)+"\t"+str(m)+"\n")
for entry in user_user_list:
    i, j, w = entry
    line = str(j)+"\t"+str(i)+"\t"+str(w)+"\n"
    fp.write(line)
fp.close()
