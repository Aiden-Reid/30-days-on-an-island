#include <algorithm> //vector functions
#include <cstdlib> //rand()
#include <cstring> //string methods
#include <ctime> //time(NULL)
#include <iostream>
#include <string>
#include <vector>
#include <windows.h> //Sleep()
using namespace std;

/*
Structures
*/
struct items{
	/*
	Used to store how much of a fruit/animal/event affects stats
	*/
	string name;
	int hunger=0; //each stat default is 0
	int thirst=0;
	int energy=0;
	bool isHostile=0;
};

//supplies and non-food island items
struct items sleep_ {"sleep", -10, -20, 20};

struct items water {"water bottle", 0, 30};

struct items firstAidKit {"first aid kit", 0, 0, 50};

struct items cannedBeans {"canned beans", 20, 0};

struct items lighter {"lighter", 0, 0, 10};

struct items rope {"rope", 0, 0, 20};

struct items radio {"radio"};

struct items campfire {"campfire", 0, 0, -15};

struct items shelter {"shelter", 0, 0, -60};

//illnesses and weather
struct items foodPoisoning {"food poisoning", -30, -30, -30, 1};

struct items heatWave {"heat wave", -20, -40, -40, 1};

struct items tsunami {"tsunami", 0, 0, -50, 1};

struct items thunderStorm {"thunder storm", 0, 0, -40, 1};

struct items windStorm {"wind storm", 0, 0, -30, 1};

//fruits and animals
struct items coconut {"coconut", 5, 0, -5};

struct items berries {"berries", 2, 0, -2}; //chance to be poisonous?

struct items gatheredWater {"water", 0, rand()%(30-10)+10, -30};//gain random amount of thirst from 10 to 30

struct items fish {"fish", 15, 0, -40};

struct items shark {"shark", 0, 0, -70, 1};

struct items chicken {"chicken", 20, 0, -30};

struct items boar {"boar", 40, 0, -60, 1};
//end of items

/*
Basic Functions
*/
void startingPrompt();

void choosingSupplies(vector<string> &, int &, const int);

void viewStats(vector<string>, int, int, int);

void notValidAnswer();

void stringToLower(string &);

bool inStringVector(vector<string>, string);

void findAndRemove(vector<string> &, string);

void adjustStat(int &);

/*
Scenarios
Seperate by categories:
collecting supplies (shelter, fire, food, water), weather, illness, pure luck, etc
*/

//Collecting Supplies
void buildShelter(vector<string> &, int &, bool &);

void gatherSupplies(vector<string> &, int &, int &, int &);

void makeCampfire(vector<string> &, int &);

void drinkWater(vector<string> &, int &, int &);

void gatherWater(int &, int &);

void eatFood(vector<string> &, int &, int &, int &);

void gatherFood(vector<string> &, int &, int &, int &);

void gatheredCoconut(vector<string> &, int &);

void gatheredBerries(vector<string> &, int &);

void gatheredChicken(vector<string> &, int &);

void gatheredFish(vector<string> &, int &);

void gatheredBoar(vector<string> &, int &);

void useFAK(vector<string> &, int &);

//Weather
void weather(vector<string> &, int &, int &, int &, bool &);

//Illness
bool hasPoisoning();

//Pure luck
bool rescuedEarly(vector<string>, bool &);

void randomWashup(vector<string> &);

//Other
void endOfDay(int &, int &, int &, bool);

bool isDead(int, int, int);

vector<string> allFoods {cannedBeans.name, coconut.name, berries.name, fish.name, chicken.name, boar.name}; //global vector that stores the name of all the foods

int main(){
	/*
	variables
	*/
	const int MAX_SUPPLY_COUNT=3;
	bool hasShelter=0, early=0;
	int hunger=0, thirst=0, energy=0, supplyCount=0, dayCount=0;
	vector<string> inventory;
	
	/*
	Start of game
	*/
	startingPrompt();
	choosingSupplies(inventory, supplyCount, MAX_SUPPLY_COUNT); //player chooses supplies
	cout<<endl;
	
	//generating random starting stats
	srand(time(NULL)); 
	hunger=rand()%(100-76)+75;
	thirst=rand()%(100-76)+75;
	energy=rand()%(100-76)+75;
	
	Sleep(1000);
	cout<<"You've arrived on the island with your supplies. "<<flush;
	Sleep(1000);
	cout<<"Help will arrive in 30 days."<<endl;
	Sleep(1000);
	cout<<"Your goal is to survive until help arrives. "<<flush;
	Sleep(2000);
	cout<<"Good Luck.\n"<<endl;
	Sleep(1000);
	cout<<"------------------------------------------------------------------------------"<<endl;
	Sleep(1000);
	
	while((hunger>0&&thirst>0&&energy>0)&&(dayCount<30)){ //loop ends when any of the stats go below 0 or survived 30 days
		++dayCount;
		cout<<"Day: "<<dayCount<<".\n"<<endl;
		Sleep(1000);
		if(dayCount==30){ //breaks out of loop when day 30 is reached and after it's displayed
			break;
		}
		
		if(dayCount>=15){ //chance to get rescued early. keep at top of while loop
			if(rescuedEarly(inventory, early)){
				break;
			}
		}
		
		viewStats(inventory, hunger, thirst, energy);
		
		if(!hasShelter){ //if the user does not have shelter built
			buildShelter(inventory, energy, hasShelter);
		}
		if(isDead(hunger, thirst, energy)){
			break;
		}
		
		gatherSupplies(inventory, hunger, thirst, energy);
		if(isDead(hunger, thirst, energy)){
			break;
		}
		
		randomWashup(inventory);
		
		eatFood(inventory, hunger, thirst, energy);
		if(isDead(hunger, thirst, energy)){
			break;
		}
		
		drinkWater(inventory, thirst, energy);
		if(isDead(hunger, thirst, energy)){
			break;
		}
		
		weather(inventory, hunger, thirst, energy, hasShelter);
		if(isDead(hunger, thirst, energy)){
			break;
		}
		
		if(energy<=30){
			useFAK(inventory, energy);
		}
		if(isDead(hunger, thirst, energy)){
			break;
		}
		
		endOfDay(hunger, thirst, energy, hasShelter);
		// viewStats(hunger, thirst, energy, inventory);
		cout<<endl<<"------------------------------------------------------------------------------"<<endl;
	}//end of while loop
	
	
	
	/*
	Final Messages
	*/
	if(dayCount==30){
		cout<<"\nYou did it!"<<endl;
		Sleep(4000);
		cout<<"You were rescued and they made a Hollywood movie about you starring Tom Hanks.\n"<<flush;
		Sleep(4000);
		cout<<"Good job!"<<endl;
	}
	else if(early){
		cout<<"\nYou were rescued early on day "<<dayCount<<"!"<<endl;
		Sleep(4000);
		cout<<"You were brought home safely and you were later interviewed by international"<<endl;
		cout<<"news networks and you published a book about your story."<<endl;
		Sleep(5000);
		cout<<"Good job!"<<endl;
	}
	else{
		if(hunger<=0){
			cout<<"\nYou got too hungry and died on the island on day "<<dayCount<<"."<<endl;
			Sleep(3000);
			cout<<"Better luck next time!\n";
		}
		else if(thirst<=0){
			cout<<"\nYou got too thirsty and died on the island on day "<<dayCount<<"."<<endl;
			Sleep(3000);
			cout<<"Better luck next time!\n";
		}
		else{
			cout<<"\nYou ran out of energy and died on the island on day "<<dayCount<<"."<<endl;
			Sleep(3000);
			cout<<"Better luck next time!\n";
		}
	}
	
	return 0;
}

/*
Basic Functions
*/
void startingPrompt(){ //if you add supplies make sure you add choosingSupplies
	/*
	Prints the prompt when you first start the game.
	*/
	cout<<"Your plane just crashed in the water near an island."<<endl;
	Sleep(2000);
	cout<<"You must swim to the shore, but you can only carry 3 supplies:"<<endl;
	Sleep(3000);
	cout<<"a. Water bottle"<<endl;
	Sleep(500);
	cout<<"b. First aid kit"<<endl;
	Sleep(500);
	cout<<"c. Canned beans"<<endl;
	Sleep(500);
	cout<<"d. Lighter"<<endl;
	Sleep(500);
	cout<<"e. Rope"<<endl;
	Sleep(500);
	cout<<"f. Radio"<<endl;
	Sleep(500);
	cout<<"What will you choose?"<<endl;
}

void choosingSupplies(vector<string> &inventory, int &supplyCount, const int MAX_SUPPLY_COUNT){
	/*
	Asks what supplies the player wants to choose from the wreckage. A total of MAX_SUPPLY_COUNT supplies
	can be chosen.
	Inputs: Pass inventory, supplyCount, radioBonus, and constant integer MAX_SUPPLY_COUNT by reference.
	Outputs: Changes to inventory and radio bonus are made based on the player's decision.
	*/
	char choice;
	while(supplyCount<MAX_SUPPLY_COUNT){
		cout<<">>";
		cin>>choice;
		switch(choice){
			case 'a': inventory.push_back(water.name); cout<<"You chose water bottle.\n"; break;
			case 'b': inventory.push_back(firstAidKit.name); cout<<"You chose first aid kit.\n"; break;
			case 'c': inventory.push_back(cannedBeans.name); cout<<"You chose canned beans.\n"; break;
			case 'd': inventory.push_back(lighter.name); cout<<"You chose a lighter.\n"; break;
			case 'e': inventory.push_back(rope.name); cout<<"You chose rope.\n"; break;
			case 'f': inventory.push_back(radio.name); cout<<"You chose a radio.\n"; break;
			default: cout<<"That is not an option.\n"; --supplyCount; break;
		}
		cin.ignore();
		++supplyCount;
	}
}

void viewStats(vector<string> inventory, int hunger, int thirst, int energy){
	/*
	Prints the value of each stat and the items in the inventory.
	Parameters: Hunger, thirst, energy stats and the inventory vector.
	Outputs: None
	*/
	cout<<"Your stats:\n";
	Sleep(500);
	cout<<"Hunger: "<<flush;
	Sleep(500);
	cout<<hunger<<" out of 100"<<endl;
	cout<<"Thirst: "<<flush;
	Sleep(500);
	cout<<thirst<<" out of 100"<<endl;
	cout<<"Energy: "<<flush;
	Sleep(500);
	cout<<energy<<" out of 100"<<endl;
	cout<<"Supplies left: ";
	Sleep(500);
	
	if(inventory.size()==0){
			cout<<"None";
	}
	else{
		string delim="";
		for(const auto &s: inventory){
			cout<<delim<<s;
			delim=", ";
		}
	}
	cout<<".\n\n"<<flush;
}

void notValidAnswer(){
	/*
	Used in else branches when you want the string inputs to be 'yes' or 'no'
	*/
	cout<<"Not a valid answer. Please type 'yes' or 'no'."<<endl;
}

void stringToLower(string &s){
	/*
	Converts string to all lowercase letters
	Parameters: A string passed by reference
	Outputs: Changes the value of string
	*/
	for(int i=0; i<s.size(); i++){
		s[i]=tolower(s[i]);
	}
}

bool inStringVector(vector<string> v, string s){
	/*
	Checks if a string is in a string vector.
	Parameters: A string vector and a string
	Outputs: If the string is in the vector, 1 is returned. If not, 0 is returned.
	*/
	for(string f: v){
		if(f==s){
			return 1;
		}
	}
	return 0;
}

void findAndRemove(vector<string> &v, string s){
	/*
	Finds an elements in a string vector and if the string is in the vector,
	it is removed.
	Parameters: A string vector passed by reference and the string you want to remove
	Outputs: Removes string from the vector
	*/
	auto it=find(v.begin(), v.end(), s);
	if(it!=v.end()){ //if element is in vector
		v.erase(it);
	}
}

void adjustStat(int &stat_){
	/*
	If a stat goes above 100, it is set to 100. This is because each stat is out of 100.
	Parameters: Pass the stat of interest by reference
	Outputs: None; changes value of passed stat
	*/
	if(stat_>100){
		stat_=100;
	}
}

/*
Scenarios
Seperate by categories:
collecting supplies (shelter, fire, food, water), weather, illness, pure luck
*/

//Collecting Supplies
void buildShelter(vector<string> &inventory, int &energy, bool &hasShelter){
	/*
	Asks user if they want to build a shelter.
	Parameters: pass boolean hasShelter, int energy, and string vectory inventory by reference
	Outputs: none; only changes value of hasShelter, energy, and inventory if user has a rope and
	chooses to use it
	*/
	string choice="";
	
	cout<<"Would you like to build a shelter? This will cost "<<-shelter.energy<<" energy."<<endl;
	while(choice!="yes"&&choice!="no"){
		cout<<">>";
		getline(cin, choice);
		stringToLower(choice);
		if(choice=="yes"){
			if(inStringVector(inventory, rope.name)){ //if user has rope in their inventory
				choice="";
				cout<<"Would you like to use your rope to build the shelter?"<<endl;
				Sleep(500);
				cout<<"This will take "<<rope.energy<<" less energy than building the shelter without a rope."<<endl;
				while(choice!="yes"&&choice!="no"){
					cout<<">>";
					getline(cin, choice);
					stringToLower(choice);
					if(choice=="yes"){
						energy+=rope.energy;
						findAndRemove(inventory, rope.name); //removes one rope from inventory
						//add 20 energy so when the energy is subtracted 40, the net change is -20
						//also, remove "rope" from inventory
						cout<<"You have used your rope."<<endl;
					}
					else if(choice=="no"){
						cout<<"You have decided to build a shelter without using your rope."<<endl;
					}
					else{
						cout<<"Not a valid answer. Please type 'yes' or 'no'."<<endl;
					}
				}
			}
			Sleep(500);
			energy+=shelter.energy;
			cout<<"You have built a shelter to protect yourself."<<endl;
			hasShelter=1;
		}
		else if(choice=="no"){
			cout<<"You have decided to not build a shelter."<<endl;
			hasShelter=0;
		}
		else{
			notValidAnswer();
		}
	}
	adjustStat(energy);
	cout<<endl;
	Sleep(1000);
}

void gatherSupplies(vector<string> &inventory, int &hunger, int &thirst, int &energy){
	/*
	Asks player what supplies they would like to gather and the corresponding function
	calls are made based on their decision.
	Inputs: Pass inventory and stats by referece in order
	Outputs: Changes to inventory and the stats are made accordingly.
	*/
	string answer="";
	char choice;
	
	cout<<"Would you like to gather supplies?"<<endl;
	while(answer!="yes"&&answer!="no"){
		cout<<">>";
		getline(cin, answer);
		stringToLower(answer);
		if(answer=="yes"){
			int count=0;
			cout<<"\nWhat would you like to gather?"<<endl;
			Sleep(500);
			cout<<"a. Food"<<endl;
			Sleep(500);
			cout<<"b. Water"<<endl;
			Sleep(500);
			cout<<"c. Campfire Materials"<<endl;
			Sleep(500);
			cout<<"d. Change mind"<<endl;
			while(count==0){
				cout<<">>";
				cin>>choice;
				switch(choice){
					case 'a': gatherFood(inventory, hunger, thirst, energy); break;
					case 'b': gatherWater(thirst, energy); cin.ignore(); break;
					case 'c': makeCampfire(inventory, energy); break;
					case 'd': cout<<"You decided to not gather any supplies."<<endl; cin.ignore(); break;
					default: cout<<"That is not an option."<<endl; --count; cin.ignore(); break;
				}
				++count;
			}
		}
		else if(answer=="no"){
			cout<<"You decided to not gather any supplies."<<endl;
		}
		else{
			notValidAnswer();
		}
	}
	Sleep(1000);
}

void makeCampfire(vector<string> &inventory, int &energy){
	/*
	If the player has a lighter, they are asked if they want to use it
	to make a campfire. Using a lighter will take less energy to make a
	campfire than not using a lighter. If the player doesn't have a
	lighter, campfire.energy is used to make a campfire and a campfire
	is added to their inventory.
	Inputs: Pass inventory and energy by reference
	Outputs: Changes to inventory and energy made accordingly.
	*/
	string answer="";
	bool usedLighter=0;
	
	cin.ignore();
	
	if(inStringVector(inventory, lighter.name)){
		cout<<"Would you like to use your lighter to make a campfire?"<<endl;
		cout<<"This will take "<<lighter.energy<<" less energy than making a campfire without a lighter."<<endl;
		while(answer!="yes"&&answer!="no"){
			cout<<">>";
			getline(cin, answer);
			stringToLower(answer);
			if(answer=="yes"){
				usedLighter=1;
				findAndRemove(inventory, lighter.name);
				energy+=(lighter.energy+campfire.energy);
				cout<<"You used your lighter and you now have a campfire in your inventory."<<endl;
			}
			else if(answer=="no"){
				cout<<"You decided to not use your lighter."<<endl;
				cout<<"You now have a campfire in your inventory.\n"<<endl;
			}
			else{
				notValidAnswer();
			}
		}
	}
	else if(!inStringVector(inventory, "lighter")||usedLighter==0){
		cout<<"You used "<<-campfire.energy<<" energy to make a campfire."<<endl;
		energy+=campfire.energy;
		Sleep(500);
		cout<<"You now have a campfire in your inventory."<<endl;
	}
	inventory.push_back(campfire.name);
	Sleep(500);
}

void drinkWater(vector<string> &inventory, int &thirst, int &energy){
	/*
	If the player has water in their inventory, they are asked if they want to
	use it to gain thirst. If they don't have water in their inventory, they
	are asked if they want to gather water and function gatherWater is called.
	Inputs: Pass inventory, thirst, and energy by reference
	Outputs: Changes to inventory, thirst, and energy are made accordingly
	*/
	string choice="", answer="";
	
	cout<<"\nWould you like to drink water?"<<endl;
	while(answer!="yes"&&answer!="no"){
		cout<<">>";
		getline(cin, answer);
		stringToLower(answer);
		if(answer=="yes"){
			if(inStringVector(inventory, water.name)){
				cout<<"Would you like to use one of your water?"<<endl;
				while(choice!="yes"&&choice!="no"){
					cout<<">>";
					getline(cin, choice);
					stringToLower(choice);
					if(choice=="yes"){
						thirst+=water.thirst;
						adjustStat(thirst);
						findAndRemove(inventory, water.name); //removes one water from inventory
						cout<<"You have used one water and gained "<<water.thirst<<" thirst."<<endl;
						Sleep(2000);
					}
					else if(choice=="no"){
						cout<<"You decided to not use a water."<<endl;
						Sleep(2000);
					}
					else{
						notValidAnswer();
					}
				}
			}
			else{
				cout<<"You do not have any water in your inventory."<<endl;
				Sleep(500);
				cout<<"Would you like to gather some water?"<<endl;
				while(choice!="yes"&&choice!="no"){
					cout<<">>";
					getline(cin, choice);
					stringToLower(choice);
					if(choice=="yes"){
						gatherWater(thirst, energy);
					}
					else if(choice=="no"){
						cout<<"You decided not to gather water."<<endl;
					}
					else{
						notValidAnswer();
					}
				}
			}
		}
		else if(answer=="no"){
			cout<<"You decided not to drink water."<<endl;
		}
		else{
			notValidAnswer();
		}
	}
	cout<<endl;
	Sleep(2000);
}

void gatherWater(int &thirst, int &energy){
	/*
	If player chooses to gather water, a random amount is generated from 10 to 30
	on how much thirst they will gain and they will lose gatheredWater.energy energy.
	Inputs: Pass thirst and energy by reference
	Outputs: Changes stats accordingly
	*/
	gatheredWater.thirst=rand()%(30-10)+10; //random amount between 10 and 30
	thirst+=gatheredWater.thirst;
	energy+=gatheredWater.energy;
	cout<<"You gathered some water from a nearby pond."<<endl;
	Sleep(2000);
	cout<<"You have gained "<<gatheredWater.thirst<<" thirst and lost "<<-gatheredWater.energy<<" energy."<<endl;
	adjustStat(thirst);
}

void eatFood(vector<string> &inventory, int &hunger, int &thirst, int &energy){
	/*
	Tell user what food is in their inventory and asks what food they want to eat.
	If the food is raw and the user has a campfire, they are asked if they want to
	use their campfire to cook the food. Cooking the food doesn't allow the possibility
	of food poisoning. If the user does not have a campfire or they choose to not use their
	campfire and eat raw meat, there is a chance of catching food poisoning. The function
	hasPoisoning is used to determine if they got food poisoning.
	If the food isn't raw (coconuts, berries, canned beans), they are relatively safe.
	Except that berries can be poisonous. This is also calculated by function hasPoisoning.
	
	Inputs: Pass inventory and the stats by reference in order
	Outputs: Changes inventory and the stats accordingly.
	*/
	string choice="", answer="", delim="";
	bool hasFood=0, usedCampfire=0;
	
	for(string s: inventory){ //if player has food in their inventory
		if(inStringVector(allFoods, s)){
			hasFood=1;
			break;
		}
	}
	if(hasFood){
		cout<<"Would you like to eat your food?"<<endl;
		while(answer!="yes"&&answer!="no"){
			cout<<">>";
			getline(cin, answer);
			stringToLower(answer);
			if(answer=="yes"){
				cout<<"Food in your inventory: ";
				for(const auto &s: inventory){
					if(inStringVector(allFoods, s)){
						cout<<delim<<s;
						delim=", ";
					}
				}
				cout<<endl<<"What would you like to eat?"<<endl;
				while(!inStringVector(inventory, choice)||!inStringVector(allFoods, choice)){
					cout<<">>";
					getline(cin, choice);
					stringToLower(choice);
					if(inStringVector(inventory, choice)&&inStringVector(allFoods, choice)){
						if((choice==chicken.name||choice==fish.name||choice==boar.name)){ //if choice is raw meat
							if(inStringVector(inventory, campfire.name)){ //if player has campfire
								cout<<"Would you like to cook your "<<choice<<" with your campfire?"<<endl;
								while(answer!="yes"&&answer!="no"){
									cout<<">>";
									getline(cin, answer);
									stringToLower(answer);
									if(answer=="yes"){
										usedCampfire=1;
										findAndRemove(inventory, campfire.name); //removes campfire from inventory
										cout<<"You used your campfire to cook a "<<choice<<"."<<endl;
										if(choice==chicken.name){
											cout<<"You have gained "<<chicken.hunger<<" hunger."<<endl;
											hunger+=chicken.hunger;
										}
										else if(choice==fish.name){
											cout<<"You have gained "<<fish.hunger<<" hunger."<<endl;
											hunger+=fish.hunger;
										}
										else{
											cout<<"You have gained "<<boar.hunger<<" hunger."<<endl;
											hunger+=boar.hunger;
										}
									}
									else if(answer!="no"){
										notValidAnswer();
									}
								}
							}
							if(answer=="no"){
								cout<<"You have decided to not use your campfire."<<endl;
								Sleep(2000);
							}
							if(usedCampfire==0){
								if(hasPoisoning()){
									cout<<"You have contracted food poisoning and lost: "<<endl;
									Sleep(1000);
									cout<<-foodPoisoning.hunger<<" hunger, "<<endl;
									Sleep(1000);
									cout<<-foodPoisoning.thirst<<" thirst, "<<endl;
									Sleep(1000);
									cout<<"and "<<-foodPoisoning.energy<<" energy."<<endl;
									hunger+=foodPoisoning.hunger;
									thirst+=foodPoisoning.thirst;
									energy+=foodPoisoning.energy;
								}
								else{
									if(choice==chicken.name){
										cout<<"You have gained "<<chicken.hunger<<" hunger."<<endl;
										hunger+=chicken.hunger;
									}
										else if(choice==fish.name){
										cout<<"You have gained "<<fish.hunger<<" hunger."<<endl;
										hunger+=fish.hunger;
									}
									else{
										cout<<"You have gained "<<boar.hunger<<" hunger."<<endl;
										hunger+=boar.hunger;
									}
								}
							}	
						}
						else{ //if choice is berries, coconut, or beans
							if(choice==coconut.name){
								cout<<"You have gained "<<coconut.hunger<<" hunger."<<endl;
								hunger+=coconut.hunger;
							}
							else if(choice==berries.name){
								if(hasPoisoning()){
									cout<<"The berries you ate were poisonous. You lost :"<<endl;
									Sleep(1000);
									cout<<-foodPoisoning.hunger<<" hunger, "<<endl;
									Sleep(1000);
									cout<<-foodPoisoning.thirst<<" thirst, "<<endl;
									Sleep(1000);
									cout<<"and "<<-foodPoisoning.energy<<" energy."<<endl;
									hunger+=foodPoisoning.hunger;
									thirst+=foodPoisoning.thirst;
									energy+=foodPoisoning.energy;
								}
								else{
									cout<<"You have gained "<<berries.hunger<<" hunger."<<endl;
									hunger+=berries.hunger;
								}
							}
							else{
								cout<<"You have gained "<<cannedBeans.hunger<<" hunger."<<endl;
								hunger+=cannedBeans.hunger;
							}
						}
					}
					else{
						cout<<"That is not a valid choice."<<endl;
					}
				}
				findAndRemove(inventory, choice);
			}
			else if(answer=="no"){
				cout<<"You decided not to eat."<<endl;
			}
			else{
				notValidAnswer();
			}
		}
		cout<<endl;
		adjustStat(hunger);
		Sleep(1000);
	}
}

void gatherFood(vector<string> &inventory, int &hunger, int &thirst, int &energy){
	/*
	Asks the user if they want to gather food and the appropriate function calls are made.
	Inputs: Pass inventory and the stats by reference in order
	Outputs: Changes inventory and the stats accordingly
	*/
	char choice;
	int count=0;
	
	cout<<"What would you like to gather?"<<endl;
	Sleep(500);
	cout<<"a. Coconuts"<<endl;
	Sleep(500);
	cout<<"b. Berries"<<endl;
	Sleep(500);
	cout<<"c. Chicken"<<endl;
	Sleep(500);
	cout<<"d. Fish"<<endl;
	Sleep(500);
	cout<<"e. Boar"<<endl;
	Sleep(500);
			
	while(count==0){
		cout<<">>";
		cin>>choice;
		switch(choice){
			case 'a': gatheredCoconut(inventory, energy); break;
			case 'b': gatheredBerries(inventory, energy); break;
			case 'c': gatheredChicken(inventory, energy); break;
			case 'd': gatheredFish(inventory, energy); break;
			case 'e': gatheredBoar(inventory, energy); break;
			default: cout<<"That is not an option.\n"; --count; break;
		}
		++count;
		cin.ignore();
	}
	cout<<endl;
	Sleep(1000);
}

void gatheredCoconut(vector<string> &inventory, int &energy){
	/*
	If the player gathers a coconut, a coconut is added to their inventory and
	they lose energy.
	Inputs: Pass inventory and energy by reference.
	Outputs: Changes inventory and energy accordingly.
	*/
	inventory.push_back(coconut.name);
	cout<<"You used "<<-coconut.energy<<" energy to get a coconut."<<endl;
	energy+=coconut.energy;
}

void gatheredBerries(vector<string> &inventory, int &energy){
	/*
	If the player chooses to gather berries, they are added to their inventory and
	energy is lost.
	Inputs: Pass inventory and energy by reference.
	Outputs: Changes inventory and energy accordingly.
	*/
	inventory.push_back(berries.name);
	cout<<"You used "<<-berries.energy<<" energy to get berries."<<endl;
	energy+=berries.energy;
}

void gatheredChicken(vector<string> &inventory, int &energy){
	/*
	If the player chooses to catch a chicken, there is a 50% chance the chicken gets away and
	the player loses energy. If they catch the chicken, it is added to their inventory.
	Inputs: Pass inventory and energy by reference.
	Outputs: Changes inventory and energy accordingly.
	*/
	int gotChickenChance=1+rand()%2; //50% chance they don't catch a chicken
	if(gotChickenChance==1){
		inventory.push_back(chicken.name);
		cout<<"You used "<<-chicken.energy<<" energy to get a chicken."<<endl;
	}
	else{
		cout<<"You fought long and hard, but you didn't capture the chicken."<<endl;
		Sleep(2000);
		cout<<"You used "<<-chicken.energy<<" energy trying to get a chicken."<<endl;
	}
	energy+=chicken.energy;
	cout<<endl;
}

void gatheredFish(vector<string> &inventory, int &energy){
	/*
	If the player chooses to catch a fish, there is a 20% chance they get "attacked by a shark"
	and loose energy. If they get lucky, they catch a fish and fish.name is added to their inventory.
	Inputs: Pass inventory and energy by reference.
	Outputs: Changes inventory and energy accordingly.
	*/
	int sharkAttackChance=1+rand()%5; //20% chance of a shark attack
	if(sharkAttackChance==1){
		cout<<"While you went to gather fish, you were attacked by a shark."<<endl;
		cout<<"You lost "<<-shark.energy<<" energy and did not catch any fish."<<endl;
		energy+=shark.energy;
	}
	else{
		inventory.push_back(fish.name);
		cout<<"You used "<<-fish.energy<<" energy to catch a fish."<<endl;
		energy+=fish.energy;
	}
}

void gatheredBoar(vector<string> &inventory, int &energy){
	/*
	If the player chooses to catch a boar, there is a 1/3 chance they catch it. If they
	aren't lucky, they lose energy and come home empty handed.
	Inputs: Pass inventory and energy by reference
	Outputs: Changes energy and inventory accordingly.
	*/
	int gotBoarChance=1+rand()%3; //33% chance they get the boar
	if(gotBoarChance==1){
		inventory.push_back(boar.name);
		cout<<"You used "<<-boar.energy<<" energy to catch a boar."<<endl;
	}
	else{
		cout<<"You were badly injured trying to capture a boar."<<endl;
		Sleep(2000);
		cout<<"You have lost "<<-boar.energy<<" energy and did not catch a boar."<<endl;
	}
	energy+=boar.energy;
}

void useFAK(vector<string> &inventory, int &energy){
	/*
	Asks user if they want to use their first aid kit if they have one
	Inputs: Pass inventory by reference and energy stat by reference
	Outputs: If the player has a first aid kit and wants to use it, they gain energy.
	If the energy goes above 100, adjustStat sets it to 100.
	If the player doesn't have a first aid kit or chooses not to use their first aid kit,
	no energy is gained.
	*/
	string answer="";
	
	if(inStringVector(inventory, firstAidKit.name)){
		cout<<"Would you like to use your first aid kit?"<<endl;
		Sleep(500);
		cout<<"This will give you "<<firstAidKit.energy<<" energy."<<endl;
		Sleep(500);
		while(answer!="yes"&&answer!="no"){
			cout<<">>";
			getline(cin, answer);
			stringToLower(answer);
			if(answer=="yes"){
				cout<<"You used your first aid kit and gained "<<firstAidKit.energy<<" energy."<<endl;
				findAndRemove(inventory, firstAidKit.name);
				energy+=firstAidKit.energy;
			}
			else if(answer=="no"){
				cout<<"You did not use your first aid kit."<<endl;
			}
			else{
				notValidAnswer();
			}
		}
		cout<<endl;
	}
	adjustStat(energy);
	Sleep(1000);
}

//Weather
void weather(vector<string> &inventory, int &hunger, int &thirst, int &energy, bool &hasShelter){
	/*
	There is a 10% chance a disaster occurs and of that 10%, there is a 25% chance for each disaster. 
	Prints out that there has been a disaster and what disaster it is. If the disaster is hostile,
	The player loses their shelter if they have one and the player loses an item from their inventory.
	Parameters: Pass the disaster by referece, and pass hunger, thirst, 
	and energy by reference
	Outputs: Changes hunger, thirst, and energy accordingly.
	*/
	int c1=1+rand()%10, c2=(1+rand()%4)-1; //c2 used as index for weather vector
	vector<items> weatherVector {heatWave, tsunami, thunderStorm, windStorm};
	
	if(c1==1){
		cout<<"There has been a "<<weatherVector[c2].name<<"."<<endl;
		Sleep(2000);
		cout<<"You have lost: "<<endl;
		Sleep(500);
		cout<<-weatherVector[c2].hunger<<" hunger,"<<endl;
		Sleep(500);
		cout<<-weatherVector[c2].thirst<<" thirst, and"<<endl;
		Sleep(500);
		cout<<-weatherVector[c2].energy<<" energy."<<endl;
		Sleep(2000);
		if(hasShelter){
			cout<<"You shelter also has been destroyed."<<endl;
			hasShelter=0;
		}
		Sleep(2000);
		if(inventory.size()!=0){ //removes random item from inventory
			int r=(1+rand()%inventory.size())-1;
			cout<<"You have lost "<<inventory[r]<<" from your inventory."<<endl;
			findAndRemove(inventory, inventory[r]);
			Sleep(2000);
		}
		hunger+=weatherVector[c2].hunger;
		thirst+=weatherVector[c2].thirst;
		energy+=weatherVector[c2].energy;
		cout<<endl;
		Sleep(3000);
	}
}

//Illness
bool hasPoisoning(){
	/*
	Determines if a player gets food poisoning. There is a 33% chance they do
	Inputs: None
	Outputs: If the random number generated from 1 to 3 is equal to 1, true is returned.
	If not, false is returned.
	*/
	int chanceOfPoisoning=1+rand()%3;
	if(chanceOfPoisoning==1){
		return 1;
	}
	else{
		return 0;
	}
}

//Pure luck
bool rescuedEarly(vector<string> inventory, bool &early){
	/*
	Determines if the player gets rescued early
	Parameters: Inventory, which is checked if the player has a radio, which increases the chances to
	get rescued early, and boolean early, which if the user is returned early this
	becomes true. if not, it stays false.
	Outputs: 1 if the user got lucky and got rescued early, 0 if not
	*/
	int radioBonus=0;
	if(inStringVector(inventory, radio.name)){
		radioBonus=50;
	}
	int rescueEarlyChance=1+rand()%(100-radioBonus); //5% if no radioBonus, 10% if radioBonus
	if(rescueEarlyChance<=5){
		early=1;
		return 1;
	}
	else{
		return 0;
	}
}

void randomWashup(vector<string> &inventory){
	/*
	There is a 10% chance that something washes up to the shore (r1).
	If you get lucky, then it randomly gives you a supply from the plane (r2).
	If not, nothing happens.
	Inputs: The inventory vector
	Outputs: Addes the item to the inventory if the player gets lucky
	*/
	int r1=1+rand()%10, r2=1+rand()%5;
	string a[5]={water.name, firstAidKit.name, cannedBeans.name, lighter.name, rope.name};
	if(r1==1){
		cout<<"\nA "<<a[r2-1]<<" washed up to the shore!"<<endl;
		Sleep(1000);
		cout<<"It has been added to your inventory.\n"<<endl;
		inventory.push_back(a[r2-1]);
	}
	Sleep(3000);
}

//Other
void endOfDay(int &hunger, int &thirst, int &energy, bool hasShelter){
	/*
	At the end of each day, the player will lose a bit of hunger and thirst
	But gain a little energy, depending on if they have a shelter or not
	Parameters: Pass stats by reference and pass boolean hasShelter
	Outputs: Stats are affected. If player has a shelter, they will gain a bit
	more energy than if they didnt have a shelter
	*/
	cout<<"The sun sets and you rest."<<endl;
	sleep_.hunger=-(1+rand()%20); //lose a random amount of thirst and hunger (1-20)
	sleep_.thirst=-(1+rand()%20);
	hunger+=sleep_.hunger;
	thirst+=sleep_.thirst;
	if(hasShelter){
		energy+=(10+sleep_.energy); //if shelter, player gains 30 energy instead of 20
	}
	else{
		energy+=sleep_.energy;
	}
	adjustStat(energy);
	Sleep(5000);
}

bool isDead(int hunger, int thirst, int energy){
	/*
	Checks if any of the stats are less than or equal to 0
	Inputs: Stats in the correct order
	Outputs: True if any of the stats are less than or equal to 0,
	False if they are all greater than 0
	*/
	if((hunger<=0)||(thirst<=0)||(energy<=0)){
		return 1;
	}
	return 0;
}
