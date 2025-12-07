#include <stdarg.h>
#include "HC05_Commands.h"
#include "bluetooth_comms_library.h"

static void read_command ( cmd );


void
send_command ( cmd command, ... )
{
    char *cmd_buffer = cmd_buff_ptr;
    va_list args;
    va_start (args, command);


    switch (command.num_param)
    {
    case 0:
        strcpy (command.arg, cmd_buffer);
        break;
    case 1:
        cmd_buffer = strcat (command.arg, va_arg (args, char *));
        break;
    case 2:
        cmd_buffer = strcat (command.arg, va_arg (args, char *));
        cmd_buffer = strcat (cmd_buffer, ", ");
        cmd_buffer = strcat (cmd_buffer, va_arg (args, char *));
        break;
    case 3:
        cmd_buffer = strcat (command.arg, va_arg (args, char *));
        cmd_buffer = strcat (cmd_buffer, ", ");
        cmd_buffer = strcat (cmd_buffer, va_arg (args, char *));
        cmd_buffer = strcat (cmd_buffer, ", ");
        cmd_buffer = strcat (cmd_buffer, va_arg (args, char *));
        break;
    case 4:
        cmd_buffer = strcat (command.arg, va_arg (args, char *));
        cmd_buffer = strcat (cmd_buffer, ", ");
        cmd_buffer = strcat (cmd_buffer, va_arg (args, char *));
        cmd_buffer = strcat (cmd_buffer, ", ");
        cmd_buffer = strcat (cmd_buffer, va_arg (args, char *));
        cmd_buffer = strcat (cmd_buffer, ", ");
        cmd_buffer = strcat (cmd_buffer, va_arg (args, char *));
        break;
    default:
        break;
    }

    cmd_buffer = strcat (cmd_buffer, "\r\n");
    output_hc05_cmd (cmd_buffer);
    read_command (command);
    clear_cmd_buffer ();
}

static void
read_command ( cmd command )
{
    read_hc05_rsp (command.num_response);
    output_string (rsp_buff_ptr);
    clear_rsp_buffer ();
}

cmd test = { 0, 1, "AT", "OK" };
cmd reset = { 0, 1, "AT+RESET", "OK" };
cmd firmware_version = { 0, 2, "AT+VERSION?", "+VERSION:" };
cmd restore_default = { 0, 1, "AT+ORGL", "OK" };

cmd get_module_addr = { 0, 2, "AT+ADDR?", "+ADDR:" };

cmd set_module_name = { 1, 1, "AT+NAME=", "OK" };
cmd check_module_name = { 0, 2, "AT+NAME?", "+NAME:" };

cmd get_btdev_name = { 1, 2, "AT+RNAME?", "+NAME:" };

cmd set_module_mode = { 1, 1, "AT+ROLE=", "OK" };
cmd check_module_mode = { 0, 1, "AT+ROLE?", "+ROLE:" };

cmd set_device_class = { 1, 1, "AT+CLASS=", "OK" };
cmd check_device_class = { 0, 2, "AT+CLASS?", "+CLASS:" };

cmd set_giac = { 1, 1, "AT+IAC=", "OK" };
cmd check_giac = { 0, 2, "AT+IAC?", "+IAC:" };

cmd set_query_patterns = { 3, 1, "AT_INQM=", "OK" };
cmd check_query_patterns = { 0, 2, "AT+INQM?", "+INQM:" };

cmd set_pin_code = { 1, 1, "AT+PSWD=", "OK" };
cmd check_pin_code = { 0, 2, "AT+PSWD", "+PSWD:"};

cmd set_serial_param = { 3, 1, "AT+UART=", "OK" };
cmd check_serial_param = { 0, 2, "AT+UART?", "+UART=" };

cmd set_connect_mode = { 1, 1, "AT+CMDOE=", "OK" };
cmd check_connect_mode = { 0, 2, "AT+CMODE?", "+CMODE:" };

cmd set_fixed_addr = { 1, 1, "AT+BIND=", "OK" };
cmd check_fixed_addr = { 0, 2, "AT+BIND?", "+BIND:" };

cmd set_led_io = { 2, 1, "AT+POLAR=", "OK" };
cmd check_led_io = { 0, 2, "AT+POLAR?", "+POLAR:" };

cmd set_pio_output = { 2, 1, "AT+PIO=", "OK" };

cmd set_scan_param = { 4, 1, "AT+IPSCAN=", "OK" };
cmd check_scan_param = { 0, 2, "AT+IPSCAN?", "+IPSCAN:" };

cmd set_sniff_param = { 4, 1, "AT+SNIFF=", "OK" };
cmd check_sniff_param = { 0, 2, "AT+SNIFF?", "+SNIFF:" };

cmd set_security_mode = { 2, 1, "AT+SENM=", "OK" };
cmd check_security_mode = { 0, 2, "AT+SENM?", "+SENM:" };

cmd delete_auth_dev = { 1, 1, "AT+PMSAD", "OK" };
cmd delete_all_auth_dev = { 0, 1, "AT_RMAAD", "OK" };
cmd search_auth_dev = { 1, 2, "AT+FSAD?", "OK" };
cmd get_auth_dev_count = { 0, 2, "AT+ADCN?", "+ADCN:" };
cmd mru_auth_dev = { 0, 2, "AT+MRAD?", "+MRAD:" };

cmd get_module_working_state = { 0, 2, "AT+STATE?", "+STATE:" };
cmd init_spp_profile_lib = { 0, 1, "AT+INIT", "OK" };

// cmd inquiry_btdev = { 0, } <- this one is weird. not clear how many responses to expect
cmd cancel_inquiry_btdev = { 0, 1, "AT+INQC", "OK" };

cmd equipment_matching = { 2, 1, "AT+PAIR=", "OK" };

cmd connect_device = { 1, 1, "AT+LINK=", "OK" };
cmd disconnect = { 1, 2, "AT+DISC=", "+DISK:" };
