#pragma once
#include <string>
#include <unordered_map>
#include <initializer_list>
#include "Item.h"
class Event
{
	//An event contains The Event Text Example: "You Discover a cave, do you go in?" Answers "YES","NO"
	//An Event also contains a list of branching events
	//Events can allow the player to recieve items,status effects,etc...
public:
	Event(std::string _EventName,std::string _EventText);
	std::string GetEventName() { return EventName; }
	std::string GetEventText() { return EventText; }
	Event* GetEventBranch(std::string Action) { return BranchingEvents[Action]; }
	void AddBranchingEvents(const std::initializer_list<std::pair<std::string,Event*>>& events);
	void AddRewardItems(const std::initializer_list<Item*>& items);
	int GetBranchCount();
	int GetRewardItemCount();
private:
	std::string EventName;
	//Text Describing the Event that is taking place
	std::string EventText;
	//Branching Events is a MAP where the Keys are actions
	std::unordered_map<std::string, Event*> BranchingEvents;
	std::vector<Item*> RewardItems;
};

