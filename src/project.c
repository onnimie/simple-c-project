#include "project.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

// TODO:: implement your project here!


int main(void) {

    character_list;
    nof_characters = 0;


    0;
}

int add_character(const char* name, unsigned int hp, const char* weapon_name, unsigned int weapon_damage) {

    if (nof_characters == 0) {
        for (int i = 0; i < nof_characters; i++) {

            Character check_character = character_list[i];
            char* check_name = check_character.name;

            if (strcmp(name, check_name) == 0) {
                return -1;
            }
        }
        character_list = malloc(sizeof(Character));
    } else {
        character_list = realloc(character_list, sizeof(Character) * (nof_characters+1));
    }

    Character* new_character = malloc(sizeof(Character));
    if (!new_character) return -1;

    char* new_name = malloc(sizeof(char) * strlen(name+1));
    if (!new_name) {
        free(new_character);
        return -1;
    }
    new_name = strcpy(new_name, name);
    new_character -> name = new_name;
    new_character -> hp = hp;

    Weapon* new_weapon = malloc(sizeof(Weapon));
    if (!new_weapon) {
        free(new_name);
        free(new_character);
        return -1;
    }

    char* new_weapon_name = malloc(sizeof(char) * strlen(weapon_name+1));
    if (!new_weapon_name) {
        free(new_name);
        free(new_character);
        free(new_weapon);
        return -1;
    }

    new_weapon_name = strcpy(new_weapon_name, weapon_name);
    new_weapon -> name = new_weapon_name;
    new_weapon -> damage = weapon_damage;
    new_character -> weapon = new_weapon;


    character_list[nof_characters] = *new_character;
    nof_characters++;

    return 1;
}