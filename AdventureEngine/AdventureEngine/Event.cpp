#include "Event.h"
#include <iostream>

Event::Event(std::string _EventName,std::string _EventText) : EventName(_EventName), EventText(_EventText) 
{
	//???
}
void Event::AddBranchingEvents(const std::initializer_list<std::pair<std::string, Event*>>& events)
{
	for (auto& eventPair : events)
	{
		if (BranchingEvents.find(eventPair.first) == BranchingEvents.end())
		{
			BranchingEvents[eventPair.first] = eventPair.second;
		}
		else
		{
			std::cout << "Event Already Exists : " << eventPair.second->GetEventName() << std::endl;
		}
	}

	//Example:  
		//cave.AddBranchingEvents({
		//{ "YES", & yesBranch },
		//{ "NO", &noBranch }
		//});
}
void Event::AddRewardItems(const std::initializer_list<Item*>& items) {
	for (Item* item : items) {
		// Check if the item already exists in RewardItems
		if (std::find(RewardItems.begin(), RewardItems.end(), item) == RewardItems.end()) {
			// Item does not exist, add it to the list
			RewardItems.push_back(item);
		}
		else {
			// Item already exists, print a message
			std::cout << "Item Already Exists: " << item->GetName() << std::endl;
		}
	}
}

int Event::GetBranchCount()
{
	if (!BranchingEvents.empty())
	{
		return BranchingEvents.size() + 1;
	}
	else
	{
		return BranchingEvents.size();
	}
}

int Event::GetRewardItemCount()
{ 
	if (!RewardItems.empty())
	{
		return RewardItems.size() + 1;
	}
	else
	{
		return RewardItems.size();
	}
	 
}
