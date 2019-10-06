#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <sqlite3.h> 

#define IS_COUNTING_VOTES 1
#define IS_COLLECTING_VOTES 2

static uint16_t num_votes = 0;
static uint16_t val_votes = 0;
static uint8_t actual_state = 0;

static int callback(void* data, int argc, char** argv, char** azColName){ 
    
    switch(actual_state){
        case IS_COUNTING_VOTES :
            num_votes = atoi(argv[0]);
	    break;
        case IS_COLLECTING_VOTES :
            val_votes = atoi(argv[0]);
	    break;
	default:
	    break;
    }
    
    return 0; 
}

int main(int argc, char** argv){ 
    
    sqlite3 * db; 
    int8_t result = 0;
    uint16_t mac_count = 0;
    float vote_value = 0;

    char query_count_logs[] = "SELECT COUNT(*) FROM eletrica_logs;";
    char query_count_votes[] = "SELECT COUNT(*) FROM eletrica_votos;";
    char query_vote[50]; 
    char query_vote_id[10];

    result = sqlite3_open("/home/pi/bandas.db", &db); 
	      
    if (result != SQLITE_OK){
        printf("ERROR opening SQLite DB: %s\n", sqlite3_errmsg(db)); 
        return (-1);
    }

    actual_state = IS_COUNTING_VOTES;

    result = sqlite3_exec(db, query_count_logs, callback, NULL, NULL);
    
    if (result != SQLITE_OK) {
        printf("Not possible to execute count query!\n");
    }

    printf("Houveram %d votos, ", num_votes);

    result = sqlite3_exec(db, query_count_votes, callback, NULL, NULL);
    
    if (result != SQLITE_OK) {
        printf("Not possible to execute count query!\n");
    }

    printf("sendo %d válidas.\n", num_votes);

    actual_state = IS_COLLECTING_VOTES;
    vote_value = 0;

    for(mac_count = 0; mac_count < num_votes; mac_count++){
        memset(query_vote, '\0', 50);
        memset(query_vote_id, '\0', 10);
        strcpy(query_vote, "SELECT voto FROM eletrica_votos WHERE id=");
        sprintf(query_vote_id, "%d;", (mac_count + 1));
        strcat(query_vote, query_vote_id);
        sqlite3_exec(db, query_vote, callback, NULL, NULL);
        vote_value += val_votes;
    }

    if(vote_value != 0){
        vote_value /= num_votes;
    }

    printf("Média dos votos: %f\n", vote_value);

    sqlite3_close(db); 
    return (0); 
}
