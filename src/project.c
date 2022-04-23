#include "project.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"


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
                printf("ERROR: character already exists\n");
                return -1;
            }
        }
        character_list = malloc(sizeof(Character));
    } else {
        character_list = realloc(character_list, sizeof(Character) * (nof_characters+1));
    }

    Character* new_character = malloc(sizeof(Character));
    if (!new_character) {
        printf("ERROR: failed to allocate memory for character\n");
        return -1;
    }

    char* new_name = malloc(sizeof(char) * strlen(name+1));
    if (!new_name) {
        free(new_character);
        printf("ERROR: failed to allocate memory for character\n");
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
        printf("ERROR: failed to allocate memory for character\n");
        return -1;
    }

    char* new_weapon_name = malloc(sizeof(char) * strlen(weapon_name+1));
    if (!new_weapon_name) {
        free(new_name);
        free(new_character);
        free(new_weapon);
        printf("ERROR: failed to allocate memory for character\n");
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
    if (!target_found || !attacker_found) {
        printf("ERROR: character(s) not found\n");
        return -1;
    }

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

    printf("%s attacked %s with %s by %d damage.\n", attacker -> name, target -> name, attacker_weapon.name, damage);
    printf("%s has %d hit points remaining.\n", target -> name, target -> hp);
    printf("%s gained %d experience points.\n", attacker -> name, damage);
    printf("SUCCESS\n");

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

int save(const char* filename) {

    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("ERROR: couldn't open file\n");
        return -1;
    }

    // printing format:
    // <name> <hit-points> <experience> <weapon-name> <weapon-damage>
    for (int i = 0; i < nof_characters; i++) {
        Character character = character_list[i];
        Weapon weapon = *(character.weapon);

        int res = fprintf(file, "%s %d %d %s %d\n", character.name, character.hp, character.exp, weapon.name, weapon.damage);
        if (res < 0) {
            printf("ERROR: writing to file failed\n");
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return 1;
}

int load(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("ERROR: couldn't open file\n");
        return -1;
    }

    delete_characters();

    // file data format:
    // <name> <hit-points> <experience> <weapon-name> <weapon-damage> (newline)
    char name_buffer[1000];
    char weapon_name_buffer[1000];
    unsigned int hp;
    unsigned int exp;
    unsigned int damage;

    int res = fscanf(file, "%s %d %d %s %d\n", name_buffer, hp, exp, weapon_name_buffer, damage);
    while (res != EOF) {
        if (ferror(file)) {
            printf("ERROR: loading from file failed");
            fclose(file);
            return -1;
        }
        add_character(name_buffer, hp, weapon_name_buffer, damage);
        res = fscanf(file, "%s %d %d %s %d\n", name_buffer, hp, exp, weapon_name_buffer, damage);
    }
    

    fclose(file);
    return 1;
}

void delete_characters() {

    for (int i = 0; i < nof_characters; i++) {
        Character character = character_list[i];
        char* character_name = character.name;
        Weapon* weapon = character.weapon;
        char* weapon_name = weapon -> name;

        free(weapon_name);
        free(weapon);
        free(character_name);
        free(&character);
    }

    free(character_list);
    nof_characters = 0;
}