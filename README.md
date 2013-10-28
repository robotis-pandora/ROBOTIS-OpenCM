ROBOTIS_OpenCM
==================

ROBOTIS OpenCM is a opensource project for robot controller.  
This repository provides all source codes including firmware and IDE.
  
  1. Firmware : This directory includes the bootloader for CM-9XX controller board and 
              user firmware codes we call it as core-library, is executed at address 0x08003000.
              The bootloader is executed at address 0x08000000.
  2. OpenCM_ide : This is for CM-9 IDE source codes based on Arduino 1.0.1 which also based on Processing.
              There are two directories in this folder, processing-core and processing-head.
              Processing-core is just library for processing component used in implementing cm-9 ide.
              Processing-head is real source codes for cm-9 ide, you can improve cm-9 ide by modifying it.

  3. Documents : Tutorial documents for OpenCM
  4. Hardware : All H/W development resource
  5. Notice_en.txt, Notice_ko.txt : License


