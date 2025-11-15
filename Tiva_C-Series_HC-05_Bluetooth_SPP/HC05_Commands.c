#include <stdarg.h>
#include "HC05_Commands.h"

extern void output_string ( char * );
extern char *strcat ( char *, char * );
extern void clear_cmd_buffer ( void );

void
send_command ( cmd command, ... )
{
    if (command.num_param == 0)
    {
        output_string (command.arg);
        return;
    }

    va_list args;
    va_start (args, command);
    char *cmd_buffer;

    switch (command.num_param)
    {
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
    output_string (cmd_buffer);
    clear_cmd_buffer ();
}

cmd test = { 0, 1, "AT", "OK" };
cmd reset = { 0, 1, "AT+RESET", "OK" };
cmd firmware_version = { 0, 2, "AT+VERSION?", "+VERSION:" };
cmd restore_default = { 0, 1, "AT+ORGL", "OK" };
cmd get_module_addr = { 0, 2, "AT+ADDR?", "+ADDR:" };
cmd set_module_name = { 1, 1, "AT+NAME=", "OK" };
cmd get_module_name = { 0, 2, "AT+NAME?", "+NAME:" };

