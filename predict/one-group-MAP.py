import sys

solution = "../track1/KDD_Track1_solution.csv"
user_item_recommendation = "user_item_recommendation_%s.txt" % (str(sys.argv[1]))


# read users' clicks
user_click = dict()
file_solution = open(solution, "r")
for line in file_solution:
    strs = line.split(",")
    if strs[0] in user_click:
        user_click[strs[0]].extend(strs[1].split(" "))
    else:
        user_click[strs[0]] = strs[1].split(" ")
file_solution.close()
print "the number of lines in solution file is " + str(len(user_click)) + "\n"


# read the recommendation to users
file_user_item_recommendation = open(user_item_recommendation, "r")
i = 0
AP3 = 0
for line in file_user_item_recommendation:
    strs = line.split()
    if strs[0] in user_click:
        strs_len = len(strs)
        j = 0
        ap3 = 0
        number_of_clicks = 0
    #print user_click[strs[0]]
    #print strs
    #print strs[1 : strs_len]
    #print user_click[strs[0]]
        for item in strs[1 : strs_len]:
            j += 1
            if item in user_click[strs[0]]:
               number_of_clicks += 1
               ap3 += float(number_of_clicks) / float(j)
        if number_of_clicks != 0:
            ap3 = float(ap3) / float(number_of_clicks)
        else: 
            ap3 = 0
        #print "when ap3 is 0, i is " + str(i)
        AP3 += float(ap3)
    #print i
    #print AP3
    i = i + 1
file_user_item_recommendation.close()
print "the number of lines in recommendation file is " + str(i) + "\n"
AP3 = float(AP3) / float(i)
print "AP@3 is " + str(AP3)

# we should check the number of lines of each file to justify if the 
# result is right or not.
