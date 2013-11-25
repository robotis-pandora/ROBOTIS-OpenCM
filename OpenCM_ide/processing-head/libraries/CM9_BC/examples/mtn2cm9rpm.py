#! /usr/bin/env python3.1

'''
 *******************************************************************************
 *  LEGAL STUFF
 *******************************************************************************
 *  Copyright (c) 2011, 2013 Matthew Paulishen. All rights reserved.
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *  
 *******************************************************************************
'''

import re
from optparse import OptionParser
from datetime import datetime


parser = OptionParser()
parser.add_option(  '-i', '--infile', dest='inFile',
                    help='Name of input file (required)', metavar='FILE',
                    action='store', type='string')
parser.add_option(  '-p', '--prefix', dest='seqPrefix',
                    help='Header file prefix (recommended)', metavar='string',
                    action='store', type='string')
parser.add_option(  "-T", "--task", dest="directlyTaskCompatible", default=False,
                    help="Produce an RPM array using same motion page indices as a Task program",
                    action="store_true")

(options, args) = parser.parse_args()
inFile = options.inFile
seqPrefix = options.seqPrefix
directlyTaskCompatible = options.directlyTaskCompatible

if (not inFile or len(inFile)<4):
    print('Minimum command-line options for proper usage is:\n python mtn2cm9rpm.py -i SomeRoboPlusMotionFile.mtn')
    exit()

if (not seqPrefix):
    seqPrefix='HelloRobo'


posType = 'bc_pose_t '
seqType = 'bc_seq_t '
rpmType = 'rpm_page_t '
storageDef = ' __FLASH__ '

templist = []
pmetlist = []
splitlist = []

tempposelist = []
temppagelist = []

rawposelist = []
rawpagelist = []

seqnamelist = []


outposelist = []
outseqlist = []
outrpmlist = []

outservolist = []

activeServos = []
numActiveServos = 0
typesOfServos = []

# open the input '.mtn' file and parse it a bit
moFi = open(inFile, 'r')

# RoboPlus Motion file format
# 'type='
# 'version='
# 'enable='
# 'motor_type='
  # for 1:1:256
    # 'page_begin'
    # 'name='
    # 'compliance='
    # 'play_param='
      # 0~7
        # 'step='
    # 'page_end'
# checksum

sequence_counter = 0
for line in moFi:

    # 'page_begin'
    if (re.match('page_begin', line) != None):
        sequence_counter += 1

        # 'name='
        name_line = moFi.next()

        # 'compliance='
        comp_line = moFi.next()
        
        # 'play_param='
        play_line = moFi.next()
        temp=play_line[11:-1]
        splitlist=re.split(' ', temp)
        temppagelist.append(int(splitlist[0]))  # 'next' index
        temppagelist.append(int(splitlist[1]))  # 'stop' index
        
        # 'step=' (0~7 of these)
        poses_in_sequence = 0
        for index in range(7):
            vari_line = moFi.next()

            # 'page_end'
            if (re.match('page_end', vari_line) != None):
                break

            # Collects the servo positions within this step (pose)
            elif (re.match('step=',vari_line) != None):
                temp=vari_line[5:-1]
                tempposelist.append(temp)
                poses_in_sequence += 1

        # No steps
        #  Drop it from pose and sequence header files
        #  Set transition_t* of RPM sequencer_t to 0
        if (poses_in_sequence == 0):
            tempposelist.append('nope')

            if (not directlyTaskCompatible):
                sequence_counter -= 1
           

        if (re.match('name=[a-zA-Z0-9][a-zA-Z0-9]*', name_line) != None):
            seqnamelist.append(name_line[5:-1].replace(' ', '') + '_' + str(sequence_counter))
        else:
            seqnamelist.append('unnamed_' + str(sequence_counter))

 
        rawpagelist.append(temppagelist[:])
        del temppagelist[:]
        rawposelist.append(tempposelist[:])
        del tempposelist[:]



    # 'enable='
    elif (re.match('enable=', line) != None):
        temp=line[7:-1]
        splitlist=re.split(' ', temp)
        # RoboPlus Motion only permits servos 0~25
        for index in range(26):
            activeServos.append(int(splitlist[index]))
            if (activeServos[index]==1):
                numActiveServos+=1

    # 'motor_type='
    elif (re.match('motor_type=', line) != None):
        temp=line[11:-1]
        splitlist=re.split(' ', temp)
        # RoboPlus Motion only permits servos 0~25
        for index in range(26):
            typesOfServos.append(int(splitlist[index]))

moFi.close()
#print(rawpagelist)
#print(rawposelist)
#exit()


# Create the active servo ID array
templist.append(posType + seqPrefix + '_id[]' + storageDef + '= {' + str(numActiveServos))

for index in range(25):
    if (activeServos[index]==1):
        templist.append(',' + str(index))
templist.append('};')
neweststr=''
outservolist.append(neweststr.join(templist))
del templist[:]






# run through the raw strings recovered from the input file and
#  finish cleanup/parsing
nextPoseDecrementCount = 0
for index in range(len(rawposelist)):
    if (re.match('nope',rawposelist[index][0]) == None):

        del templist[:]
        del pmetlist[:]
        
        # Add pointer to the servo ID array and number of poses in sequence
        #  as first entry in sequence array
        templist.append(seqType + seqPrefix + '_'  + seqnamelist[index] + '[]' + storageDef + '= {{' + seqPrefix + '_id,' + str(len(rawposelist[index])) + '}')

        # Add correct commented name of the sequence containing these poses
        pmetlist.append('// Sequence: ' + seqnamelist[index] + '\n')

        for pose in range(len(rawposelist[index])):
            # Start building a pose array
            pmetlist.append(posType + seqPrefix + '_' + seqnamelist[index] + '_' + str(pose+1) + '[]' + storageDef + '= {' + str(numActiveServos))

            # Split the pose string into integers
            splitlist=re.split(' ', rawposelist[index][pose])

            # Add pose values to pose array
            for indice in range(25):
                if (activeServos[indice]==1):
                    pmetlist.append(',' + str(splitlist[indice]))
            # Close out the pose array
            pmetlist.append('};\n')
            
            # Start building a sequence array
            # Add pose pointer and time-length of pose as next entry in sequence array
            templist.append(',{' + seqPrefix + '_'  + seqnamelist[index] + '_' + str(pose+1) + ',' + str(int(float(splitlist[27])*1000)) + '}')

            # If the last pose, close out the sequence array
            if (pose==len(rawposelist[index])-1):
                templist.append('};')
                newstr = ''
                outseqlist.append(newstr.join(templist))
                newerstr=''
                outposelist.append(newerstr.join(pmetlist))

        if (rawpagelist[index][0] > 0):
            nextPageIndex = rawpagelist[index][0]-nextPoseDecrementCount
        else:
            nextPageIndex = 0
        if (rawpagelist[index][1] > 0):
            stopPageIndex = rawpagelist[index][1]-nextPoseDecrementCount
        else:
            stopPageIndex = 0


        outrpmlist.append('\t{' + seqPrefix + '_'  + seqnamelist[index] + ',\t\t\t' + str(nextPageIndex) + ',\t\t' + str(stopPageIndex) + '}')

    elif (directlyTaskCompatible):
        outrpmlist.append('\t{0,\t\t\t\t\t\t0,\t\t0}')
    else:
        nextPoseDecrementCount += 1


    if (index == len(rawpagelist)):
        outrpmlist.append('};')

'''
        if (directlyTaskCompatible):
            if (index < len(rawposelist)-1):
                outrpmlist.append('\t{' + seqPrefix + '_'  + seqnamelist[index] + ',\t\t\t' + str(rawpagelist[index][0]) + ',\t\t' + str(rawpagelist[index][1]) + '},\t// ' + str(index+1) + '\n')
            elif (index == len(rawposelist)-1):
                outrpmlist.append('\t{' + seqPrefix + '_'  + seqnamelist[index] + ',\t\t\t' + str(rawpagelist[index][0]) + ',\t\t' + str(rawpagelist[index][1]) + '}\t// ' + str(index+1) + '\n};\n')
        else:
            if (index < len(rawposelist)-1):
                outrpmlist.append('\t{' + seqPrefix + '_'  + seqnamelist[index] + ',\t\t\t' + str(rawpagelist[index][0]) + ',\t\t' + str(rawpagelist[index][1]) + '},\t// ' + str(shorterPoseCounter) + '\n')
            elif (index == len(rawposelist)-1):
                outrpmlist.append('\t{' + seqPrefix + '_'  + seqnamelist[index] + ',\t\t\t' + str(rawpagelist[index][0]) + ',\t\t' + str(rawpagelist[index][1]) + '}\t// ' + str(shorterPoseCounter) + '\n};\n')
'''
'''
            if (index < len(rawposelist)-1):
                outrpmlist.append('\t{0,\t\t\t\t\t\t0,\t\t0},\t// ' + str(index+1) + '\n')
            elif (index == len(rawposelist)-1):
                outrpmlist.append('\t{0,\t\t\t\t\t\t0,\t\t0}\t// ' + str(index+1) + '\n};\n')
'''    
    


#print(servolist)
#print(templist)
#print(pmetlist)
#print(seqnamelist)
#print(outseqlist)
#print(outposelist)
#print(outrpmlist)
#exit()


outFile_pose = seqPrefix+'_Poses.h'
outFile_seq  = seqPrefix+'_Sequences.h'
outFile_rpm  = seqPrefix+'_RPM.h'

poFi = open(outFile_pose, 'w')

poFi.write('#ifndef _' + seqPrefix.upper() + '_POSES_H_\n#define _' + seqPrefix.upper() + '_POSES_H_\n\n#include <CM9_BC.h>\n\n')

poFi.write('// Motion file: ' + seqPrefix + '_' + inFile + '\n\n')

poFi.write(outservolist[0] + '\n\n')
for index in range(len(outposelist)):
    poFi.write(outposelist[index] + '\n\n')

poFi.write('#endif\n')
poFi.close()


poFi = open(outFile_seq, 'w')

poFi.write('#ifndef _' + seqPrefix.upper() + '_SEQUENCES_H_\n#define _' + seqPrefix.upper() + '_SEQUENCES_H_\n\n#include \"' + outFile_pose + '\"\n\n')

poFi.write('// Motion file: ' + seqPrefix + '_' + inFile + '\n\n')

for index in range(len(outseqlist)):
    poFi.write(outseqlist[index] + '\n\n')

poFi.write('#endif\n')
poFi.close()



poFi = open(outFile_rpm, 'w')

poFi.write('#ifndef _' + seqPrefix.upper() + '_RPM_H_\n#define _' + seqPrefix.upper() + '_RPM_H_\n\n#include \"' + outFile_seq + '\"\n\n')

poFi.write('// Motion file: ' + seqPrefix + '_' + inFile + '\n\n')

poFi.write(rpmType + seqPrefix + '_RoboPlusMotion_Array[]' + storageDef + '= {\n')

poFi.write('\t{0,\t\t\t\t\t\t0,\t\t' + str(len(outrpmlist)) + '},\t// 0\n')

for index in range(len(outrpmlist)-1):
    poFi.write(outrpmlist[index] + ',\t // ' + str(index+1) + '\n')
poFi.write(outrpmlist[len(outrpmlist)-1] + '\t // ' + str(len(outrpmlist)) + '\n};\n')

poFi.write('\n#endif\n')
poFi.close()

exit()

