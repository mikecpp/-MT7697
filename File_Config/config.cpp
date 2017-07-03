#include "config.h"

#define MAX_STR_LEN     64
#define CONFIG_FILE     "config.txt"

File m_file;

static int32_t _config_gets(char *str)
{
    char ch;
    int i = 0;
    
    while(m_file.available()) {
        ch = m_file.read();
        switch(ch) {
            case '\r':
                 break;
                
            case '\n':
                 str[i] = '\0';
                 return i;
                
            default:
                if(i >= MAX_STR_LEN) {
                    str[i-1] = '\0';
                    return i;    
                }
                str[i++] = ch;
        }
    }

    return i;
}

int32_t config_init(void)
{
    if(!SD.begin()) { // SD initial fail
        return -1;
    }

    m_file = SD.open(CONFIG_FILE);
    if(m_file == false) { // File open fail
        return -2;
    }    

    return 0;
}

int32_t config_read(const char* name, char* value, int* len)
{
    char str[MAX_STR_LEN];
    char *ptr = NULL;
    
    m_file.seek(0);

    while(1) {
        if(_config_gets((char*) str) <= 0) {
            break;
        }
        ptr = strtok(str, "=");
        if(ptr == NULL) {
            return -1;
        }
        if(strcmp(ptr, name) == 0) {
            ptr = strtok(NULL, "\0");
            if(ptr == NULL) {
                return -1;
            }
            strcpy(value, ptr);
            *len = strlen(ptr);
            return *len;
        }
    }

    return 0;
}

int32_t config_write(const char* name, char* value, int len)
{
    return 0;
}

int32_t config_dump(void)
{
    char str[MAX_STR_LEN];
    char msg[MAX_STR_LEN];
    
    while(1) {
        if(_config_gets((char*) str) <= 0) {
            break;
        }
        sprintf(msg, "%s", (char*) str);
        Serial.println(msg);
    }
}

