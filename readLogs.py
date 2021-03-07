import csv
import sys

#arg 1 = QA file
#arg 2 = Log file
#arg 3 = Run number

outputName = "compiledInfo_"
outputName += str(sys.argv[3])
outputName += ".tsv"

# Lists of the SiPM IDs. These elements are 
#meant to be removed when found so that at the
#end the elements remaining are those that are
#missing from the files.
nEcalIDs = range(748)
for i in range(len(nEcalIDs)):   # Add the leading zeros
    number = str(nEcalIDs[i])
    if i < 10:
        number = "00" + number
    elif i >= 10 and i < 100:
        number = "0" + number

    nEcalIDs[i] = "EN" + number

sEcalIDs = range(748)
for i in range(len(sEcalIDs)):
    number = str(sEcalIDs[i])
    if i < 10:
        number = "00" + number
    elif i >= 10 and i < 100:
        number = "0" + number

    sEcalIDs[i] = "ES" + number

nHcalIDs = range(260)
for i in range(len(nHcalIDs)):
    number = str(nHcalIDs[i])
    if i < 10:
        number = "00" + number
    elif i >= 10 and i < 100:
        number = "0" + number

    nHcalIDs[i] = "HN" + number

sHcalIDs = range(260)
for i in range(len(sHcalIDs)):
    number = str(sHcalIDs[i])
    if i < 10:
        number = "00" + number
    elif i >= 10 and i < 100:
        number = "0" + number

    sHcalIDs[i] = "HS" + number


with open(outputName, "w") as outputFile:
    tsv_writer = csv.writer(outputFile, delimiter='\t')

    #qaFile  = open("QA_1106920.txt", "r")
    #logFile = open("LogFile_1106920.log", "r")
    qaFile  = open(str(sys.argv[1]), "r")
    logFile = open(str(sys.argv[2]), "r")


    def hCalRow(dep, group):
        if dep == "6" and group == "0": return 1
        if dep == "0" and group == "0": return 3
        if dep == "2" and group == "0": return 5
        if dep == "1" and group == "1": return 7
        if dep == "8" and group == "0": return 9
        if dep == "8" and group == "1": return 11
        if dep == "3" and group == "0": return 13
        if dep == "5" and group == "0": return 15
        if dep == "4" and group == "1": return 17
        if dep == "7" and group == "0": return 19

    def eCalRow(dep, group):
        if dep == "20" and group == "0": return 1
        if dep == "0" and group == "0": return 3
        if dep == "1" and group == "0": return 5
        if dep == "22" and group == "0": return 7
        if dep == "5" and group == "0": return 9
        if dep == "6" and group == "0": return 11
        if dep == "7" and group == "0": return 13
        if dep == "8" and group == "0": return 15
        if dep == "9" and group == "0": return 17
        if dep == "10" and group == "0": return 19
        if dep == "11" and group == "0": return 21
        if dep == "12" and group == "0": return 23
        if dep == "13" and group == "0": return 25
        if dep == "14" and group == "0": return 27
        if dep == "22" and group == "1": return 29
        if dep == "18" and group == "0": return 31
        if dep == "19" and group == "0": return 33



    # Read QA text file
    qaNameList = []
    qaCodeList = []
    qaDetList = []
    qaSideList = []
    qaIdList = []
    qaLedList = []
    qaFlagList = []

    for line in qaFile:
        elements = line.split()
        totalElements = len(elements)

        name = elements[1]
        if name == "name": continue   # Skip first line that just has the column labels

        code = name.split('_')[0]   # HN034, ES123, etc.
        led = elements[4]

        if code[0] == "E":   detector = "0"
        elif code[0] == "H": detector = "1"
        elif code[0] == "P": continue
            
        if code[1] == "N":   side = "0"
        elif code[1] == "S": side = "1"
            
        sipmID = code[2:]      # Last 3 characters of 'code'

        if sipmID[0] == "0": sipmID = code[3:]  # Last 2 chars

        flag = elements[7]
        if totalElements > 8:
            extraFlags = totalElements - 8
            for x in range(extraFlags):
                flag += "_"
                flag += elements[x+8]
        

        qaNameList.append(name)
        qaCodeList.append(code)
        qaDetList.append(detector)
        qaSideList.append(side)
        qaIdList.append(sipmID)
        qaLedList.append(led)
        qaFlagList.append(flag)

    print str(sys.argv[1]), "read in."

    # Read log file
    logDetList = []
    logSideList = []
    logDepList = []
    logGroupList = []
    logAddrList = []
    logVsldacList = []
    logAdc0List = []
    logAdc1List = []
    logAdc2List = []
    logAdc3List = []

    for line in logFile:
        elements = line.split()

        if elements[0] == "#": continue
        
        detector = elements[0]
        side = elements[1]
        dep  = elements[2]
        group = elements[3]
        addr = elements[4]
        vsldac = elements[6]
        adc0 = elements[19]
        adc1 = elements[20]
        adc2 = elements[21]
        adc3 = elements[22]
        
        logDetList.append(detector)
        logSideList.append(side)
        logDepList.append(dep)
        logGroupList.append(group)
        logAddrList.append(addr)
        logVsldacList.append(vsldac)
        logAdc0List.append(adc0)
        logAdc1List.append(adc1)
        logAdc2List.append(adc2)
        logAdc3List.append(adc3)


    print str(sys.argv[2]), "read in."


    #lists are filled, now match the log info to the qa info and write to a new file
    for i in range(len(logDetList)):
        if logDetList[i] == "0":   #ECAL
            erow = eCalRow(logDepList[i],logGroupList[i])
            emap = ((erow-1)/2)*11 + int(logAddrList[i])

            etwid=((erow-1)/2)*22+(emap*2);
            etwid_nxcol=(((erow-1)/2)*22)+((emap*2)+1);
            etwid_nxrow=(((erow-1)/2+1)*22)+(emap*2);
            etwid_nxrow_nxcol=(((erow-1)/2+1)*22)+(emap*2+1);
        
        elif logDetList[i] == "1":   #HCAL
            hrow = hCalRow(logDepList[i],logGroupList[i])
            hmap = ((hrow-1)/2)*13 + int(logAddrList[i])

            htwid=((hrow-1)/2)*13+hmap;
            htwid_nxrow=((hrow-1)/2+1)*13+hmap;


        for j in range(len(qaDetList)):
            if logDetList[i] == qaDetList[j] and logSideList[i] == qaSideList[j]:   #Same detector and side
                
                if qaDetList[j] == "0" and qaSideList[j] == "0":   #ECal North
                    if int(qaIdList[j]) == etwid:
                        tsv_writer.writerow([qaNameList[j], qaLedList[j], logDetList[i], logSideList[i], logDepList[i],
                                             logGroupList[i], logAddrList[i], logVsldacList[i], logAdc0List[i], qaFlagList[j]])
                        foundCode = qaCodeList[j]
                        if foundCode in nEcalIDs: nEcalIDs.remove(foundCode)
                    elif int(qaIdList[j]) == etwid_nxcol:
                        tsv_writer.writerow([qaNameList[j], qaLedList[j], logDetList[i], logSideList[i], logDepList[i],
                                             logGroupList[i], logAddrList[i], logVsldacList[i], logAdc1List[i], qaFlagList[j]])
                        foundCode = qaCodeList[j]
                        if foundCode in nEcalIDs: nEcalIDs.remove(foundCode)
                    elif int(qaIdList[j]) == etwid_nxrow:
                        tsv_writer.writerow([qaNameList[j], qaLedList[j], logDetList[i], logSideList[i], logDepList[i],
                                             logGroupList[i], logAddrList[i], logVsldacList[i], logAdc2List[i], qaFlagList[j]])
                        foundCode = qaCodeList[j]
                        if foundCode in nEcalIDs: nEcalIDs.remove(foundCode)
                    elif int(qaIdList[j]) == etwid_nxrow_nxcol:
                        tsv_writer.writerow([qaNameList[j], qaLedList[j], logDetList[i], logSideList[i], logDepList[i],
                                             logGroupList[i], logAddrList[i], logVsldacList[i], logAdc3List[i], qaFlagList[j]])
                        foundCode = qaCodeList[j]
                        if foundCode in nEcalIDs: nEcalIDs.remove(foundCode)
                    
                
                elif qaDetList[j] == "0" and qaSideList[j] == "1":   #ECAL South
                    if int(qaIdList[j]) == etwid:
                        tsv_writer.writerow([qaNameList[j], qaLedList[j], logDetList[i], logSideList[i], logDepList[i],
                                             logGroupList[i], logAddrList[i], logVsldacList[i], logAdc1List[i], qaFlagList[j]])
                        foundCode = qaCodeList[j]
                        if foundCode in sEcalIDs: sEcalIDs.remove(foundCode)
                    elif int(qaIdList[j]) == etwid_nxcol:
                        tsv_writer.writerow([qaNameList[j], qaLedList[j], logDetList[i], logSideList[i], logDepList[i],
                                             logGroupList[i], logAddrList[i], logVsldacList[i], logAdc0List[i], qaFlagList[j]])
                        foundCode = qaCodeList[j]
                        if foundCode in sEcalIDs: sEcalIDs.remove(foundCode)
                    elif int(qaIdList[j]) == etwid_nxrow:
                        tsv_writer.writerow([qaNameList[j], qaLedList[j], logDetList[i], logSideList[i], logDepList[i],
                                             logGroupList[i], logAddrList[i], logVsldacList[i], logAdc3List[i], qaFlagList[j]])
                        foundCode = qaCodeList[j]
                        if foundCode in sEcalIDs: sEcalIDs.remove(foundCode)
                    elif int(qaIdList[j]) == etwid_nxrow_nxcol:
                        tsv_writer.writerow([qaNameList[j], qaLedList[j], logDetList[i], logSideList[i], logDepList[i],
                                             logGroupList[i], logAddrList[i], logVsldacList[i], logAdc2List[i], qaFlagList[j]])
                        foundCode = qaCodeList[j]
                        if foundCode in sEcalIDs: sEcalIDs.remove(foundCode)
                        
                
                elif qaDetList[j] == "1" and qaSideList[j] == "0":   #HCal North
                    if int(qaIdList[j]) == htwid:
                        tsv_writer.writerow([qaNameList[j], qaLedList[j], logDetList[i], logSideList[i], logDepList[i],
                                             logGroupList[i], logAddrList[i], logVsldacList[i], logAdc0List[i], qaFlagList[j]])
                        foundCode = qaCodeList[j]
                        if foundCode in nHcalIDs: nHcalIDs.remove(foundCode)
                    elif int(qaIdList[j]) == htwid_nxrow:
                        tsv_writer.writerow([qaNameList[j], qaLedList[j], logDetList[i], logSideList[i], logDepList[i],
                                             logGroupList[i], logAddrList[i], logVsldacList[i], logAdc1List[i], qaFlagList[j]])
                        foundCode = qaCodeList[j]
                        if foundCode in nHcalIDs: nHcalIDs.remove(foundCode)
                    
                
                elif qaDetList[j] == "1" and qaSideList[j] == "1":   #HCAL South
                    if int(qaIdList[j]) == htwid:
                        tsv_writer.writerow([qaNameList[j], qaLedList[j], logDetList[i], logSideList[i], logDepList[i],
                                             logGroupList[i], logAddrList[i], logVsldacList[i], logAdc1List[i], qaFlagList[j]])
                        foundCode = qaCodeList[j]
                        if foundCode in sHcalIDs: sHcalIDs.remove(foundCode)
                    elif int(qaIdList[j]) == htwid_nxrow:
                        tsv_writer.writerow([qaNameList[j], qaLedList[j], logDetList[i], logSideList[i], logDepList[i],
                                             logGroupList[i], logAddrList[i], logVsldacList[i], logAdc0List[i], qaFlagList[j]])
                        foundCode = qaCodeList[j]
                        if foundCode in sHcalIDs: sHcalIDs.remove(foundCode)
                    


    print
    print "SiPMs that were not matched up with FEEs:"
    for x in nEcalIDs: print x
    for x in sEcalIDs: print x
    for x in nHcalIDs: print x
    for x in sHcalIDs: print x
    print 

    qaFile.close()
    logFile.close()

    print "Done!"
