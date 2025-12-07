#ifndef __BLUETOOTH_COMMS_LIBRARY_H__
#define __BLUETOOTH_COMMS_LIBRARY_H__

#include <inttypes.h>

extern int strcmp ( char *, char * );
extern char *strcat ( char *, char * );
extern int strlen ( char * );
extern void strcpy ( char *, char * );

extern void output_string ( char * );
extern void read_string ( char * );
extern void output_hc05_cmd ( char * );
extern void read_hc05_rsp ( uint16_t );

extern void clear_cmd_buffer ( void );
extern void clear_rsp_buffer ( void );

extern char *cmd_buff_ptr;
extern char *rsp_buff_ptr;


#endif /* __BLUETOOTH_COMMS_LIBRARY_H__ */
