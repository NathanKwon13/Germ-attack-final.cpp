//
//  main.cpp
//  Germ_attack_final.cpp
//
//  Created by Nathan Kwon on 8/10/23.
//

#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <ctime>

using namespace std;

int main() {
    bool w = false; // win/loss boolean
    bool isActive = true; // game loop boolean
    
    // Germ stats
    int infectionRate = 2; // rate that infected people increase. Scale: 1 - 6
    int stealth = 3; // Scale: 1 - 6
    
    // time
    int monthsPassed = 0;
    const int SURVIVAL_GOAL = 12;
    
    // Germ team's moves
    vector<string> moves;
    moves.push_back("Infect");      // increases infected count (enemy counterattack: treat citizens)
    moves.push_back("Migrate");     // increases stealth (enemy counterattack: research)
    moves.push_back("Mutate");      // increases infection rate (enemy counterattack: lockdown)
    
    vector<string>::iterator iter;
    
    // City stats
    int population = 1000000;
    int infected = 100;
    int publicSanity = 100; // well-being of city, the higher the number the happier. Reaching 0 means the citizens riot, germs win. scale: 0 - 100
    int lockdownStreak = 0;
    bool lockdown = false;
    int cureProgress = 0; // // Once cureProgress reaches 100, city wins. Increased by 'develope a cure'
    int researchRate = 14;
    
    vector<string> city_moves;
    city_moves.push_back("Develop a cure");                 // increases cureProgress, gets closer to win
    city_moves.push_back("Research the germ");              // decreases stealth
    city_moves.push_back("Treat the infected citizens");    // decreases infected count
    city_moves.push_back("Initiate a lockdown protocol");   // decreases infection rate and prevents it from rising
    
    
    // Intro screen
    cout << "\t\t\t\tGERM ATTACK\n" << endl;
    cout << "\nWelcome to GERM ATTACK!" << endl;
    cout << "\n\tYou are a germ that must take over an entire city. Employ various strategies and schemes to develop your virus, infect thousands of citizens, maneuver around the city's precautionary defenses, and begin an epidemic!" << endl;
    cout << "\nWays to win: " << endl;
    cout << "\t- Infect each citizen in the city." << endl;
    cout << "\t- Make the city enter a state of complete insanity." << endl;
    cout << "\nWays to tie: " << endl;
    cout << "\t- Survive a whole year without being eradicated." << endl;
    cout << "\nGood luck! Beginning game...\n" << endl;
    
    
    // Game Loop
    while (isActive) {
        cout << "\n\nMonth " << (monthsPassed + 1) << endl;
        cout << "The current number of infected is " << infected << " out of " << population << " people.\n" << endl;
        
        // Your turn!
        cout << "\nGerm's turn!" << endl;
        cout << "Here are your possible moves this turn:\n" << endl;
        
        int a = 1;
        for (iter = moves.begin(); iter != moves.end(); ++iter) {
            cout << "\tType the number " << a << " to " << *iter << "." << endl;
            ++a;
        }
        cout << "What would you like to do next? Choose one: " << endl;
        
        int choice;
        cin >> choice;
        while (choice != 1 && choice != 2 && choice != 3) {
            cout << "Not a valid input, please enter an available option: " << endl;
            cin >> choice;
        }
        
        if (choice == 1) { // Infect
            cout << "\nYou have selected Infect." << endl;
            int newInfected = infected + (infected * infectionRate);
            if (newInfected >= 1000000) {        // If the whole city is infected, germ victory (1)
                newInfected = 1000000;
                cout << "\nYou have infected the whole city!" << endl;
                w = true;       // Germs won
                break;
            }
            cout << "\nThe current number of infected has increased from " << infected << " to " << newInfected << ".\n" << endl;
            infected = newInfected;
            
        } else if (choice == 2) { // Migrate
            cout << "\nYou have selected Migrate." << endl;
            cout << "Your germ's stealth has increased by 1." << endl;
            stealth += 1;
            
        } else if (choice == 3) { // Mutate
            cout << "\nYou have selected Mutate." << endl;
            if (lockdown) {
                cout << "The germ has mutated, but it cannot boost the efficiency of your infection since a lockdown is active." << endl;
            } else {
                cout << "The germ has mutated, boosting the efficiency of your infection. The next time you Infect, the attack will be stronger." << endl;
                infectionRate += 1;
            }
            
        }
        
        // City turn!
        cout << "\nCity's turn!" << endl;
        
        // calculate city cureProgress
        if (cureProgress > 100) {
            cureProgress = 100;
        }
        cout << "The cure is " << cureProgress << "% complete.";
        if (cureProgress == 100) {
            cout << "\nThe city is now completely immune to your disease. The city team has won!" << endl;
            cout << "\n\n\t\t\t\tGAME OVER" << endl;
            w = false;      // germs lost
            break;
        }
            // calculate city sanity
            cout << " Meanwhile, ";
            if (publicSanity > 90) {
                cout << "the citizens continue to remain strong, and with faithful hearts, they wait patiently for the team to devise a cure." << endl;
            } else if (publicSanity > 60) {
                cout << "the city begins to grow weary of this sickness and long for its end, although some are skeptical..." << endl;
            } else if (publicSanity > 40) {
                cout << "the citizens are tired of this disease, some a little frustrated at the germs, others even at the city's own medical team. Despair hovers over the city..." << endl;
            } else if (publicSanity > 20) {
                cout << "the city is restless, as people lose hope. Some riots and protests even occur." << endl;
            } else if (publicSanity > 0) {
                cout << "the city is going insane! Can the medical team devise a solution before it is too late?" << endl;
            } else if (publicSanity == 0) {
                cout << "the city has officially sunk into total madness, and the medical team is forced to flee the city due to violent threats from the public." << endl;
                w = true; // germs won
                break;
            }
            
            // cease lockdown?
            if (lockdown == true && lockdownStreak > 0) {
                if (infected < 350000) {
                    cout << "Lockdown has been lifted." << endl;
                    lockdown = false;
                    lockdownStreak = 0;
                } else {
                    cout << "Lockdown will continue. It is now month " << lockdownStreak + 1 << " of lockdown." << endl;
                    lockdownStreak += 1;
                    publicSanity -= 10 * lockdownStreak;
                }
            }
            
            // the city will make a move.
            /*
             AI decision tree:
                Priority 1: keep infection count less than 100%
                
             */
            // priority 1: keep infection count less than 100%
            if (infected > 850000) {
                cout << "\nThe medical team is choosing to treat infected citizens, temporarily keeping the city stable from an epidemic." << endl;
                int newInfected = infected - (7 - stealth) * 1000;
                publicSanity += 5;
                cout << "\nInfected count has gone from " << infected << " to " << newInfected << "." << endl;
            } else if ((lockdown == false && infectionRate > 4 && infected > 250000) || (infectionRate > 3 && monthsPassed > 1)) { // priority 2: lockdown if infectionRate is dangerously high and threatens wipeout next turn (if lockdown is off); or enact lockdown if the germ is rapidly spamming an early mutation.
                cout << "\nThe medical team has issued a citywide lockdown to slow the spread of the virus." << endl;
                lockdown = true;
                lockdownStreak += 1;
                infectionRate -= 1;
                publicSanity -= 10;
            } else if (cureProgress < 30 || cureProgress + (researchRate - stealth) >= 100) { // priority 3: if cureProgress is low or close to 100, increase it.
                int newCureProgress = cureProgress + researchRate - stealth;
                if (newCureProgress > 100) {
                    newCureProgress = 100;
                }
                cout << "\nThe medical team is choosing to continue developing a cure, increasing the city's Research from " << cureProgress << " to " << newCureProgress << "." << endl;
                cureProgress = newCureProgress;
                publicSanity -= 10;
            } else if (stealth > 4) { // priority: if stealth is above 4, decrease it.
                cout << "\nThe medical team is now researching the germ further, decreasing its stealth from " << stealth << " to " << stealth - 1 << "." << endl;
                stealth -= 1;
                publicSanity -= 8;
            } else if (cureProgress + (researchRate - stealth) * 2 >= 100) { // priority: if cureProgress is semi-close to 100, increase.
                int newCureProgress = cureProgress + researchRate - stealth;
                cout << "\nThe medical team is choosing to continue developing a cure, increasing the city's Research from " << cureProgress << " to " << newCureProgress << "." << endl;
                cureProgress = newCureProgress;
                publicSanity -= 10;
            } else if (stealth > 3) {
                cout << "\nThe medical team is now researching the germ further, decreasing its stealth from " << stealth << " to " << stealth - 1 << "." << endl;
                stealth -= 1;
                publicSanity -= 8;
            } else { // default priority: increase cureProgress.
                int newCureProgress = cureProgress + researchRate - stealth;
                cout << "\nThe medical team is choosing to continue developing a cure, increasing the city's Research from " << cureProgress << " to " << newCureProgress << "." << endl;
                cureProgress = newCureProgress;
                publicSanity -= 10;
            }
            
            
            // Evaluating if any team has lost
            
            monthsPassed += 1; // time increment
            
            // Tie-game
            if (monthsPassed == SURVIVAL_GOAL) {
                cout << "\nYou have successfully survived eradication for a whole year! The germs are victorious!" << endl;
                cout << "\n\n\t\t\t\tGAME OVER" << endl;
                isActive = false;
                w = true;       // germs won
            }
            
            // prevent publicSanity from going under 0
            if (publicSanity < 0) {
                publicSanity = 0;
            }
        } // end game loop
        
        
        return 0;
    }
