#include <stdio.h>
#include "hashmap.h"
#include "hex.h"
#include "utils.h"

//on initialise le dictionnaire
hashmap init_dict(){

    //printf("Initialisation en cours...\n\n");
    //on alloue le hashmap
    hashmap out_hashmap = (hashmap_s*)malloc(sizeof(hashmap_s));

    //appelle de la fonction hashmap pour créer 
    hashmap_create(HASHMAP_SIZE,out_hashmap);

    int j;

    for (int i = 0; i < CHAR_MAX; i++){

        j = i;

        //on convertie la valeur i en character ascii
        char chara = i;
        //on le convertie en string
        char* string = char2str(chara);
        //on place l'adresse de i dans value

        //on calcule la valeur de la clée avec l'adresse i + j
        //si on avait que mis l'adresse i on aurait la même
        //pour chaque clée
        void* value = &i+j;

        //appelle de la fonction hasmap_put pour insérer un
        //élément dans le hashmap
        //printf("Ajout de l'élement %s comme clee avec comme value %p\n", string, value);
        hashmap_put(out_hashmap, string, sizeof(string), value);
        //printf("Fin de l'ajout\n");
    }

    //printf("Initialisation terminé !\n\n");
    return out_hashmap;

}
//on free le dictionnaire
void free_dictionnaire(hashmap dict){

    hashmap_destroy(dict);

}

//ajouter des éléments dans un dictionnaire
void add_to_dict(hashmap dict, const char* clee, void* const value){

    //printf("Ajout de l'élement %s avec comme value %p\n", clee, value);
    hashmap_put(dict, clee, sizeof(clee), value);
    //printf("Fin de l'ajout\n");

}

//test pour vérifier si un élément est dans le dictionnaire en fonction de sa clée
int cherche_caractere(hashmap dict, const char* clee){

    //printf("On cherche la clée %s\n", clee);
    if (hashmap_get(dict, clee, sizeof(clee)) != HASHMAP_NULL){

        //printf("On a trouvé ! \n");
        return 0;
    }
    //printf("On a pas trouvé !\n");
    return -1;

}

void compression_hashmap(FILE* input_stream, FILE* output_stream){

    //creation des buffer qui vont contenir les caractères du
    //fichier
    char buffer1[2];
    char buffer2[2];

    //premier caractère
    char* p = fgets(buffer1, MAX_LENGTH, input_stream);

    //printf("1er caractère : %s\n", p);
    
    //caractère suivant
    char* c;

    //test
    int test;

    int i = 256;
    int j = i;

    //valeur caractère initial
    void* value_p = &i+j;

    //on initialise le dictionnaire
    hashmap dictionnaire = init_dict();

    while (!feof(input_stream) && ((c = fgets(buffer2, MAX_LENGTH, input_stream)) != NULL)){

        if (c == "\0"){
            continue;
        }
        
        //printf("caractère suivant : %s\n", c);
        //printf("caractère precedent : %s\n", p);
        //si p et c sont dans le dictionnaire
        char* pc = concat(p, c);
        //printf("on cherche si %s est dans le dico\n", pc);
        if ((test = cherche_caractere(dictionnaire, pc)) != -1){
            
            //alors p = p+c;
            p = pc;
        }

        else{

            //printf("avant écriture output_code !\n");
            wb_hex_as_short(output_stream, value_p);
            //printf("après écriture output_code !\n");

            i++;
            j++;

            value_p = &i+j;
            add_to_dict(dictionnaire, pc, value_p);

            *p = *c;

        }
    }
    wb_hex_as_short(output_stream, value_p);

    free_dictionnaire(dictionnaire);

}


