#include "lab4.h"
#include "lab4_part2.c"

long int get_num_lines(char* fn){
    FILE* fp = fopen(fn, "r");
    long int num_lines = 0;
    char ch;
    for (ch = getc(fp); ch != EOF; ch = getc(fp)){
        if (ch == '\n')
            ++num_lines;
    }
    fclose(fp);
    return num_lines;
}


PersonalData** parse_data(char* fn){
    
    long int num_lines = get_num_lines(fn);
    FILE* fp = fopen(fn, "r");
    
    INT_SIN SIN;
	char gender; 
	char first_name[MAX_NAME_LEN]; 
	char last_name[MAX_NAME_LEN]; 
	char passport_num[PASSPORT_LEN]; 
	char bank_acc_num[BANK_ACC_LEN]; 
	int dob_year;
	int dob_month;
	int dob_day;
    char headers[1000];
    fgets(headers, 1000, fp);

    PersonalData** arr_data = calloc(num_lines,sizeof(PersonalData*));
    
    //printf("%s",headers);

    int i = 0;
    while(fscanf(fp,"%d\t %c\t %s\t %s\t %s\t %s\t %d-%d-%d\n",&SIN,&gender,first_name,last_name,passport_num,bank_acc_num,&dob_year,&dob_month,&dob_day) != EOF){
        PersonalData* data = malloc(sizeof(PersonalData));

        data->SIN = SIN;
        data->gender = gender;
        strcpy(data->first_name, first_name);
        strcpy(data->last_name, last_name);
        strcpy(data->passport_num, passport_num);
        strcpy(data->bank_acc_num, bank_acc_num);
        data->dob_year = dob_year;
        data->dob_month = dob_month;
        data->dob_day = dob_day;
        //printf("data %d : %d\n",i,data->SIN);

        arr_data[i] = data;
        i++;
    }
    fclose(fp);
    return arr_data;
}


void counter_intelligence(char* load, char* update, char* validate, char* outfp){
    FILE* fp = fopen(outfp,"w");
    PersonalData** data_load = parse_data(load);
    PersonalData** data_update = parse_data(update);
    PersonalData** data_validate = parse_data(validate);
    HashTable *table = create_hash_table((int)(log(get_num_lines(load))/log(2)),CLOSED_ADDRESSING);
    print_status(table);
    

    for(int i=0; i < get_num_lines(load); i++){
        //print_personal_data(data_load[i]);
        //printf("LOADING \nsize of data_load: %d\n",get_num_lines(load));
        update_key(data_load[i],&table);
        print_status(table);
        //print_buckets(table);
    }
    free(data_load);
    
    
    //print_status(table);
    for(int i=0; i < get_num_lines(update); i++){
        //print_personal_data(data_update[i]);
        //printf("UPDATING \nsize of data_load: %d\n",get_num_lines(update));
        update_key(data_update[i],&table);
        print_status(table);
    }
    free(data_update);
    //print_buckets(table);
    //print_status(table);

    for(int i=0; i < get_num_lines(validate); i++){

        char str[1000] = {0};
        //printf("new iteration\n");
        snprintf(str,999,"%d\n",data_validate[i]->SIN);
        //printf("%s",str);
        PersonalData *table_data = lookup_key(data_validate[i]->SIN,table);
        //print_personal_data(table_data);
        //print_personal_data(data_validate[i]);
        if(!table_data)
            fputs(str, fp);
        else if((table_data)->gender != data_validate[i]->gender)
            fputs(str, fp);
        else if(strcmp((table_data)->first_name, data_validate[i]->first_name) != 0)
            fputs(str, fp);
        else if(strcmp((table_data)->last_name, data_validate[i]->last_name) != 0)
            fputs(str, fp);
        else if(strcmp((table_data)->passport_num, data_validate[i]->passport_num) != 0)
            fputs(str, fp);
        else if(strcmp((table_data)->bank_acc_num, data_validate[i]->bank_acc_num) != 0)
            fputs(str, fp);
        else if((table_data)->dob_year != data_validate[i]->dob_year)
            fputs(str, fp);
        else if((table_data)->dob_month != data_validate[i]->dob_month)
            fputs(str, fp);
        else if((table_data)->dob_day != data_validate[i]->dob_day)
            fputs(str, fp);
    }
    free(data_validate);
    delete_table(table);
    fclose(fp);
}
