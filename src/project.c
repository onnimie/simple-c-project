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
    new_character -> exp = 0;

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

int attack_names(const char* attacker_name, const char* target_name) {

    int attacker_found = 0;
    int target_found = 0;
    Character attacker;
    Character target;

    for (int i = 0; i < nof_characters; i++) {
        Character character = character_list[i];
        if (!attacker_found && (strcmp(attacker_name, character.name) == 0)) {
            attacker_found = 1;
            attacker = character;
        }
        if (!target_found && (strcmp(target_name, character.name) == 0)) {
            target_found = 1;
            target = character;
        }
    }
    if (!target_found || !attacker_found) return -1;

    return attack(&attacker, &target);
}

int attack(Character* attacker, Character* target) {

    Weapon attacker_weapon = *(attacker -> weapon);
    unsigned int damage = attacker_weapon.damage;
    unsigned int target_hp = target -> hp;
    if (target_hp < damage) {
        damage = target_hp;
    }

    attacker -> exp += damage;
    target -> hp -= damage;

    return 1;
}



int compare_for_qsort(const void* a, const void* b) {
    Character* a_character = (Character*)a;
    Character* b_character = (Character*)b;

    unsigned int a_exp = a_character -> exp;
    unsigned int b_exp = b_character -> exp;

    if (a_exp > b_exp) {
        return -1;
    } else {
        return 1;
    }
}

void print_characters() {
    // format characters, each on their own line:
    // <name> <hit-points> <experience> <weapon-name> <weapon-damage>
    qsort(character_list, nof_characters, sizeof(Character), compare_for_qsort);

    for (int i = 0; i < nof_characters; i++) {
        Character character = character_list[i];
        Weapon weapon = *(character.weapon);
        printf("%s %d %d %s %d", character.name, character.hp, character.exp, weapon.name, weapon.damage);
        printf("\n");
    }
}