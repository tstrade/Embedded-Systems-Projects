#ifndef __HC05_COMMANDS_H__
#define __HC05_COMMANDS_H__

#include <inttypes.h>
#include <stdarg.h>

/*
    AT Command Default:
    https://components101.com/sites/default/files/component_datasheet/HC-05%20Datasheet.pdf

*/


typedef struct cmd
{
    uint16_t num_param;
    uint16_t num_response;
    char *arg;
    char *response;
} cmd;

extern cmd test;
extern cmd reset;
extern cmd firmware_version;
extern cmd restore_default;

extern cmd get_module_addr;

extern cmd set_module_name;
extern cmd check_module_name;

extern cmd get_btdev_name;

extern cmd set_module_mode;
extern cmd check_module_mode;

extern cmd set_device_class;
extern cmd check_device_class;

extern cmd set_giac;
extern cmd check_giac;

extern cmd set_query_patterns;
extern cmd check_query_patterns;

extern cmd set_pin_code;
extern cmd check_pin_code;

extern cmd set_serial_param;
extern cmd check_serial_param;

extern cmd set_connect_mode;
extern cmd check_connect_mode;

extern cmd set_fixed_addr;
extern cmd check_fixed_addr;

extern cmd set_led_io;
extern cmd check_led_io;

extern cmd set_pio_output;

extern cmd set_scan_param;
extern cmd check_scan_param;

extern cmd set_sniff_param;
extern cmd check_sniff_param;

extern cmd set_security_mode;
extern cmd check_security_mode;

extern cmd delete_auth_dev;
extern cmd delete_all_auth_dev;
extern cmd search_auth_dev;
extern cmd get_auth_dev_count;
extern cmd mru_auth_dev;

extern cmd get_module_working_state;
extern init_spp_profile_lib;

extern cmd inquiry_btdev;
extern cmd cancel_inquiry_btdev;

extern cmd equipment_matching;

extern cmd connect_device;
extern cmd disconnect;


void send_command ( cmd, ... );



#endif /* __HC05_COMMANDS_H__ */
