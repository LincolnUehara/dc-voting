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

static void light_0led(void){
    system("killall gpioset");
}

static void light_1led(void){
    system("killall gpioset");
    system("gpioset --mode=signal --background gpiochip0 5=1");
}

static void light_2led(void){
    system("killall gpioset");
    system("gpioset --mode=signal --background gpiochip0 5=1");
    system("gpioset --mode=signal --background gpiochip0 6=1");
}

static void light_3led(void){
    system("killall gpioset");
    system("gpioset --mode=signal --background gpiochip0 5=1");
    system("gpioset --mode=signal --background gpiochip0 6=1");
    system("gpioset --mode=signal --background gpiochip0 13=1");
}

static void light_4led(void){
    system("killall gpioset");
    system("gpioset --mode=signal --background gpiochip0 5=1");
    system("gpioset --mode=signal --background gpiochip0 6=1");
    system("gpioset --mode=signal --background gpiochip0 13=1");
    system("gpioset --mode=signal --background gpiochip0 19=1");
}

static void light_5led(void){
    system("killall gpioset");
    system("gpioset --mode=signal --background gpiochip0 5=1");
    system("gpioset --mode=signal --background gpiochip0 6=1");
    system("gpioset --mode=signal --background gpiochip0 13=1");
    system("gpioset --mode=signal --background gpiochip0 19=1");
    system("gpioset --mode=signal --background gpiochip0 26=1");
}

int main(int argc, char** argv){ 
    
    sqlite3 * db; 
    int8_t result = 0;
    uint16_t mac_count = 0;
    float vote_value = 0;

    char query_count[] = "SELECT COUNT(*) FROM eletrica_votos;";
    char query_vote[50]; 
    char query_vote_id[10];

    result = sqlite3_open("/home/pi/bandas.db", &db); 
	      
    if (result != SQLITE_OK){
        printf("ERROR opening SQLite DB: %s\n", sqlite3_errmsg(db)); 
        return (-1);
    }else{
        printf("Opened Database Successfully!\n");
    }

    while(1){
	
	actual_state = IS_COUNTING_VOTES;

        result = sqlite3_exec(db, query_count, callback, NULL, NULL);
    
        if (result != SQLITE_OK) {
            printf("Not possible to execute count query!\n");
        }else{
            printf("Count query OK!\n");
        }

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

	printf("Media dos votos: %f\n", vote_value);

	if(vote_value > 0 && vote_value <= 1){
            light_1led();
	}else if(vote_value > 1 && vote_value <= 2){
            light_2led();
	}else if(vote_value > 2 && vote_value <= 3){
            light_3led();
	}else if(vote_value > 3 && vote_value <= 4){
            light_4led();
	}else if(vote_value > 4 && vote_value <= 5){
            light_5led();
	}else{
            light_0led();
	}

	sleep(2);
    }

    sqlite3_close(db); 
    return (0); 
} 

