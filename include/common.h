/*
 * common.h
 *
 *  Created on: 28.10.2013
 *      Author: Anton
 */

#ifndef COMMON_H_
#define COMMON_H_

int getLocation(char *location);
int getLocationAdapter(char *result);
int hdlrLocation(char *args, char *result);

int getGSMLocation(char *location);
int getGSMLocationAdapter(char *result);
int hdlrGSMLocation(char *args, char *result);

int getBattery(void);
int getBatteryAdapter(char *result);
int hdlrBattery(char *args, char *result);

int getNumber(char *number, uint8_t num);
int getNumberAdapter(char *number, uint8_t num);

int setNumber(char *number, uint8_t num);
int setNumberAdapter(char *number, char *result);
int hdlrNumber(char *args, char *result);

//int setAccel(FunctionalState *stat);		// function in lis331 driver
int setAccelAdapter(char *stat, char *result);
int hdlrAccel(char *args, char *result);

int getTimeUpdate(uint8_t *interval);
int getTimeUpdateAdapter(char *result);

int setTimeUpdate(uint8_t *interval);
int setTimeUpdateAdapter(char *var, char *result);
int hdlrTimeUpdate(char *args, char *result);

int getPass(char *pass);

int setPass(char *pass);
int setPassAdapter(char *pass, char *result);
int hdlrPass(char *args, char *result);

int setDefault(void);
int setDefaultAdapter(char *result);
int hdlrDefault(char *args, char *result);

int setEchoAdapter(char *var, char *result);
int hdlrDefault(char *args, char *result);

int hdlrEcho(char *args, char *result);

#endif /* COMMON_H_ */
