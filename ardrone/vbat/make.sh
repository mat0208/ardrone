#!/bin/bash
arm-none-linux-gnueabi-g++ -o ../bin/vbat vbat.c main_vbat.c
arm-none-linux-gnueabi-strip ../bin/vbat
