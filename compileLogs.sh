#!/bin/bash

#Here for each pair of QA/Log files just write a separate line of:
#python readLogs.py [QA file] [Log file] [run number]

DIR="LEDresponse"

python readLogs.py ${DIR}/ped_1107038.txt ${DIR}/fcs_all_physics_01107038.log 1107038
python readLogs.py ${DIR}/ped_1107039.txt ${DIR}/fcs_all_physics_01107039.log 1107039
python readLogs.py ${DIR}/ped_1107040.txt ${DIR}/fcs_all_physics_01107040.log 1107040
python readLogs.py ${DIR}/ped_1107041.txt ${DIR}/fcs_all_physics_01107041.log 1107041
python readLogs.py ${DIR}/ped_1107042.txt ${DIR}/fcs_all_physics_01107042.log 1107042
