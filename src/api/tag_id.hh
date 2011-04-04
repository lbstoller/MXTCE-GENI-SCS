#ifndef _tag_id_hh_
#define _tag_id_hh_

#define PCE_USERCONSTRAINT 0xF1
//#define PCE_RESERVEDCONSTRAINT 0xF1
#define PCE_RESVCONSTRAINT 0xF2

#define PCE_REPLY 0xF8

	
#define PCE_GRI   0x81
#define PCE_LOGIN 0x82
#define PCE_LAYER 0x83
#define PCE_SOURCE 0x84
#define PCE_DESTINATION 0x85
#define PCE_BANDWIDTH 0x86
#define PCE_DESCRIPTION 0x87
#define PCE_STARTTIME 0x88
#define PCE_ENDTIME 0x89
#define PCE_PATHSETUPMODEL 0x8A
#define PCE_PATHTYPE 0x8B
#define PCE_SRCVLAN 0x8C
#define PCE_DESTVLAN 0x8D
#define PCE_PATH 0x8E
#define PCE_SRCIPPORT 0x8F
#define PCE_DESTIPPORT 0x90
#define PCE_L3_PROTOCOL 0x91
#define PCE_L3_DSCP 0x92
#define PCE_MPLS_BURSTLIMIT 0x93
#define PCE_MPLS_LSPCLASS 0x94

//#define PCE_HOP 0x95

#define PCE_PATH_ID 0x95
#define PCE_HOP_ID 0x96
#define PCE_LINK_ID 0x97
#define PCE_SWITCHINGCAPTYPE 0x98
#define PCE_SWITCHINGENCTYPE 0x99
#define PCE_SWITCHINGVLANRANGEAVAI 0x9A
#define PCE_PATH_LENGTH 0x9B

#define PCE_TEST1 0xE0
#define PCE_TEST2 0xE1
#define PCE_TEST3 0xE2
#define PCE_TEST4 0xE3
#define PCE_TEST5 0xE4
	
#define ASN_LONG_LEN 0x80


#define BOOLEAN 0x01
#define INTEGER_NUM 0x02
#define ENUMERATED_NUM 0x03
#define FLOAT_NUM 0x04
#define DOUBLE_NUM 0x05
#define CHARACTER_STRING 0x06

#endif
