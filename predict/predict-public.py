test_file = "../track1/rec_log_test.txt"
user_profile = "../track1/user_profile.txt"
user_profile_group_param = "../predata/user_profile_%s.txt"
inactive_relation_active = "inactive_relation_active.txt"
R_matrix_param = "../training/model%s.csv" # TODO: change the matrix name according to the real name
item_ID = "../track1/item.txt"
user_item_recommendation = "user_item_recommendation_public.txt"

print "get the item IDs and their relative numbers"
# get the item IDs and their relative numbers
itemID_number = dict()
number_itemID = dict()
file_item = open(item_ID, "r")
i = 0
for line in file_item:
    strs = line.split()
    itemID_number[strs[0]] = i
    number_itemID[i] = strs[0]
    i = i + 1
file_item.close()

print "get all the active userIDs and their relative groupIDs"
# get all the active userIDs and their relative groupIDs#
active_user_group = dict()
group_active_user = dict()
for i in range(1, 12):
    active_user_group[i] = dict()
    group_active_user[i] = dict()
    user_profile_group = user_profile_group_param % (str(i))
    print user_profile_group
    file_user_profile_group = open(user_profile_group, "r")
    group_userId = file_user_profile_group.readline().split()
    file_user_profile_group.close()
    j = 0
    for ID in group_userId:
        group_active_user[i][j] = ID
        active_user_group[i][ID] = dict()
        active_user_group[i][ID]["id"] = j
        active_user_group[i][ID]["ownitems"] = dict()
        active_user_group[i][ID]["followeritems"] = dict()
        j = j + 1

print "read the test file"
# read the test file #####################################
inactive_user = dict()
file_test = open(test_file, "r")
i = 0
for line in file_test:
    strs = line.split()
    if int(strs[3]) >= 1321891200:
	    continue
    for j in range(1, 12):
        if strs[0] in active_user_group[j]:
            active_user_group[j][strs[0]]["ownitems"][itemID_number[strs[1]]] = 0
            break
    if j == 12:
        if strs[0] not in inactive_user:
            inactive_user[strs[0]] = dict()
            inactive_user[strs[0]]["items"] = dict()
            inactive_user[strs[0]]["followees"] = dict() 
        inactive_user[strs[0]]["items"][itemID_number[strs[1]]] = 0
    i = i + 1
    if i % 1000000 == 0:
        print i
file_test.close()

print "get all the inactive userIDs and their relative followees"
# get all the inactive userIDs and their relative followees
# At the same time, pass items to followees
# please see the file to understand why we do it in this way
file_inactive_relation_active = open(inactive_relation_active, "r")
for line in file_inactive_relation_active:
    strs = line.split()
    if strs[0] not in inactive_user:
        continue
    # notice, the last element of the array is "\n", which should be skipped
    followee_and_weight = strs[1].split(";")
    followee_len = len(followee_and_weight)
    for one in followee_and_weight[0 : (followee_len - 2)]:
        # set the inactive user's followees and their weight
        [followee, weight] = one.split(",")
        inactive_user[strs[0]]["followees"][followee] = weight
        # set the inactive user's followees' items
        for i in range(1, 12):
            if followee in active_user_group[i]:
                for item in inactive_user[strs[0]]["items"]:
                    active_user_group[i][followee]["followeeitems"][item] = 0
                break
file_inactive_relation_active.close()

print "iterate all the R matrices to get the relation between active users and items"
file_user_item_recommendation = open(user_item_recommendation, "w")
# iterate all the R matrices to get the relation between active users and items
for i in range(1, 12):
    R_matrix = R_matrix_param % (str(i))
    file_R_matrix = open(R_matrix, "r")
    j = 0
    for line in file_R_matrix:
        userID = group_active_user[i][j]
        Rui = line.split()
        if len(active_user_group[i][userID]["ownitems"]) != 0:
            for item in active_user_group[i][userID]["ownitems"]:
                active_user_group[i][userID]["ownitems"][item] = float(Rui[item])
            # for active user, we already get the result
            sorteditem = sorted(active_user_group[i][userID]["ownitems"].iteritems(), \
                         key=lambda d:d[1], reverse = True)
            count = 0
            total_item_num = len(sorteditem)
            file_user_item_recommendation.write(userID + "\t")
            for item in sorteditem:
                file_user_item_recommendation.write(number_itemID[item[0]])
                count = count + 1
                if count == 3 or count == total_item_num:
                    break
                file_user_item_recommendation.write(" ")
            file_user_item_recommendation.write("\n")
            # end of writing file
        if "followeeitems" in active_user_group[i][userID]:
            for item in active_user_group[i][userID]["followeeitems"]:
                active_user_group[i][usrID]["followeeitems"][item] = float(Rui[item])
        j = j + 1
    file_R_matrix.close()

print "get the first 0~3 recommendations for inactive users"
# get the first 0~3 recommendations for inactive users ###########################
for one in inactive_user:
    for followee in inactive_user[one]["followees"]:
        for i in range(1, 12):
            if followee in active_user_group[i]:
                for item in inactive_user[one]["items"]:
                    inactive_user[one]["items"][item] += \
                    float(inactive_user[one]["followees"][followee]) * \
                    float(active_user_group[i][followee]["followeritems"][item])
                break
    # for inactive user, we already get the result
    sorteditem = sorted(inactive_user[one]["items"].iteritems(), key = lambda d:d[1], \
                 reverse = True)
    count = 0
    total_item_num = len(sorteditem)
    file_user_item_recommendation.write(userID + "\t")
    for item in sorteditem:
        file_user_item_recommendation.write(number_itemID[item[0]])
        count = count + 1
        if count == 3 or count == total_item_num:
            break
        file_user_item_recommendation.write(" ")
    file_user_item_recommendation.write("\n")
    # end of writing file
file_user_item_recommendation.close()
