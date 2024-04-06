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
struct items water {"water", 0, 30, 0};

struct items firstAidKit {"first aid kit", 0, 0, 50};

struct items cannedBeans {"canned beans", 0, 20, 0};

struct items lighter {"lighter", 0, 0, 10};

struct items rope {"rope", 0, 0, 20};

struct items radio {"radio", 0, 0, 0};

struct items campfire {"campfire", 0, 0, 15};

//illnesses and weather
struct items foodPoisoning {"food poisoning", -30, -30, -30, 1};

struct items rationing {"rationing", -20, -20, 5, 1};

struct items heatWave {"heat wave", -20, -40, -40, 1};

//fruits and animals
struct items coconut {"coconut", 5, 0, -5};

struct items berries {"berries", 2, 0, -2}; //chance to be poisonous?

struct items gatheredWater {"water", 0, rand()%(30-10)+10, -10};//gain random amount of thirst from 10 to 30

struct items fish {"fish", 20, 0 -20};

struct items chicken {"chicken", 15, 0, -15};

struct items boar {"boar", 40, 0, -30, 1};

struct items sleep {"sleep", -5, -10, 40};
//end of items

/*
Basic Functions
*/
void startingPrompt();

void choosingSupplies(char, int &, const int , vector<string> &, int &);

void viewStats(int, int, int, vector<string>);

void stringToLower(string &);

bool inStringVector(vector<string>, string);

bool inCharVector(vector<char>, char);

bool inIntVector(vector<int>, int);

void adjustStat(int &);

/*
Scenarios
Seperate by categories:
collecting supplies (shelter, fire, food, water, random washup), weather, illness
*/

//Collecting Supplies
void shelter(bool &, int &, vector<string> &);

//Weather
void weather(items, int &, int &, int &);

bool rescuedEarly(int radioBonus, bool &);

int main(){
	/*
	variables
	*/
	const int MAX_SUPPLY_COUNT=3;
	bool hasShelter=0, early=0;
	int hunger=0, thirst=0, energy=0, supplyCount=0, dayCount=0, radioBonus=0;
	int scenario;
	vector<string> inventory;
	char choice;
	vector<int> occuredScenarios; //if a scenario has occured, its corresponding integer is added
	
	
	startingPrompt();
	choosingSupplies(choice, supplyCount, MAX_SUPPLY_COUNT, inventory, radioBonus);
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
	viewStats(hunger, thirst, energy, inventory);
	
	while((hunger>0&&thirst>0&&energy>0)&&(dayCount<30)){ //loop ends when any of the stats go below 0 or survived 30 days
		dayCount++;
		cout<<"Day: "<<dayCount<<endl;
		Sleep(1000);
		
		if(dayCount>=15){ //chance to get rescued early
			if(rescuedEarly(radioBonus, early)){
				break;
			}
		}
		
		
		if(!hasShelter){ //if the user does not have shelter built
			shelter(hasShelter, energy, inventory);
		}
		
		// viewStats(hunger, thirst, energy, inventory);
		cout<<"\n------------------------------------------------------------------------------"<<endl;
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
	cout<<"a. Water"<<endl;
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

void choosingSupplies(char choice, int &supplyCount, const int MAX_SUPPLY_COUNT, vector<string> &inventory, int &radioBonus){
	while(supplyCount<MAX_SUPPLY_COUNT){
		cout<<">>";
		cin>>choice;
		switch(choice){
			case 'a': inventory.push_back("water"); cout<<"You chose water.\n"; break;
			case 'b': inventory.push_back("first aid kit"); cout<<"You chose first aid kit.\n"; break;
			case 'c': inventory.push_back("canned beans"); cout<<"You chose canned beans.\n"; break;
			case 'd': inventory.push_back("lighter"); cout<<"You chose a lighter.\n"; break;
			case 'e': inventory.push_back("rope"); cout<<"You chose rope.\n"; break;
			case 'f': inventory.push_back("radio"); cout<<"You chose a radio.\n"; radioBonus=50; break;
			default: cout<<"That is not an option.\n"; --supplyCount; break;
		}
		++supplyCount;
	}
}

void viewStats(int hunger, int thirst, int energy, vector<string> inventory){
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
			cout<<"None."<<endl;
	}
	else if(inventory.size()==1){
		cout<<inventory[0]<<endl;
	}
	else if(inventory.size()==2){
		cout<<inventory[0]<<" and "<<inventory[1]<<endl;
	}
	else{
		for(int i=0; i<inventory.size(); i++){
			
			if(i==inventory.size()-1){
				cout<<"and "<<inventory[i]<<flush;
			}
			else{
				cout<<inventory[i]<<", "<<flush;
			}
			Sleep(500);
		}
	}
	cout<<".\n"<<endl;
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
collecting supplies (shelter, fire, food, water, random washup), weather, illness
*/

//Collecting Supplies
void shelter(bool &hasShelter, int &energy, vector<string> &inventory){
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
		cin>>choice;
		stringToLower(choice);
		if(choice=="yes"){
			if(inStringVector(inventory, "rope")){ //if user has rope in their inventory
				choice="";
				cout<<"Would you like to use your rope to build the shelter?"<<endl;
				Sleep(1000);
				cout<<"This will take 20 less energy than building the shelter without a rope."<<endl;
				while(choice!="yes"&&choice!="no"){
					cout<<">>";
					cin>>choice;
					stringToLower(choice);
					if(choice=="yes"){
						energy+=rope.energy;
						inventory.erase(remove(inventory.begin(), inventory.end(), "rope"), inventory.end());
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
			energy-=40;
			cout<<"You have built a shelter to protect yourself."<<endl;
			hasShelter=1;
		}
		else if(choice=="no"){
			cout<<"You have decided to not build a shelter."<<endl;
			hasShelter=0;
		}
		else{
			cout<<"Not a valid answer. Please type 'yes' or 'no'."<<endl;
		}
	}
	Sleep(2000);
}

//Weather
void weather(items weather, int &hunger, int &thirst, int &energy){
	/*
	Prints out that there has been a disaster and what disaster it is
	Parameters: Pass the disaster by referece, and pass hunger, thirst, 
	and energy by reference
	Outputs: Changes hunger, thirst, and energy accordingly.
	*/
	cout<<"There has been a "<<weather.name<<endl;
	if(weather.isHostile){
		cout<<"You have lost: "<<endl;
	}
	else{
		cout<<"You have gained: "<<endl;
	}
	cout<<weather.hunger<<" hunger,"<<endl;
	cout<<weather.thirst<<" thirst, and"<<endl;
	cout<<weather.energy<<" energy."<<endl;
	hunger+=weather.hunger;
	thirst+=weather.thirst;
	energy+=weather.energy;
	Sleep(3000);
}

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
