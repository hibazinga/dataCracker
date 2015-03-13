active_user_data = "../predata/rec_log_train_pre_3.txt"
user_sns = "../track1/user_sns.txt"
user_action = "../track1/user_action.txt"
inactive_relation_active = "inactive_relation_active.txt"

# read all the active users
file_active_user_data = open(active_user_data, "r")
active_userID = set([])
for line in file_active_user_data:
    l = line.split()
    i = 0
    while i<len(l):
        active_userID.add(l[i])
        i = i + 4
file_active_user_data.close()

print "read active user\n"

inactive_relation_active_dict = dict()
# get the inactive user SNS
file_user_sns = open(user_sns, "r")
for line in file_user_sns:
    l = line.split()
    if l[1] not in active_userID:
        continue
    if l[0] not in active_userID:
        if l[0] not in inactive_relation_active_dict:
            inactive_relation_active_dict[l[0]] = dict()
        inactive_relation_active_dict[l[0]][l[1]] = 5
file_user_sns.close()

print "get inactive user\n"

# get the inactive user action
file_user_action = open(user_action, "r")
for line in file_user_action:
    l = line.split()
    if l[1] not in active_userID:
        continue
    if l[0] not in active_userID:
        score = 2*float(l[2])+0.2*float(l[3])+float(l[4])
        if l[0] not in inactive_relation_active_dict:
            inactive_relation_active_dict[l[0]] = dict()
        if l[1] not in inactive_relation_active_dict[l[0]]:
            inactive_relation_active_dict[l[0]][l[1]] = score
        else:
            inactive_relation_active_dict[l[0]][l[1]] += score
file_user_action.close()

print "write to file\n"

file_inactive_relation_active = open(inactive_relation_active, "w")
for inactive_user in inactive_relation_active_dict:
    file_inactive_relation_active.write(inactive_user + "\t")
    for active_user in inactive_relation_active_dict[inactive_user]:
        file_inactive_relation_active.write(active_user + "," + \
        str(inactive_relation_active_dict[inactive_user][active_user]) + ";")
    file_inactive_relation_active.write("\n")
file_inactive_relation_active.close()
