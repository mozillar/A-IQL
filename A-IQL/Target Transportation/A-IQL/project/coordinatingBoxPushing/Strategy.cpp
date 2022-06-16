#include "Strategy.h"
#include "GlobalVariable.h"
Strategy::Strategy()
{
	SetStateCount(MAX_STATE_COUNT);
	SetActionCount(MAX_ACTION_COUNT);
}
int Strategy::GetStateCount()
{
	return state_count;
}
void Strategy::SetStateCount(int formal_state_count)
{
	state_count = formal_state_count;
}
int Strategy::GetActionCount()
{
	return action_count;
}
void Strategy::SetActionCount(int formal_action_count)
{
	action_count = formal_action_count;
}
