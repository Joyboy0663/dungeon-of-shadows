#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

struct Fighter {
    std::string name;
    int hp, maxHp, atk, def;
};

std::mt19937 rng(std::random_device{}());

int roll(int lo, int hi) {
    std::uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

int readChoice(int lo, int hi) {
    int c;
    while (true) {
        std::cout << "> ";
        if (std::cin >> c && c >= lo && c <= hi) return c;
        if (std::cin.eof()) std::exit(0);
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter a number from " << lo << " to " << hi << ".\n";
    }
}

void showBar(const Fighter& f) {
    const int width = 20;
    int filled = f.hp * width / f.maxHp;
    std::cout << f.name << " [" << std::string(filled, '#')
              << std::string(width - filled, '-') << "] "
              << f.hp << "/" << f.maxHp << "\n";
}

int damage(const Fighter& attacker, const Fighter& target) {
    return std::max(1, attacker.atk + roll(-2, 3) - target.def);
}

Fighter makeEnemy(int floor) {
    static const std::vector<std::string> names = {
        "Goblin", "Skeleton", "Dark Wolf", "Cultist", "Wraith"};
    if (floor == 5) return {"Shadow King", 60, 60, 10, 3};
    int hp = 18 + floor * 6 + roll(0, 5);
    std::string name = names[roll(0, static_cast<int>(names.size()) - 1)];
    return {name, hp, hp, 4 + floor, floor / 2};
}

// Returns true if the player wins the fight.
bool battle(Fighter& player, Fighter enemy, int& potions) {
    std::cout << "\nA " << enemy.name << " appears!\n";

    while (player.hp > 0 && enemy.hp > 0) {
        std::cout << "\n";
        showBar(player);
        showBar(enemy);
        std::cout << "1) Attack  2) Heavy strike  3) Potion (" << potions
                  << ")  4) Defend\n";

        int choice = readChoice(1, 4);
        bool defending = false;
        bool enemyActs = true;

        if (choice == 1) {
            int dmg = damage(player, enemy);
            bool crit = roll(1, 100) <= 15;
            if (crit) dmg *= 2;
            enemy.hp = std::max(0, enemy.hp - dmg);
            std::cout << (crit ? "CRITICAL! " : "") << "You hit for " << dmg << ".\n";
        } else if (choice == 2) {
            if (roll(1, 100) <= 60) {
                int dmg = damage(player, enemy) * 2;
                enemy.hp = std::max(0, enemy.hp - dmg);
                std::cout << "Heavy strike lands for " << dmg << "!\n";
            } else {
                std::cout << "You swing wide and miss.\n";
            }
        } else if (choice == 3) {
            if (potions > 0) {
                int heal = roll(12, 18);
                player.hp = std::min(player.maxHp, player.hp + heal);
                --potions;
                std::cout << "You drink a potion and heal " << heal << " HP.\n";
            } else {
                std::cout << "No potions left!\n";
                enemyActs = false;
            }
        } else {
            defending = true;
            std::cout << "You raise your guard.\n";
        }

        if (enemy.hp > 0 && enemyActs) {
            int dmg = damage(enemy, player);
            if (defending) dmg = std::max(1, dmg / 2);
            player.hp = std::max(0, player.hp - dmg);
            std::cout << "The " << enemy.name << " hits you for " << dmg << ".\n";
        }
    }
    return player.hp > 0;
}

void chooseReward(Fighter& player, int& potions) {
    std::cout << "\nChoose a reward:\n"
              << "1) +10 max HP and full heal\n"
              << "2) +2 attack\n"
              << "3) +1 potion\n";
    int choice = readChoice(1, 3);
    if (choice == 1) {
        player.maxHp += 10;
        player.hp = player.maxHp;
    } else if (choice == 2) {
        player.atk += 2;
    } else {
        ++potions;
    }
}

int main() {
    std::cout << "=== DUNGEON OF SHADOWS ===\n"
              << "Clear 5 floors and defeat the Shadow King.\n\n"
              << "Enter your name: ";
    std::string name;
    std::getline(std::cin, name);
    if (name.empty()) name = "Joyboy";

    Fighter player{name, 40, 40, 8, 2};
    int potions = 3;

    for (int floor = 1; floor <= 5; ++floor) {
        std::cout << "\n------ Floor " << floor << " ------";
        if (!battle(player, makeEnemy(floor), potions)) {
            std::cout << "\nYou fell on floor " << floor << ". Game over.\n";
            return 0;
        }
        std::cout << "\nVictory!\n";
        if (floor < 5) chooseReward(player, potions);
    }

    std::cout << "\nThe Shadow King crumbles to dust. You win, " << player.name << "!\n";
    return 0;
}