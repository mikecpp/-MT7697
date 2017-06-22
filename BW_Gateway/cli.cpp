#include <stdio.h>
#include <string.h>

#include "cli.h"

#define MAX_CLI_COUNT        (20)
#define MAX_STR_LEN          (64)
#define MAX_PARAM            (10)
#define CMD_LEN              (32)
#define HELP_LEN             (64)
#define CLI_PROMPT           "> "
#define MSG_NOTFOUND         "Error: Command not found ! Use help! \r\n"

#define CMD_EXIT    "exit"

typedef struct {
    cli_func func;
    char cmd[CMD_LEN];
    char help[HELP_LEN];
} cli_cmd_t;

static int cli_gets(char *cmd);
static int cli_parse_str(char *str, char *param[]);
static int cli_help(int argc, char *argv[]);

static cli_cmd_t m_cmd_table[MAX_CLI_COUNT];
static int       m_count = 0;

int cli_gets(char *cmd)
{
    char ch;
    int  i = 0;

    while(1)
    {
        if(Serial.available() == 0) {
            continue;
        }
        
        ch = (char) Serial.read();
        switch (ch) {
            case '\r':
                break;
            
            case '\n':
                cmd[i] = '\0';
                Serial.print("\r\n\r\n");
                return 0;

            case 127:       // Back-Space
                Serial.print(ch);
                if (i > 0) {
                    i--;
                }
                break;
            
            default:
                if(i >= MAX_STR_LEN) {
                    break;
                }
                cmd[i++] = ch;
                Serial.print(ch);
        }
    }
}

int cli_parse_str(char *str, char *param[]) 
{
    int count = 0;
    char *ptr = NULL;
    
    ptr = strtok(str, " ");
    while(ptr != NULL) {
        param[count++] = ptr;
        ptr = strtok(NULL, " ");
        if(count >= MAX_PARAM) {
            break;
        }    
    }
    
    return count;
}

static int cli_help(int argc, char *arg[])  
{
    cli_cmd_t *ptr = m_cmd_table;

    Serial.print("command:\r\n");
    while(ptr->cmd[0]) {
        Serial.print(ptr->cmd);
        Serial.print(":    \t");
        Serial.print(ptr->help);
        Serial.println();
        ptr++;
    }
    
    return 0;
}

//////////////////////////////////////////////////////////////////////////////
// Public API
//

int cli_init(void)
{
    memset(m_cmd_table, 0x00, sizeof(m_cmd_table));
    
    cli_register(cli_help, "help", "list all command"); 
    
    return 0;
}

int cli_register(cli_func func, char *cmd, char *help) 
{
    if(m_count >= MAX_CLI_COUNT) {
        return -1;
    }    
    
    cli_cmd_t *ptr = (cli_cmd_t *) &m_cmd_table[m_count]; 
    
    if((func == NULL) || (strlen(cmd) > CMD_LEN) ||
        (strlen(help) > HELP_LEN)) {
        return -2;
    }        
    
    ptr->func = func;
    strcpy(ptr->cmd, cmd);
    strcpy(ptr->help, help);
    
    m_count++;
    
    return 0;
}

int cli_exec(char *string)
{
    cli_cmd_t *ptr = NULL;
    int count = 0; 
    char *param[MAX_PARAM]; 
    
    ptr   = m_cmd_table;
    count = cli_parse_str(string, param);  
    
    while(ptr->cmd[0]) 
    {
        if(strcmp(param[0], ptr->cmd) == 0)
        {
            ptr->func(count, param);  
            break;
        }
        ptr++;
    }
        
    if(ptr->cmd[0] == 0x00) {
        Serial.print(MSG_NOTFOUND);
    }
    
    return 0;
}

int cli_task(void)
{
    char str[MAX_STR_LEN];

    while(1) 
    {
        Serial.print(CLI_PROMPT);
        cli_gets((char*) &str);
        cli_exec(str);
    }
}
