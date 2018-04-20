#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER 1024
#define MAX_ARGS 64
#define MAX_SIZE 2160
#define SEPARATORS " \t\n"

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

int nextFitIndex = 0;
/*Memory blocks*/
struct memory_blocks{
	char* name;
	int start;
	int length;
	int end;
};
typedef struct memory_blocks block;

/*Main memory thats contains memory blocks*/
struct memory_main{
	block** blocks;
	int size;
	int total;
};
typedef struct memory_main memory;

/*Let the pointer point to next loction*/
void insert(memory* mem, int i){
	for(int j = mem->size; j > i; j--){
		mem->blocks[j] = mem->blocks[j - 1];
	}

	(mem->size)++;
}

void removeB(memory* mem, int k){
	free(mem->blocks[k]);
	for(int i = k; i < mem->size - 1; i++){
		mem->blocks[i] = mem->blocks[i + 1];
	}
	
	(mem->size)--;
}

void bestFit(memory* mem, char* name, int requires){
	int nextStart;				//start address of block
	int sucess = FALSE;			//check if sucess allocate memory
	int smallest = 2147483647;		//to recorde smallest rest memory
	int index = -1;				

	for(int i = 0; i < mem->size; i++){
		if(!(mem->blocks[i]->name)){
			if(mem->blocks[i]->length >= requires){
				int rest = mem->blocks[i]->length - requires;
				if(smallest > rest){
					index = i;
					smallest = rest;
				}
			}
		}
	}
	if(index >= 0){
		mem->blocks[index]->name = (char *)malloc(sizeof(char));
		strcpy(mem->blocks[index]->name, name);
		mem->blocks[index]->length = requires;

		if(smallest > 0){
			insert(mem, index + 1);
			mem->blocks[index + 1] = (block *)malloc(sizeof(block));
			mem->blocks[index + 1]->end = mem->blocks[index]->end;
			mem->blocks[index]->end = mem->blocks[index]->end - smallest;
			nextStart = mem->blocks[index]->end + 1;
			mem->blocks[index + 1]->start = nextStart;
			mem->blocks[index + 1]->length = smallest;
			mem->blocks[index + 1]->name = NULL;
		}
		sucess = TRUE;
	}
	if(!sucess){
		printf("Process %s cannot be allacated, fulled \n", name);
	}
}

void buddy(memory* mem, char* name, int requires){
	int nextStart;
	int sucess = FALSE;
	
	for(int i = 0; i < mem->size; i++){
		if(!(mem->blocks[i]->name)){
			if(mem->blocks[i]->length >= requires){
				if((mem->blocks[i]->length)/2 >= requires){
					insert(mem, i + 1);
					mem->blocks[i + 1] = (block *)malloc(sizeof(block));
					mem->blocks[i + 1]->length = (mem->blocks[i]->length)/2;
					mem->blocks[i + 1]->start = mem->blocks[i]->start + mem->blocks[i + 1]->length;
					mem->blocks[i + 1]->end = mem->blocks[i]->end;
					mem->blocks[i + 1]->name = NULL;
					mem->blocks[i]->length = mem->blocks[i + 1]->length;
					mem->blocks[i]->end = mem->blocks[i]->start + mem->blocks[i]->length - 1;
					i--;
				}
				else{
					int rest = mem->blocks[i]->length - requires;
					mem->blocks[i]->name = (char *)malloc(sizeof(char));
					strcpy(mem->blocks[i]->name, name);
					sucess = TRUE;
					break;
				}
			}
		}
	}
	if(!sucess){
		printf("Process %s cannot be allacated, fulled \n", name);
	}
}

void firstFit(memory* mem, char* name, int requires){
	int nextStart;
	int sucess = FALSE;
	
	for(int i = 0; i < mem->size; i++){
	
		if(!(mem->blocks[i]->name)){
			if(mem->blocks[i]->length >= requires){		
				int rest = mem->blocks[i]->length - requires;
				mem->blocks[i]->name = (char *)malloc(sizeof(char));
				strcpy(mem->blocks[i]->name, name);
				mem->blocks[i]->length = requires;
				
				if(rest > 0){
					insert(mem, i + 1);
					mem->blocks[i + 1] = (block *)malloc(sizeof(block));					
					mem->blocks[i + 1]->end = mem->blocks[i]->end;
					mem->blocks[i]->end = mem->blocks[i]->end - rest;
					nextStart = mem->blocks[i]->end + 1;
					mem->blocks[i + 1]->start = nextStart;
					mem->blocks[i + 1]->length = rest;
					mem->blocks[i + 1]->name = NULL;
				}
				sucess = TRUE;
			
				break;
			}
			
		}
	}
	if(!sucess){
		printf("Process %s cannot be allacated, fulled \n", name);
	}
}

void nextFit(memory* mem, char* name, int requires){
	int nextStart;
	int sucess = FALSE;
	
	for(int i = nextFitIndex; i < mem->size; i++){
		if(!(mem->blocks[i]->name)){
			if(mem->blocks[i]->length >= requires){
				int rest = mem->blocks[i]->length - requires;
				mem->blocks[i]->name = (char *)malloc(sizeof(char));
				strcpy(mem->blocks[i]->name, name);
				mem->blocks[i]->length = requires;

				if(rest > 0){
					insert(mem, i + 1);
					mem->blocks[i + 1] = (block *)malloc(sizeof(block));
					mem->blocks[i + 1]->end = mem->blocks[i]->end;
					mem->blocks[i]->end = mem->blocks[i]->end - rest;
					nextStart = mem->blocks[i]->end + 1;
					mem->blocks[i + 1]->start = nextStart;
					mem->blocks[i + 1]->length = rest;
					mem->blocks[i + 1]->name = NULL;
				}
				sucess = TRUE;
				nextFitIndex = i;
				break;
			}
		}
	}
	if(!sucess){
		printf("Process %s cannot be allacated, fulled \n", name);
	}
}

void initiateMem(memory* mem, int n){
	int mount = 1;

	for(int i = 0; i < n; i++){                
		mount = mount * 2;
	}
	mem->blocks[0] = (block *)malloc(sizeof(block));
	mem->size = 1;
	mem->total = mount;
	mem->blocks[0]->length = mount;
	mem->blocks[0]->start = 0;
	mem->blocks[0]->end = mount - 1;
	mem->blocks[0]->name = NULL;
}

int main(int argc, char** argv){
	int alg;
	char buf[MAX_BUFFER];
	char* args[MAX_ARGS];
	char** arg;
	FILE* istream;
	int ifAva = FALSE;
	int ifSig = FALSE;
	int ifFound = FALSE;
	int mount;

	memory mem;
	mem.blocks = (block **)malloc(MAX_SIZE * sizeof(block *));

	int n = atoi(argv[2]);
	initiateMem(&mem, n);
	
	
	if(!strcmp(argv[1], "BESTFIT")){
		alg = 0;
	}
	else if(!strcmp(argv[1], "FIRSTFIT"))
		alg = 1;
	else if(!strcmp(argv[1], "NEXTFIT"))
		alg = 2;
	else if(!strcmp(argv[1], "BUDDY"))
		alg = 3;
	else 
		alg = -1;
	if(alg == -1){
		printf("There is not %s algorithm", argv[1]);
		exit(0);
	}
	
	
	if(access(argv[3], F_OK)){
		perror("Read command from not exit file");
		exit(0);
	}
	else if(!(istream = freopen(argv[3], "r", stdin))){
		perror("Read command from not exit file");
		exit(0);
	}
	
	while(!feof(stdin)){
		if(fgets(buf, MAX_BUFFER, stdin)){
			arg = args;
			*arg++ = strtok(buf, SEPARATORS);
			while((*arg++ = strtok(NULL, SEPARATORS)));
			if(args[0]){
				if(!strcmp(args[0], "#")){
					arg = args;
					while(*(++arg))
						printf("%s ", *arg);	
					printf("\n");
				}
				else if(!strcmp(args[0], "REQUEST")){
			
					mount = atoi(args[2]);
					switch(alg){
						case 0:
							bestFit(&mem, args[1], mount);
							break;
						case 1:
							firstFit(&mem, args[1], mount);
							break;
						case 2:
							nextFit(&mem, args[1], mount);
							break;
						case 3:
							buddy(&mem, args[1], mount);
						default:
							break;
					}
				}

				
				else if(!strcmp(args[0], "RELEASE")){
					for(int i = 0; i < mem.size; i++){
						if(mem.blocks[i]->name){	
							if(!strcmp(args[1], mem.blocks[i]->name)){
								printf("FREE %s %i %i\n", mem.blocks[i]->name, mem.blocks[i]->length, mem.blocks[i]->start);
								free(mem.blocks[i]->name);
								mem.blocks[i]->name = NULL;
								ifFound = TRUE;
								
								//Make the memory blocks size dynamic
								if(alg != 3){
									if((i > 0) && !(mem.blocks[i - 1]->name)){
										mem.blocks[i - 1]->length = mem.blocks[i - 1]->length + mem.blocks[i]->length;
										mem.blocks[i - 1]->end = mem.blocks[i]->end;
										removeB(&mem, i);
									}
									if((i < mem.size - 1) && !(mem.blocks[i + 1]->name)){
										mem.blocks[i]->length = mem.blocks[i]->length + mem.blocks[i + 1]->length;
										mem.blocks[i]->end = mem.blocks[i + 1]->end;
										removeB(&mem, i + 1);
									}
								}
								else{//BUDDY SYSTEM, blocks only combine with buddies
									if((i > 0) && !(mem.blocks[i - 1]->name) && mem.blocks[i - 1]->length == mem.blocks[i]->length){
										mem.blocks[i - 1]->length = mem.blocks[i - 1]->length + mem.blocks[i]->length;
										mem.blocks[i - 1]->end = mem.blocks[i]->end;
										removeB(&mem, i);
									}
									if((i < mem.size - 1) && !(mem.blocks[i + 1]->name) && mem.blocks[i + 1]->length == mem.blocks[i]->length){
										mem.blocks[i]->length = mem.blocks[i]->length + mem.blocks[i + 1]->length;
										mem.blocks[i]->end = mem.blocks[i + 1]->end;
										removeB(&mem, i + 1);
									}
								}
							}
						}
					}
					if(!ifFound)
						printf("FAIL RELEASE %s\n", args[1]);
					ifFound = FALSE;
				}
				
				else if(!strcmp(args[0], "LIST")){
					if(!strcmp(args[1], "AVAILABLE")){
						printf("LIST AVAILABLE: \n");
						for(int i = 0; i < mem.size; i++){
							if(!mem.blocks[i]->name){
								printf("(%i, %i) ", mem.blocks[i]->length, mem.blocks[i]->start);	
								ifAva = TRUE;
							}
						}
						printf("\n");

						if(!ifAva)
							printf("FULL\n");
						ifAva = FALSE;
					}
					else if(!strcmp(args[1], "ASSIGNED")){
						printf("LIST ASSIGNED: \n");
						for(int i = 0; i < mem.size; i++){
							if(mem.blocks[i]->name){
								printf("(%s, %i, %i) ", mem.blocks[i]->name, mem.blocks[i]->length, mem.blocks[i]->start);
								ifSig = TRUE;
							}
						}
					
						if(!ifSig)
							printf("NONE\n");
						else
							printf("\n");
						ifSig = FALSE;
					}
				}
				
				else if(!strcmp(args[0], "FIND")){
					for(int i = 0; i < mem.size; i++){
						if(mem.blocks[i]->name){
							if(!strcmp(args[1], mem.blocks[i]->name)){
								printf("(%s, %i, %i)\n", mem.blocks[i]->name, mem.blocks[i]->length, mem.blocks[i]->start);
				        			ifFound = TRUE; 
							}
						}
					}
					if(!ifFound)
						printf("There is not process %s in Memory.\n", args[1]);
					ifFound = FALSE;
				}				
			}	
		}
	}
	fclose(istream);
	return 0;
}
