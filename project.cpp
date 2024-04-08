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

struct items shelter {"shelter", 0, 0, 40};

//illnesses and weather
struct items foodPoisoning {"food poisoning", -30, -30, -30, 1};

struct items rationing {"rationing", -20, -20, 5, 1};

struct items heatWave {"heat wave", -20, -40, -40, 1};

struct items tsunami {"tsunami", 0, 0, -50, 1};

struct items thunderStorm {"thunder storm", 0, 0, -40, 1};

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

void choosingSupplies(vector<string> &, int &, int &, const int);

void viewStats(vector<string>, int, int, int);

void notValidAnswer();

void stringToLower(string &);

bool inStringVector(vector<string>, string);

void findAndRemove(vector<string> &, string);

bool inCharVector(vector<char>, char);

bool inIntVector(vector<int>, int);

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

void gatherWater(vector<string> &, int &, int &);

void eatFood(vector<string> &, int &, int &, int &);

void gatherFood(vector<string> &, int &, int &, int &);

void gatheredCoconut(vector<string> &, int &);

void gatheredBerries(vector<string> &, int &);

void gatheredChicken(vector<string> &, int &);

void gatheredFish(vector<string> &, int &);

void gatheredBoar(vector<string> &, int &);

//Weather
void weather(items, int &, int &, int &, bool &);

//Illness
bool hasPoisoning();

//Pure luck
bool rescuedEarly(int, bool &);

void randomWashup(vector<string> &);

//Other
void endOfDay(int &, int &, int &, bool);

vector<string> allFoods {cannedBeans.name, coconut.name, berries.name, fish.name, chicken.name, boar.name}; //global vector that stores the name of all the foods

int main(){
	/*
	variables
	*/
	const int MAX_SUPPLY_COUNT=3;
	bool hasShelter=0, early=0;
	int hunger=0, thirst=0, energy=0, supplyCount=0, dayCount=0, radioBonus=0;
	int scenario;
	vector<string> inventory;
	vector<int> occuredScenarios; //if a scenario has occured, its corresponding integer is added
	
	
	startingPrompt();
	choosingSupplies(inventory, supplyCount, radioBonus, MAX_SUPPLY_COUNT); //player chooses supplies
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
	
	while((hunger>0&&thirst>0&&energy>0)&&(dayCount<30)){ //loop ends when any of the stats go below 0 or survived 30 days
		dayCount++;
		cout<<"Day: "<<dayCount<<"."<<endl;
		Sleep(1000);
		
		if(dayCount>=15){ //chance to get rescued early. keep at top of while loop
			if(rescuedEarly(radioBonus, early)){
				break;
			}
		}
		
		viewStats(inventory, hunger, thirst, energy);
		
		if(!hasShelter){ //if the user does not have shelter built
			buildShelter(inventory, energy, hasShelter);
		}
		
		gatherSupplies(inventory, hunger, thirst, energy);
		
		randomWashup(inventory);
		
		eatFood(inventory, hunger, thirst, energy);
		
		endOfDay(hunger, thirst, energy, hasShelter);
		// viewStats(hunger, thirst, energy, inventory);
		cout<<endl<<"------------------------------------------------------------------------------"<<endl;
	}//end of while loop
	
	
	
	/*
	Final Messages
	*/
	if(dayCount==30){
		cout<<"You did it!"<<endl;
		Sleep(4000);
		cout<<"You were rescued and they made a Hollywood movie about you starring Tom Hanks.\n"<<flush;
		Sleep(4000);
		cout<<"Good job!"<<endl;
	}
	else if(early){
		cout<<"You were rescued early on day "<<dayCount<<"!"<<endl;
		Sleep(4000);
		cout<<"You were brought to home safely and you were later interviewed by international"<<endl;
		cout<<"news networks and you published a book about your story."<<endl;
		Sleep(5000);
		cout<<"Good job!"<<endl;
	}
	else{
		if(hunger<=0){
			cout<<"You got too hungry and died on the island on day "<<dayCount<<endl;
			Sleep(3000);
			cout<<"Better luck next time!\n";
		}
		else if(thirst<=0){
			cout<<"You got too thirsty and died on the island on day "<<dayCount<<endl;
			Sleep(3000);
			cout<<"Better luck next time!\n";
		}
		else{
			cout<<"You ran out of energy and died on the island on day "<<dayCount<<endl;
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

void choosingSupplies(vector<string> &inventory, int &supplyCount, int &radioBonus, const int MAX_SUPPLY_COUNT){
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
			case 'f': inventory.push_back(radio.name); cout<<"You chose a radio.\n"; radioBonus=50; break;
			default: cout<<"That is not an option.\n"; --supplyCount; break;
		}
		++supplyCount;
	}
	cin.ignore();
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

bool inCharVector(vector<char> v, char c){
	/*
	Checks if a character is in an character vector.
	Parameters: A character vector and a character
	Outputs: If the character is in the vector, 1 is returned. If not, 0 is returned.
	*/
	for(char k: v){
		if(k==c){
			return 1;
		}
	}
	return 0;
}

bool inIntVector(vector<int> v, int x){
	/*
	Checks if an integer is in an integer vector.
	Parameters: An integer vector and an integer
	Outputs: If the integer is in the vector, 1 is returned. If not, 0 is returned.
	*/
	for(int n: v){
		if(n==x){
			return 1;
		}
	}
	return 0;
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
	
	cout<<"Would you like to build a shelter? This will cost 40 energy."<<endl;
	while(choice!="yes"&&choice!="no"){
		cout<<">>";
		getline(cin, choice);
		stringToLower(choice);
		if(choice=="yes"){
			if(inStringVector(inventory, rope.name)){ //if user has rope in their inventory
				choice="";
				cout<<"Would you like to use your rope to build the shelter?"<<endl;
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
			energy-=shelter.energy;
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
					case 'b': gatherWater(inventory, thirst, energy); break;
					case 'c': makeCampfire(inventory, energy); break;
					case 'd': cout<<"You decided to not gather any supplies.\n"<<endl; break;
					default: cout<<"That is not an option."<<endl; --count; break;
				}
				++count;
			}
			cin.ignore();
		}
		else if(answer=="no"){
			cout<<"You decided to not gather any supplies.\n"<<endl;
		}
		else{
			notValidAnswer();
		}
	}
	Sleep(2000);
}

void makeCampfire(vector<string> &inventory, int &energy){
	string answer="";
	bool usedLighter=0;
	cin.ignore(1000, '\n');
	
	if(inStringVector(inventory, "lighter")){
		cout<<"Would you like to use your lighter to make a campfire?"<<endl;
		cout<<"This will take "<<lighter.energy<<" less energy than making a campfire without a lighter."<<endl;
		while(answer!="yes"&&answer!="no"){
			cout<<">>";
			getline(cin, answer);
			stringToLower(answer);
			if(answer=="yes"){
				usedLighter=1;
				findAndRemove(inventory, "lighter");
				energy+=lighter.energy;
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
		cout<<"You now have a campfire in your inventory.\n"<<endl;
		inventory.push_back("campfire");
	}
}

void drinkWater(vector<string> &inventory, int &thirst, int &energy){ //change to drink water. add gatherWater
	string choice="";
	cin.ignore(1000, '\n');
	
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
		cout<<"Would you like to gather some?"<<endl;
		while(choice!="yes"&&choice!="no"){
			cout<<">>";
			cin>>choice;
			stringToLower(choice);
			if(choice=="yes"){
				gatherWater(inventory, thirst, energy);
			}
			else if(choice=="no"){
				cout<<"You decided not to gather water."<<endl;
			}
			else{
				cout<<"Not a valid answer. Please type 'yes' or 'no'."<<endl;
			}
		}
	}
	cout<<endl;
	Sleep(2000);
}

void gatherWater(vector<string> &inventory, int &thirst, int &energy){
	gatheredWater.thirst=rand()%(30-10)+10; //random amount between 10 and 30
	thirst+=gatheredWater.thirst;
	energy+=gatheredWater.energy;
	cout<<"You gathered some water from a nearby pond."<<endl;
	Sleep(2000);
	cout<<"You have gained "<<gatheredWater.thirst<<" thirst and lost "<<-gatheredWater.energy<<" energy.\n"<<endl;
	adjustStat(thirst);
}

void eatFood(vector<string> &inventory, int &hunger, int &thirst, int &energy){
	string choice="", answer="", delim="";
	bool hasFood=0;
	
	cout<<"Food in your inventory: ";
	for(const auto &s: inventory){
		if(inStringVector(allFoods, s)){
			cout<<delim<<s;
			delim=", ";
			hasFood=1;
		}
	}
	if(hasFood==0){
		cout<<"none."<<endl;
		cout<<"You cannot eat anything."<<endl;
	}
	else{
		cout<<endl<<"What would you like to eat?"<<endl;
		while(!inStringVector(inventory, choice)||!inStringVector(allFoods, choice)){
			cout<<">>";
			getline(cin, choice);
			stringToLower(choice);
			if(inStringVector(inventory, choice)&&inStringVector(allFoods, choice)){
				if((choice==chicken.name||choice==fish.name||choice==boar.name)){ //if choice is raw meat
					if(inStringVector(inventory, campfire.name)){ //if player has campfire
						cout<<"Would you like to cook your "<<choice<<"?"<<endl;
						while(answer!="yes"&&answer!="no"){
							cout<<">>";
							getline(cin, answer);
							stringToLower(answer);
							if(answer=="yes"){
								findAndRemove(inventory, campfire.name);
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
					else if(answer=="no"){
						cout<<"You have decided to not use your campfire."<<endl;
					}
					if(hasPoisoning()){
						cout<<"You have contracted food poisoning and lost: "<<endl;
						Sleep(500);
						cout<<-foodPoisoning.hunger<<" hunger, "<<endl;
						Sleep(500);
						cout<<-foodPoisoning.thirst<<" thirst, "<<endl;
						Sleep(500);
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
				else{ //if choice is berries, coconut, or beans
					if(choice==coconut.name){
						cout<<"You have gained "<<coconut.hunger<<" hunger."<<endl;
						hunger+=coconut.hunger;
					}
					else if(choice==berries.name){
						if(hasPoisoning()){
							cout<<"The berries you ate were poisonous. You lost :"<<endl;
							Sleep(500);
							cout<<-foodPoisoning.hunger<<" hunger, "<<endl;
							Sleep(500);
							cout<<-foodPoisoning.thirst<<" thirst, "<<endl;
							Sleep(500);
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
	adjustStat(hunger);
	cout<<endl;
	Sleep(500);
}

void gatherFood(vector<string> &inventory, int &hunger, int &thirst, int &energy){
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
	}
	cout<<endl;
	Sleep(1000);
}

void gatheredCoconut(vector<string> &inventory, int &energy){
	inventory.push_back(coconut.name);
	cout<<"You used "<<-coconut.energy<<" energy to get a coconut."<<endl;
	energy+=coconut.energy;
}

void gatheredBerries(vector<string> &inventory, int &energy){
	inventory.push_back(berries.name);
	cout<<"You used "<<-berries.energy<<" energy to get berries."<<endl;
	energy+=berries.energy;
}

void gatheredChicken(vector<string> &inventory, int &energy){
	int gotChickenChance=1+rand()%2; //50% chance they don't catch a chicken
	if(gotChickenChance==1){
		inventory.push_back(chicken.name);
		cout<<"You used "<<-chicken.energy<<" energy to get a chicken."<<endl;
		energy+=chicken.energy;
	}
	else{
		cout<<"You fought long and hard, but you didn't capture the chicken."<<endl;
		Sleep(2000);
		cout<<"You used "<<-chicken.energy<<" energy to get a chicken."<<endl;
		energy+=chicken.energy;
	}
}

void gatheredFish(vector<string> &inventory, int &energy){
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
	int gotBoarChance=1+rand()%3; //33% chance they get the boar
	if(gotBoarChance==1){
		inventory.push_back(boar.name);
		cout<<"You used "<<-boar.energy<<" energy to catch a boar."<<endl;
		energy+=boar.energy;
	}
	else{
		cout<<"You were badly injured trying to capture a boar."<<endl;
		cout<<"You have lost "<<-boar.energy<<" energy and did not catch a boar"<<endl;
		energy+=boar.energy;
	}
}

//Weather
void weather(items weather, int &hunger, int &thirst, int &energy, bool &hasShelter){
	/*
	Prints out that there has been a disaster and what disaster it is
	Parameters: Pass the disaster by referece, and pass hunger, thirst, 
	and energy by reference
	Outputs: Changes hunger, thirst, and energy accordingly.
	*/
	cout<<"There has been a "<<weather.name<<endl;
	Sleep(2000);
	if(weather.isHostile){
		cout<<"You have lost: "<<endl;
		Sleep(500);
		cout<<-weather.hunger<<" hunger,"<<endl;
		Sleep(500);
		cout<<-weather.thirst<<" thirst, and"<<endl;
		Sleep(500);
		cout<<-weather.energy<<" energy."<<endl;
		Sleep(2000);
		cout<<"You shelter also has been destroyed."<<endl;
		hasShelter=0;
	}
	else{
		cout<<"You have gained: "<<endl;
		Sleep(500);
		cout<<weather.hunger<<" hunger,"<<endl;
		Sleep(500);
		cout<<weather.thirst<<" thirst, and"<<endl;
		Sleep(500);
		cout<<weather.energy<<" energy."<<endl;
	}
	hunger+=weather.hunger;
	thirst+=weather.thirst;
	energy+=weather.energy;
	cout<<endl;
	Sleep(3000);
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
bool rescuedEarly(int radioBonus, bool &early){
	/*
	Determines if the player gets rescued early
	Parameters: The bonus for choosing the radio, which increases the chances to
	get rescued early, and boolean early, which if the user is returned early this
	becomes true. if not, it stays false.
	Outputs: 1 if the user got lucky and got rescued early, 0 if not
	*/
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
	There is a ten percent chance that something washes up to the shore (r1).
	If you get lucky, then it randomly gives you a supply from the plane (r2).
	If not, nothing happens.
	Inputs: The inventory vector
	Outputs: Addes the item to the inventory if the player gets lucky
	*/
	int r1=1+rand()%10, r2=1+rand()%5;
	string a[5]={water.name, firstAidKit.name, cannedBeans.name, lighter.name, rope.name};
	if(r1==1){
		cout<<"A "<<a[r2-1]<<" washed up to the shore!"<<endl<<endl;
		Sleep(3000);
		inventory.push_back(a[r2-1]);
	}
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
		energy+=(sleep_.energy+10); //if shelter, player gains 30 energy instead of 20
	}
	else{
		energy+=sleep_.energy;
	}
	adjustStat(energy);
	Sleep(5000);
}
