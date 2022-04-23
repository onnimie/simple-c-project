#ifndef _PROJECT__H_
#define _PROJECT__H_

typedef struct {
    char* name;
    unsigned int hp;
    unsigned int exp;
    Weapon* weapon;
} Character;

typedef struct {
    char* name;
    unsigned int damage;
} Weapon;

Character* character_list;
unsigned int nof_characters;

//user commands
int add_character(const char* name, unsigned int hp, const char* weapon_name, unsigned int weapon_damage); //init with 0 exp
int attack_names(const char* attacker_name, const char* target_name);
int attack(Character* attacker, Character* target);
void print_characters(void);
int save(const char* filename);
int load(const char* filename);

//helper functions
//Character* find_character(const char* name, Character* list_of_characters);
int compare_for_qsort(const void* a, const void* b);
void delete_characters(void);



#endif //! _PROJECT__H_