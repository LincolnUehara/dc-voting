#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <sqlite3.h>
#include <time.h>

#define IS_COUNTING_VOTES 1
#define IS_SORTING_MAC 2

static uint16_t num_votes = 0;
static char mac_val[20];
static uint8_t actual_state = 0;

static int callback(void* data, int argc, char** argv, char** azColName){ 
    
    switch(actual_state){
        case IS_COUNTING_VOTES :
            num_votes = atoi(argv[0]);
	    break;
        case IS_SORTING_MAC :
            memset(mac_val, '\0', 20);
            strcpy(mac_val, argv[0]);
	    break;
	default:
	    break;
    }
    
    return 0; 
}

int main(int argc, char** argv){ 
    
    sqlite3 * db; 
    int8_t result = 0;
    uint16_t mac_db_id = 0;

    char query_count[] = "SELECT COUNT(*) FROM eletrica_votos;";
    char query_mac[50]; 
    char query_mac_id[10];

    result = sqlite3_open("/home/pi/bandas.db", &db); 
	      
    if (result != SQLITE_OK){
        printf("ERROR opening SQLite DB: %s\n", sqlite3_errmsg(db)); 
        return (-1);
    }
	
    actual_state = IS_COUNTING_VOTES;

    result = sqlite3_exec(db, query_count, callback, NULL, NULL);
    
    if (result != SQLITE_OK) {
        printf("Not possible to execute count query!\n");
    }

    actual_state = IS_SORTING_MAC;
    
    if(num_votes == 0){
        printf("Ainda não houveram votos!\n");
        return (0);
    }

    srand(time(0));
    mac_db_id = (uint16_t)(rand() % (num_votes) + 1);

    memset(query_mac, '\0', 50);
    memset(query_mac_id, '\0', 10);
    strcpy(query_mac, "SELECT mac FROM eletrica_votos WHERE id=");
    sprintf(query_mac_id, "%d;", (mac_db_id));
    strcat(query_mac, query_mac_id);
    sqlite3_exec(db, query_mac, callback, NULL, NULL);
    
    printf("MAC sorteado: %s\n", mac_val);

    sqlite3_close(db); 
    return (0); 
}
