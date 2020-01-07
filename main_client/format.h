#ifndef _FORMAT_H_
#define _FORMAT_H_
#include <string.h> 

#define MAX_MESSAGE_TYPE 5
#define MAX_SIZE_MESSAGE 1024
#define SPLITSYMBOL "-"

// Client message
#define MESSAGE_LOG_IN_REQUEST 		'1'
#define MESSAGE_LOG_OUT_REQUEST 	'2'
#define MESSAGE_SCORE_REQUEST 		'3'

// Server message
#define MESSAGE_LOG_IN_SUCCESS 		'1'
#define MESSAGE_LOG_OUT_SUCCESS 	'2'
#define MESSAGE_LOG_IN_FAIL		 	'3'
#define MESSAGE_SCORE_FAIL	 		'4'
#define MESSAGE_SCORE_SUCCESS	 	'5'


typedef struct student{
    char* student_id;
    char* student_pass;
    float student_mark;
    int student_status;
} student;

typedef struct message{
    char message_type;
    char* student_id;
    char* message_data;
} message;


#endif