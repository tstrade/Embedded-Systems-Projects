#ifndef __HC05_COMMANDS_H__
#define __HC05_COMMANDS_H__

#pragma once

#include <inttypes.h>
#include <stdarg.h>

/*
    AT Command Default:
    https://components101.com/sites/default/files/component_datasheet/HC-05%20Datasheet.pdf

*/

/*
Struct comms =
{
    { , "OK" },
    { "AT_NAME=", "OK" },
    { "AT+NAME?", "+NAME:", "OK", "FAIL" },
    { "AT+ROLE=", "OK" },
    { "AT+ROLE?", "+ROLE:", "OK" },
    { "AT+STATE?", "+STATE:", "OK" },

    { "AT+RNAME?", "+NAME:", "OK", "FAIL" },
    { "AT+CLASS=", "OK" },
    { "AT+CLASS?", "+CLASS:", "OK", "FAIL" },
    { "AT+INQ", "+INQ:", "OK" },
    { "AT+INQC", "OK" },
    { "AT+PAIR=", "OK", "FAIL" },
    { "AT+DISC", "+DISC:SUCCESS", "+DISC:LINK_LOSS", "+DISC:NO_SLC", "+DISC:TIMEOUT", "+DISC:ERROR", "OK" },

    { "AT+PMSAD=", "OK" },
    { "AT+RMAAD", "OK" },
    { "AT+FSAD=", "OK", "FAIL" },
    { "AT+ADCN?", "+ADCN:", "OK" },
    { "AT+MRAD?", "+MRAD:", "OK" },

    { "AT+IAC=", "OK", "FAIL" },
    { "AT+IAC", "+IAC:", "OK" },
    { "AT+INQM=", "OK", "FAIL" },
    { "AT+INQM?", "+INQM:", "OK" },
    { "AT+PSWD=", "OK" },
    { "AT+PSWD?", "+PSWD:", "OK" },
    { "AT+UART=", "OK" },
    { "AT+UART?", "+UART=", "OK" }
    { "AT+CMODE=", "OK" }
    { "AT+CMODE?", "+CMODE:", "OK" },
    { "AT+BIND=", "OK" },
    { "AT+BIND?", "+BIND:", "OK" },
    { "AT+POLAR=", "OK" },
    { "AT+POLAR?", "+POLAR=", "OK" },
    { "AT+PIO=", "OK" },
    { "AT+IPSCAN=", "OK" },
    { "AT+IPSCAN?", "+IPSCAN:", "OK" },
    { "AT+SNIFF=", "OK" },
    { "AT+SNIFF?", "+SNIFF:", "OK" },
    { "AT+SENM=", "OK", "FAIL" },
    { "AT+SENM?", "+SENM:", "OK" },

    { "AT+INIT", "OK", "FAIL" },
    { "AT+PAIR=", "OK", "FAIL" },
    { "AT+ENSNIFF=", "OK" },
    { "AT+EXSNIFF=", "OK" }
}
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
extern cmd get_module_name;




/*
cmd set_module_mode[2]; // Param: 0= Slave, 1= Master, 2= Slave-Loop
cmd get_module_mode[3];
cmd get_module_state[3]; // INITIALIZED, READY, PAIRABLE, PAIRED, INQUIRING, CONNECTING, DISCONNECTED, NUKNOW

 Device Info
cmd get_btdev_name[4]; // Param: Address of Bluetooth device
cmd set_btdev_class[2]; // Param: Device Class
cmd get_btdev_class[4];
cmd inquiry_btdev[3];
cmd cancel_inquiry_btdev[2];
cmd connect_device[3]; // Param: Device addr
cmd disconnect_btdev[7]; // SUCCESS, LINK_LOSS, NO_SLC, TIMEOUT, ERROR

 Authenticated Devices
cmd delete_auth_dev[2]; // Param: Authenticated Device addr
cmd delete_all_auth_dev[2];
cmd search_auth_dev[3]; // Param: Device addr
cmd get_auth_dev_count[3];
cmd mru_auth_dev[3];

 General Inquire Access Code
cmd set_giac[3]; // Param: GIAC (Default: 9e8b33)
cmd get_giac[3];

 Configurations
cmd set_query_access_pattern[3]; // Param 1: 0= standard, 1= rssi
                                            // Param 2: Max # of BT devices to respond to
                                            // Param 3: Timeout (1-48 : 1.28s - 61.44s)
cmd get_query_access_pattern[3];

cmd set_pin_code[2]; // Param: PIN code (Default 1234)
cmd get_pin_code[3];

cmd set_serial_parameter[2]; // Param 1: Baud
                                        // Param 2: Stop bit
                                        // Param 3: Parity
cmd get_serial_parameter[3];

cmd set_connect_mode[2]; // Param: 0= connect fixed addr, 1= connect any addr, 2= slave-loop
cmd get_connect_mode[3];

cmd set_fixed_addr[2]; // Param: Fixed address (Default 00:00:00:00:00:00)
cmd get_fixed_addr[3];

cmd set_led_io[2]; // Param 1: 0= PIO8 low drive LED, 1= PIO8 high drive LED
                            // Param 2: 0= PIO9 low drive LED, 1= PIO9 high drive LED
cmd get_led_io[3];

cmd set_pio_output[2]; // Param 1: PIO number
                                // Param 2: PIO level (0= low, 1= high)

cmd set_scan_parameter[2]; // Param 1: Query time interval
                                    // Param 2: Query duration
                                    // Param 3: Paging interval
                                    // Param 4: Call duration
cmd get_scan_parameter[3];

cmd set_shiff_parameter[2]; // Param 1: Max time
                                    // Param 2: Min time
                                    // Param 3: Retry time
                                    // Param 4: Time out
cmd get_shiff_paramter[3];

cmd set_security_mode[3]; // Param 1 (Sec mode): 0= off, 1= non-secure, 2= service, 3= link, 4= mode unknown
                                    // Param 2 (Hci mode): 0= off, 1= pt to pt, 2= pt to pt and bcast
cmd get_security_mode[3];


cmd init_spp_profile[3];
cmd equipment_matching[3]; // Param 1: Device addr
                                    // Param 2: Time out

cmd energy_saving_mode[2]; // Param: Device addr
cmd exerts_energy_saving_mode[2]; // Param: Device addr
*/


void send_command ( cmd, ... );



#endif /* __HC05_COMMANDS_H__ */
