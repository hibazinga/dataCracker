import sys
test_file = "../track1/rec_log_test.txt"
user_profile = "../track1/user_profile.txt"
user_profile_group= "../predata/user_profile_%s.txt" % (str(sys.argv[1]))
R_matrix = "../training/model%s.csv" % (str(sys.argv[1]))# TODO: change the matrix name according to the real name
item_ID = "../track1/item.txt"
user_item_recommendation = "user_item_recommendation_%s.txt" % (str(sys.argv[1]))


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


# get all the group's active userIDs ####################
active_user_group = dict()
group_active_user = dict()
file_user_profile_group = open(user_profile_group, "r")
group_userId = file_user_profile_group.readline().split()
file_user_profile_group.close()
j = 0
for ID in group_userId:
    group_active_user[j] = ID
    active_user_group[ID] = dict()
    active_user_group[ID]["id"] = j
    active_user_group[ID]["ownitems"] = dict()
    j = j + 1
print j
print "read test file\n"
# read the test file #####################################
file_test = open(test_file, "r")
i = 0
for line in file_test:
    strs = line.split()
    if strs[0] in active_user_group:
        active_user_group[strs[0]]["ownitems"][itemID_number[strs[1]]] = 0
    i = i + 1
file_test.close()

print "get rating\n"
file_user_item_recommendation = open(user_item_recommendation, "w")
# iterate all the R matrices to get the relation between active users and items
file_R_matrix = open(R_matrix, "r")
j = 0
for line in file_R_matrix:
    userID = group_active_user[j]
    Rui = line.split()
    if len(active_user_group[userID]["ownitems"]) != 0:
        for item in active_user_group[userID]["ownitems"]:
            active_user_group[userID]["ownitems"][item] = float(Rui[item])
        # for active user, we already get the result
        sorteditem = sorted(active_user_group[userID]["ownitems"].iteritems(), \
                      key=lambda d:d[1], reverse = True)
        #if j == 0:
            #for item in sorteditem:
                #print str(item[0]) + " " + str(item[1]) + "\n"
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
    j = j + 1
file_R_matrix.close()
file_user_item_recommendation.close()
