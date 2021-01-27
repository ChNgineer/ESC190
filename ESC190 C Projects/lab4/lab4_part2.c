#include "lab4.h"
#include <math.h>
#include "lab4_part1.c"
#define NUM_HASH_FUNCS 3

HashTable *create_hash_table(int m, int mode){
/**
    create a new hash table of size 2^m and return the pointer to the newly created table.
    Fully initialize the state of the table.

    Sample IO
    HashTable* table0 = create_hash_table(1, CLOSED_ADDRESSING);
    print_status(table0);
	
	HashTable* table1 = create_hash_table(1, LINEAR_PROBING); 
	print_status(table1); 
	
	HashTable* table2 = create_hash_table(1, CUCKOO); 
	print_status(table2); 

    Stdout:
    Table capacity:    		2 
    Table load:        		0 
    Table load factor: 		0.00
	Mode:			   		closed
	
	Table capacity:         2 
	Table load:             0 
	Table load factor:      0.00 
	Mode:                   linear

	Table capacity:         2 
	Table load:        		0 
	Table load factor:      0.00 
	Mode:                   cuckoo 	

**/
    hash_funcs[0] = trivial_hash;
    hash_funcs[1] = pearson_hash;
    hash_funcs[2] = fibonacci_hash;
	
	HashTable *table = malloc(sizeof(HashTable));
	table->mode = mode;
	table->num_keys = 0;
	table->num_buckets = pow(2,m);
	Node **node_arr = calloc(table->num_buckets, sizeof(Node));
	table->buckets = node_arr;
	return table;
}

void update_without_resize(PersonalData * data, HashTable *table) {
/**
	Insert/update the data corresponding to SIN of data.
	Update all book-keeping information.
**/
	INT_HASH hash_num = hash_funcs[table->mode](data->SIN,table->num_buckets);
	Node *node = malloc(sizeof(Node));
	node->value = data;
	node->next = NULL;
	//print_personal_data(node->value);
	Node **curr_node = table->buckets + hash_num*sizeof(char);
	
	if(*curr_node == NULL){
		//printf("empty hash\n");
		++table->num_keys;
		*curr_node = node;
		//print_personal_data((*curr_node)->value);
		//print_buckets(table);
	}else if(*curr_node != NULL && table->mode == 0){
		//printf("chaining\n");
		Node *temp = *curr_node;
		while(temp->next != NULL)
			temp = temp->next;
		temp->next = node;
	}else if(*curr_node != NULL && table->mode == 1){
		//printf("linear probing\n");
		for(INT_HASH i=1; i < table->num_buckets; i ++){
			if(*(table->buckets + (hash_num + i) % table->num_buckets) == NULL){
				*(table->buckets + (hash_num + i) % table->num_buckets) = node;
				++table->num_keys;
				return;
			}
		}
	}else if(*curr_node != NULL && table->mode == 2){
		//printf("quadratic probing\n");
		for(int i=1; pow(i,2) < table->num_buckets; i++){
			if(*(table->buckets + (hash_num + (INT_HASH)pow(i,2)) % table->num_buckets) == NULL){
				*(table->buckets + (hash_num + (INT_HASH)pow(i,2)) % table->num_buckets) = node;
				++table->num_keys;
				return;
			}
		}
	}
}

void update_key(PersonalData *data, HashTable **table){
/** 
	Using update_without_resize and resize_table,
	update the hash table while ensuring the final table's 
	load factor does not exceed MAX_LOAD_FACTOR.
	
	Note that this function updates the pointer to the table with the
	return of resize_table and deletes the original table.
	
	Sample IO
	HashTable * table1 = create_hash_table(1, CLOSED_ADDRESSING);
	PersonalData data_a = {0, 'F', "Alice","Smith","XXX", "YYY", 1995, 12, 12};
	PersonalData data_b = {1, 'M', "Bob","Xu","XXX", "YYY", 1994, 12, 12};
	PersonalData data_c = {10, 'M', "Jeremy","Voyager","XXX", "YYY", 1994, 19, 12};
	update_key(&data_a, table1);
	update_key(&data_b, table1);
	update_key(&data_c, table1);
	print_buckets(table1);
	
	PersonalData data1 = {2, 'F', "Alice", "Li", "XXX", "YYY", 1986, 1, 1}; 
	PersonalData data2 = {7, 'M', "Bob", "Kim", "XXX", "YYY", 1999, 5, 12}; 
	PersonalData data3 = {10, 'F', "Eve", "Pooh", "XXX", "YYY", 1993, 4, 20}; 
	HashTable* table2 = create_hash_table(3, LINEAR_PROBING); 
	update_key(&data1, table2); 
	update_key(&data2, table2); 
	update_key(&data3, table2); 
	print_buckets(table2);

	Stdout:
	Bucket 0:    SIN: 000000000     
	Bucket 1:    SIN: 000000001
	Bucket 2:    SIN: 000000010     
	Bucket 3:     
	Bucket 4:     
	Bucket 5:     
	Bucket 6:     
	Bucket 7:   
	
	Bucket   0:     
	Bucket   1:     SIN:    2 
	Bucket   2:     SIN:    7 
	Bucket   3:     
	Bucket   4:      
	Bucket   5:     SIN:    10 
	Bucket   6:     
	Bucket   7:     
**/
	INT_HASH hash_num = hash_funcs[(*table)->mode](data->SIN,(*table)->num_buckets);
	//print_personal_data(data);
	int replace = 0;
	if(lookup_key(data->SIN,*table)){
		//printf("duplicate detected, deleting: %d\n",data->SIN);
		delete_key(data->SIN,*table);
		//print_status(*table);
		replace = 1;
	}
	while(((float)(((*table)->num_keys) + 1)/(*table)->num_buckets) > MAX_LOAD_FACTOR && replace == 0){
		//printf("resize table\n");
		//printf("next load factor: %f\n",(float)(((*table)->num_keys) + 1)/(*table)->num_buckets);
		//printf("current num_buckets: %d\n",(*table)->num_buckets);
		*table = resize_table(*table);
		//printf("new num_buckets: %d\n",(*table)->num_buckets);
	}
	update_without_resize(data,*table);
	//print_buckets(*table);
}

int delete_key(INT_SIN SIN, HashTable *table){
/**
	Delete key value, return 1 if successful,
	0 if key not in table - update book-keeping information.
	Ensure no memory leaks. Do not free memory that you did not allocate.

	Sample IO
	HashTable * table = create_hash_table(1, CLOSED_ADDRESSING);
	PersonalData data_a = {0, 'F', "Alice","Smith","XXX", "YYY", 1995, 12, 12};
	update_key(&data_a, table);
	delete_key(0, table);
	print_status(table);
	print_buckets(table);

	Stdout:
	Table capacity:    2 	
	Table load:        0 
	Table load factor: 0.00 
	Bucket 0: 
	Bucket 1: 
	Bucket 2:       
	Bucket 3:     
	Bucket 4:     
	Bucket 5:     
	Bucket 6:     
	Bucket 7:  
	
**/
	if(*(table->buckets + hash_funcs[table->mode](SIN,table->num_buckets)) != NULL){
		if(table->mode == 0){
			Node *curr_node = *(table->buckets + hash_funcs[table->mode](SIN,table->num_buckets));
			Node *temp = NULL;
			while(curr_node != NULL){
				if(curr_node->value->SIN == SIN){
					if(temp != NULL)
						temp->next = curr_node->next;
					else{
						*(table->buckets + hash_funcs[table->mode](SIN,table->num_buckets)) = curr_node->next;
						if(curr_node->next == NULL)
							--table->num_keys;
					}
					free(curr_node);
					return 1;
				}
				temp = curr_node;
				curr_node = curr_node->next;
			}
		}else{
			for (INT_HASH b = 0; b < table->num_buckets; b++){ 
				Node * bucket = table->buckets[b];
				while (bucket != NULL){
					if (bucket->value->SIN == SIN){ 
						free(bucket);
						return 1;
					}
				bucket = bucket->next;
				}
			}
		}
	}
	return 0;
}

PersonalData* lookup_key(INT_SIN SIN, HashTable *table){
/** 
	return pointer to the PersonalData struct associated with SIN.
	return NULL if the SIN is not in table.
	
	Sample IO
	HashTable * table = create_hash_table(1, CLOSED_ADDRESSING);
	PersonalData data = {8, 'F', "Alice","Xu", "AB12345", "99999-999-999999999999", 1960, 31, 12};
	update_key(&data, table);
	print_personal_data(lookup_key(data.SIN, table));

	Stdout:
	SIN:        		8 
	Gender:     		F 
	Name:        		Xu, Alice 
	Passport #:    		AB12345 
	Bank account #:		99999-999-999999999999 
	Date of birth:		12-31-1960 
**/
	for (INT_HASH b = 0; b < table->num_buckets; b++){ 
		Node * bucket = table->buckets[b];
		while (bucket != NULL){
			if (bucket->value->SIN == SIN) 
				return bucket->value;
			bucket = bucket->next;
		}
	}
	return NULL;
}

void delete_table(HashTable *table){
/**
	Delete the table and ensure no memory leaks. Do NOT free memory that you 
	did not allocate.

	Sample IO
	HashTable * table = create_hash_table(1, CLOSED_ADDRESSING);
	delete_table(table);
**/
	Node *temp;
	if((table->buckets) != NULL && table->mode != 0){
		for(int i=0; i < table->num_buckets-1; ++i){
			if(*(table->buckets + i) != NULL)
				free(*(table->buckets + i));
		}
	}
	if((table->buckets) != NULL && table->mode == 0){
		for(int i=0; i < table->num_buckets-1; ++i){
			if(*(table->buckets + i) != NULL){
				if((*(table->buckets + i))->next != NULL){
					temp = *(table->buckets + i);
					*(table->buckets + i) = (*(table->buckets + i))->next;
					free(temp);
				}
				free(*(table->buckets + i));
			}
		}
	}
	free(table->buckets);
	free(table);
}

HashTable *resize_table(HashTable *table){
/**
	Return a pointer to a new table with size m+1, where 
	the original table "table" has size 2^m.
	Delete the original table, ensure no memory leaks. 
	Update all book-keeping information.

	SampleIO
	HashTable * table = create_hash_table(2, CLOSED_ADDRESSING);
	table = resize_table(table);
	print_status(table);

	Stdout:
	Table capacity:    8 
	Table load:        0 
	Table load factor: 0.00 
**/
	//printf("current m : %d\n", (int)(log(table->num_buckets)/log(2)));
	HashTable *new_table = create_hash_table((int)(log(table->num_buckets)/log(2) + 1),table->mode);
	//printf("resize current buckets: %d\n",new_table->num_buckets);
	//printf("new resized m : %d\n", (int)(log(new_table->num_buckets)/log(2) + 1));
	new_table->buckets = calloc(new_table->num_buckets, sizeof(Node));
	for(int i=0; i < table->num_buckets; i++){
		if(table->buckets[i] != NULL){
			update_without_resize(table->buckets[i]->value,new_table);
			while(table->buckets[i]->next != NULL){
				table->buckets[i] = table->buckets[i]->next;
				update_without_resize(table->buckets[i]->value,new_table);
			}
		}
	}
	table->buckets = NULL;
	INT_HASH keys = table->num_keys;
	new_table->num_keys = keys;
	delete_table(table);
	return new_table;
}
