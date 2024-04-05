#include <cstdlib> //rand()
#include <cstring> //string methods
#include <ctime> //time(NULL)
#include <iostream>
#include <string>
#include <vector>
#include <windows.h> //Sleep()
using namespace std;

void startingPrompt(){ //if you add supplies make sure you add to switch statement
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

void wait(char c){
	/*
	Used to give player time to read the text. The player enters anything when they are ready
	to continue.
	Parameters: A character variable
	Output: Nothing
	*/
	cout<<"Enter anything to continue: ";
	if(!(cin>>c)){
		cin.clear();
		cin.ignore('\n');
	}
	cout<<endl;
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

/*
Scenarios
*/
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

void shelter(bool &hasShelter, int &energy){
	/*
	Asks user if they want to build a shelter.
	Parameters: pass boolean hasShelter and int energy by reference
	Outputs: none; only changes value of hasShelter and energy
	*/
	string choice="";
	cout<<"Would you like to build a shelter? This will cost 40 energy."<<endl;
	while(choice!="yes"&&choice!="no"){
		cin>>choice;
		stringToLower(choice);
		if(choice=="yes"){
			hasShelter=1;
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
}

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
};

int main(){
	//supplies and non-food island items
	items water;
	water.name="water";
	water.thirst=30;
	
	items firstAidKit;
	firstAidKit.name="first aid kit";
	firstAidKit.energy=50;
	
	items cannedBeans;
	cannedBeans.name="canned beans";
	cannedBeans.hunger=20;
	
	items campfire;
	campfire.name="campfire";
	campfire.energy=15;
	
	
	//illnesses and weather
	items foodPoisoning;
	foodPoisoning.name="food poisoning";
	foodPoisoning.hunger=-30;
	foodPoisoning.thirst=-30;
	foodPoisoning.energy=-30;
	
	items rationing;
	rationing.name="rationing";
	rationing.hunger=-20;
	rationing.thirst=-20;
	rationing.energy=5;
	
	//fruits and animals
	items coconut;
	coconut.name="coconut";
	coconut.hunger=5; //gains hunger when obtained
	coconut.energy=-5; //loses energy to get
	
	items berries; //chance to be poisonous?
	berries.name="berries";
	berries.hunger=2;
	berries.energy=-2;
	
	items gatheredWater;
	gatheredWater.name="water";
	gatheredWater.thirst=rand()%(30-10)+10; //gain random amount of thirst from 10 to 30
	gatheredWater.energy=10;
	
	items fish;
	fish.name="fish";
	fish.hunger=20;
	fish.energy=-20;
	
	items chicken;
	chicken.name="chicken";
	chicken.hunger=15;
	chicken.energy=-15;
	
	items boar;
	boar.name="boar";
	boar.hunger=40;
	boar.energy=-30;
	
	items sleep;
	sleep.name="sleep";
	sleep.hunger=-5;
	sleep.thirst=-10;
	sleep.energy=40;
	//end of items
	
	/*
	variables
	*/
	const int MAX_SUPPLY_COUNT=3;
	bool hasShelter=0;
	int hunger=0, thirst=0, energy=0, supplyCount=0, dayCount=0, rescueEarlyChance, radioBonus=0;
	int scenario;
	vector<string> inventory;
	char choice;
	vector<int> occuredScenarios; //if a scenario has occured, its corresponding integer is added
	
	
	startingPrompt();
	
	while(supplyCount<MAX_SUPPLY_COUNT){
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
			rescueEarlyChance=1+rand()%(100-radioBonus); //if chose radio 10%, else 5%
			if(rescueEarlyChance<=5){
				rescueEarlyChance=0;
				break;
			}
		}
		
		
		if(!hasShelter){ //if the user does not have shelter built
			shelter(hasShelter, energy);
		}
		
		// viewStats(hunger, thirst, energy, inventory);
		cout<<"\n------------------------------------------------------------------------------"<<endl;
	}//end of while loop
	
	
	if(dayCount==30){
		cout<<"You did it!"<<endl;
		Sleep(4000);
		cout<<"You were rescued and they made a Hollywood movie about you starring Tom Hanks.\n"<<flush;
		Sleep(4000);
		cout<<"Good job!"<<endl;
	}
	else if(rescueEarlyChance==0){
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