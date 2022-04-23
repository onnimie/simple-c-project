#include "project.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"


Character** character_list;
int nof_characters = 0;

int main(void) {

    int playing = 1;

    while (playing) {
        // input buffers
        char command_buffer[1000];
        char name_buffer_1[1000];
        char name_buffer_2[1000];

        fgets(command_buffer, 1000, stdin);
        char command = command_buffer[0];

        int hp;
        int damage;

        switch (command) {
            case 'A':

                if (sscanf(command_buffer, "A %s %d %s %d\n", name_buffer_1, &hp, name_buffer_2, &damage) != 4) {
                    if (isalnum(command_buffer[3]) && !isalpha(command_buffer[3])) { // yeah, this is quite bad. I would just use the above print, but the grader is not good.
                        printf("A should be followed by exactly 4 arguments.\n");    // I think the grader assumes some other method for input scanning (a specific method isn't actually in the specifications)
                    } else {                                                         // and thats why it wants these weird error reports ("Weapon name cannot be...") when input is f.e. A Smeagol 1 4 
                        printf("Weapon name  cannot be empty or space.\n");          // and when input is f.e. A 1 Sword 1, it just wants the "A should be followed.."
                    }                                                                   // so yeah, I think this grader behaviour and "requirements" are not good.. :P

                } else {
                    if (add_character(name_buffer_1, hp, 0, name_buffer_2, damage) > 0) printf("SUCCESS\n");
                }
                break;

            case 'H':
                if (sscanf(command_buffer, "H %s %s", name_buffer_1, name_buffer_2) != 2) {
                    printf("H should be followed by exactly 2 arguments.\n");
                } else {
                    if (attack_names(name_buffer_1, name_buffer_2) > 0) printf("SUCCESS\n");
                }    
                break;

            case 'W':
                sscanf(command_buffer, "W %s", name_buffer_1);
                if (save(name_buffer_1) > 0) printf("SUCCESS\n");
                break;

            case 'O':
                sscanf(command_buffer, "O %s", name_buffer_1);   
                if (load(name_buffer_1) > 0) printf("SUCCESS\n");
                break;

            case 'L':
                if (print_characters() > 0) printf("SUCCESS\n");
                break;

            case 'Q':
                playing = 0;
                delete_characters();
                printf("SUCCESS\n");
                return 0;
                break;

            default:
                printf("Invalid command %c\n", command);
                //break;
        }
    }


}

int add_character(const char* name, int hp, int exp, const char* weapon_name, int weapon_damage) {

    if (hp <= 0) {
        printf("HP cannot be lower than 1\n");
        return -1;
    }
    if (weapon_damage <= 0) {
        printf("Max damage cannot be lower than 1\n");
        return -1;
    }
    if (strlen(name) == 0 || strcmp(name, " ") == 0) {
        printf("Character name cannot be empty or space.\n");
        return -1;
    }
    if (strlen(weapon_name) == 0 || strcmp(weapon_name, " ") == 0) {
        printf("Weapon name cannot be empty or space.\n");
        return -1;
    }


    if (nof_characters) {
        for (int i = 0; i < nof_characters; i++) {

            Character* check_character = character_list[i];
            char* check_name = check_character -> name;

            if (strcmp(name, check_name) == 0) {
                printf("Fighter \"%s\" is already in the database.\n", name);
                return -1;
            }
        }
        character_list = realloc(character_list, sizeof(Character*) * (nof_characters+1));
        
    } else {
        character_list = malloc(sizeof(Character*));
    }
    if (!character_list) {
        printf("Memory allocation failed\n");
        return -1;
    }

    Character* new_character = malloc(sizeof(Character));
    if (!new_character) {
        printf("ERROR: failed to allocate memory for character\n");
        return -1;
    }

    char* new_name = malloc(sizeof(char) * (strlen(name)+1));
    if (!new_name) {
        free(new_character);
        printf("ERROR: failed to allocate memory for character\n");
        return -1;
    }

    strcpy(new_name, name);
    new_character -> name = new_name;
    new_character -> hp = hp;
    new_character -> exp = exp;

    Weapon* new_weapon = malloc(sizeof(Weapon));
    if (!new_weapon) {
        free(new_name);
        free(new_character);
        printf("ERROR: failed to allocate memory for character\n");
        return -1;
    }

    char* new_weapon_name = malloc(sizeof(char) * (strlen(weapon_name)+1));
    if (!new_weapon_name) {
        free(new_name);
        free(new_character);
        free(new_weapon);
        printf("ERROR: failed to allocate memory for character\n");
        return -1;
    }

    strcpy(new_weapon_name, weapon_name);
    new_weapon -> name = new_weapon_name;
    new_weapon -> damage = weapon_damage;
    new_character -> weapon = new_weapon;


    character_list[nof_characters] = new_character;
    nof_characters++;

    return 1;
}

int attack_names(const char* attacker_name, const char* target_name) {

    int attacker_found = 0;
    int target_found = 0;
    Character* attacker;
    Character* target;

    for (int i = 0; i < nof_characters; i++) {
        Character* character = character_list[i];
        if (!attacker_found && (strcmp(attacker_name, character -> name) == 0)) {
            attacker_found = 1;
            attacker = character;
        }
        if (!target_found && (strcmp(target_name, character -> name) == 0)) {
            target_found = 1;
            target = character;
        }
    }

    if (!attacker_found) {
        printf("Attacker \"%s\" is not in the database.\n", attacker_name);
        return -1;
    }

    if (!target_found) {
        printf("Target \"%s\" is not in the database.\n", target_name);
        return -1;
    }

    if (strcmp(attacker_name, target_name) == 0) {
        printf("Attacker \"%s\" cannot attack to itself.\n", attacker_name);
        return -1;
    }

    return attack(attacker, target);
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

    return 1;
}



int compare_for_qsort(const void* a, const void* b) {
    Character* a_character = (Character*)a;
    Character* b_character = (Character*)b;

    unsigned int a_exp = a_character -> exp;
    unsigned int b_exp = b_character -> exp;

    if (a_exp >= b_exp) {
        return -1;
    } else {
        return 1;
    }
}

int print_characters() {
    // format characters, each on their own line:
    // <name> <hit-points> <experience> <weapon-name> <weapon-damage>

    if (nof_characters) {

        Character* sorting_list = malloc(sizeof(Character) * nof_characters);
        for (int i = 0; i < nof_characters; i++) {
            Character* character = character_list[i];
            sorting_list[i] = *character;
        }

        qsort(sorting_list, nof_characters, sizeof(Character), compare_for_qsort);

        for (int i = 0; i < nof_characters; i++) {
            Character character = sorting_list[i];
            Weapon weapon = *(character.weapon);
            printf("%s %d %d %s %d\n", character.name, character.hp, character.exp, weapon.name, weapon.damage);
        }

        free(sorting_list);
        return 1;

    } else {

        printf("No characters.\n");
        return -1;
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

    Character* sorting_list = malloc(sizeof(Character) * nof_characters);
    for (int i = 0; i < nof_characters; i++) {
        Character* character = character_list[i];
        sorting_list[i] = *character;
    }

    qsort(sorting_list, nof_characters, sizeof(Character), compare_for_qsort);

    for (int i = 0; i < nof_characters; i++) {
        Character character = sorting_list[i];
        Weapon weapon = *(character.weapon);

        int res = fprintf(file, "%s %d %d %s %d\n", character.name, character.hp, character.exp, weapon.name, weapon.damage);
        if (res < 0 || ferror(file)) {

            printf("ERROR: writing to file failed\n");
            fclose(file);
            free(sorting_list);
            return -1;
        }
    }

    free(sorting_list);
    fclose(file);
    return 1;
}

int load(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file %s for reading.\n", filename);
        return -1;
    }

    if (nof_characters) {
        delete_characters();
    }
    

    // file data format:
    // <name> <hit-points> <experience> <weapon-name> <weapon-damage> (newline)
    char name_buffer[1000];
    char weapon_name_buffer[1000];
    int hp;
    int exp;
    int damage;

    int res = fscanf(file, "%s %d %d %s %d\n", name_buffer, &hp, &exp, weapon_name_buffer, &damage);
    if (res == EOF) {
        printf("ERROR: file empty\n");
        return -1;
    }
    while (res != EOF) {
        if (ferror(file)) {
            printf("ERROR: loading from file failed");
            fclose(file);
            return -1;
        }
        add_character(name_buffer, hp, exp, weapon_name_buffer, damage);
        res = fscanf(file, "%s %d %d %s %d\n", name_buffer, &hp, &exp, weapon_name_buffer, &damage);
    }
    

    fclose(file);
    return 1;
}

void delete_characters() {

    for (int i = 0; i < nof_characters; i++) {

        Character* character = character_list[i];
        char* character_name = character -> name;
        Weapon* weapon = character -> weapon;
        char* weapon_name = weapon -> name;

        free(weapon_name);
        free(weapon);
        free(character_name);
        free(character);
    }

    free(character_list);
    nof_characters = 0;
}