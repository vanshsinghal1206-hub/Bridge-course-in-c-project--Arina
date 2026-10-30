#include <stdio.h>
#include <stdlib.h>

// Union: hunger ya energy me se sirf ek value rehti hai
typedef union {
    int hunger;
    int energy;
} Status;

// Struct: ek pet ki saari info
typedef struct {
    char name[30];
    int level;
    char mode;      // 'H' = hunger, 'E' = energy
    Status status;
} Pet;

void adoptPet(Pet **pets, int *count, int *capacity);
void showPets(Pet *pets, int count);
int pickPet(int count);
void carePet(Pet *p);
int trainingPower(int base, int days);
int getScore(Pet *p);
Pet *strongestPet(Pet *pets, int count);

int main() {
    int capacity = 2, count = 0, choice;
    Pet *pets = (Pet *)malloc(capacity * sizeof(Pet));   // jagah li

    do {
        printf("\n1.Adopt 2.Show 3.Care 4.Train 5.Arena 6.Strongest 7.Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            adoptPet(&pets, &count, &capacity);

        } else if (choice == 2) {
            showPets(pets, count);

        } else if (choice == 3) {                 // Feed / Play
            int i = pickPet(count);
            if (i >= 0) carePet(pets + i);

        } else if (choice == 4) {                 // Train (recursion)
            int i = pickPet(count);
            if (i >= 0) {
                int days;
                printf("Kitne din train karein: ");
                scanf("%d", &days);
                Pet *p = pets + i;
                p->level = trainingPower(p->level * 10, days) / 10;
                printf("%s ka naya level: %d\n", p->name, p->level);
            }

        } else if (choice == 5) {                 // Arena battle
            if (count < 2) {
                printf("Battle ke liye 2 pets chahiye\n");
            } else {
                printf("Pehla pet:\n");
                int a = pickPet(count);
                printf("Doosra pet:\n");
                int b = pickPet(count);
                if (a < 0 || b < 0 || a == b) {
                    printf("Galat selection\n");
                } else {
                    Pet *p1 = pets + a;
                    Pet *p2 = pets + b;
                    printf("%s score %d  VS  %s score %d\n",
                           p1->name, getScore(p1), p2->name, getScore(p2));
                    if (getScore(p1) > getScore(p2)) {
                        printf("%s jeet gaya! Level +1\n", p1->name);
                        p1->level++;
                    } else if (getScore(p2) > getScore(p1)) {
                        printf("%s jeet gaya! Level +1\n", p2->name);
                        p2->level++;
                    } else {
                        printf("Draw!\n");
                    }
                }
            }

        } else if (choice == 6) {
            Pet *p = strongestPet(pets, count);
            if (p == NULL)
                printf("Koi pet nahi hai\n");
            else
                printf("Strongest: %s (Level %d)\n", p->name, p->level);
        }
    } while (choice != 7);

    free(pets);   // jagah wapas
    return 0;
}

// Pet add karo. Jagah full ho to realloc se double karo.
void adoptPet(Pet **pets, int *count, int *capacity) {
    if (*count == *capacity) {
        *capacity = *capacity * 2;
        *pets = (Pet *)realloc(*pets, *capacity * sizeof(Pet));
    }

    Pet *p = *pets + *count;   // naye pet ki jagah
    int m;

    printf("Naam: ");
    scanf("%29s", p->name);
    printf("Level: ");
    scanf("%d", &p->level);
    printf("1 = Hunger, 2 = Energy: ");
    scanf("%d", &m);

    if (m == 1) {
        p->mode = 'H';
        printf("Hunger %%: ");
        scanf("%d", &p->status.hunger);
    } else {
        p->mode = 'E';
        printf("Energy %%: ");
        scanf("%d", &p->status.energy);
    }
    (*count)++;
}

// Sab pets dikhao (pets + i = i-th pet ka address)
void showPets(Pet *pets, int count) {
    for (int i = 0; i < count; i++) {
        Pet *p = pets + i;
        printf("%d. %s | Level %d | ", i + 1, p->name, p->level);
        if (p->mode == 'H')
            printf("Hunger %d%%\n", p->status.hunger);
        else
            printf("Energy %d%%\n", p->status.energy);
    }
}

// Pet ka number puchho, index return karo (galat ho to -1)
int pickPet(int count) {
    int n;
    if (count == 0) {
        printf("Koi pet nahi hai\n");
        return -1;
    }
    printf("Pet number (1 se %d): ", count);
    scanf("%d", &n);
    if (n < 1 || n > count) {
        printf("Galat number\n");
        return -1;
    }
    return n - 1;
}

// Hunger wale ko khana, Energy wale ko khel
void carePet(Pet *p) {
    if (p->mode == 'H') {
        p->status.hunger -= 20;
        if (p->status.hunger < 0) p->status.hunger = 0;
        printf("%s ne khana khaya. Hunger: %d%%\n", p->name, p->status.hunger);
    } else {
        p->status.energy += 20;
        if (p->status.energy > 100) p->status.energy = 100;
        printf("%s khela. Energy: %d%%\n", p->name, p->status.energy);
    }
}

// Recursion: har din power +5
int trainingPower(int base, int days) {
    if (days <= 0) return base;               // ruk jao
    return 5 + trainingPower(base, days - 1); // khud ko dobara bulao
}

// Battle score: level ka zyada asar, status ka thoda
int getScore(Pet *p) {
    if (p->mode == 'E')
        return p->level * 10 + p->status.energy;
    return p->level * 10 + (100 - p->status.hunger);   // kam hunger = achha
}

// Sabse high level wale pet ka address return karo
Pet *strongestPet(Pet *pets, int count) {
    if (count == 0) return NULL;
    Pet *best = pets;
    for (int i = 1; i < count; i++)
        if ((pets + i)->level > best->level)
            best = pets + i;
    return best;
}
