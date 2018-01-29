// mutedChatHelper.cpp : Defines the entry point for the DLL application.
//
// Copyright 2018 Joshua Bodine
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the
// Software without restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so, subject to the
// following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "bzfsAPI.h"
#include "plugin_utils.h"
#include <cstdlib>
#include <string>
#include <sstream>
#include <map>
#include <vector>

class mutedChatHelper : public bz_Plugin, public bz_CustomSlashCommandHandlerV2 {
protected:
	struct PendingMessage {
		int to;
		std::string message;
	};

	std::map<int, std::vector<PendingMessage> > pendingMessages;

	virtual void sendForcedTextMessage(int from, int to, const char* message);

public:
	virtual const char* Name() { return "Muted Chat Helper"; }
	virtual void Init(const char *config);
	virtual void Cleanup();

	virtual void Event(bz_EventData *eventData);
	virtual bool SlashCommand(int playerID, int sourceChannel, bz_ApiString command, bz_ApiString message, bz_APIStringList* params);
};

BZ_PLUGIN(mutedChatHelper)

void mutedChatHelper::sendForcedTextMessage(int from, int to, const char* message) {
	if(pendingMessages.find(from) == pendingMessages.end() && bz_hasPerm(from, "TALK") && bz_hasPerm(from, "PRIVATEMESSAGE")) {
		bz_sendTextMessage(from, to, message);

		return;
	}

	bz_grantPerm(from, "TALK");
	bz_grantPerm(from, "PRIVATEMESSAGE");

	bz_sendTextMessage(from, to, message);

	PendingMessage pendingMessage;

	pendingMessage.to = to;
	pendingMessage.message = message;

	pendingMessages[from].push_back(pendingMessage);
}

void mutedChatHelper::Init(const char* /*commandLine*/) {
	bz_debugMessage(4,"mutedChatHelper plugin loaded");

	Register(bz_ePlayerJoinEvent);
	Register(bz_eRawChatMessageEvent);
	Register(bz_eTickEvent);
	Register(bz_ePlayerPartEvent);

	bz_registerCustomSlashCommand("hello", this);
	bz_registerCustomSlashCommand("hi", this);
	bz_registerCustomSlashCommand("icanfm", this);
	bz_registerCustomSlashCommand("icanfunmatch", this);
	bz_registerCustomSlashCommand("icanoffi", this);
	bz_registerCustomSlashCommand("icanofficial", this);
	bz_registerCustomSlashCommand("icanmixedoffi", this);
	bz_registerCustomSlashCommand("icanmixedofficial", this);
	bz_registerCustomSlashCommand("icantmatch", this);
	bz_registerCustomSlashCommand("icannotmatch", this);
	bz_registerCustomSlashCommand("ours", this);
	bz_registerCustomSlashCommand("theirs", this);
	bz_registerCustomSlashCommand("atk", this);
	bz_registerCustomSlashCommand("attack", this);
	bz_registerCustomSlashCommand("def", this);
	bz_registerCustomSlashCommand("defend", this);
	bz_registerCustomSlashCommand("ok", this);
	bz_registerCustomSlashCommand("okay", this);
	bz_registerCustomSlashCommand("thanks", this);
	bz_registerCustomSlashCommand("gj", this);
	bz_registerCustomSlashCommand("gg", this);
}

void mutedChatHelper::Cleanup() {
	Flush();

	bz_removeCustomSlashCommand("hello");
	bz_removeCustomSlashCommand("hi");
	bz_removeCustomSlashCommand("icanfm");
	bz_removeCustomSlashCommand("icanfunmatch");
	bz_removeCustomSlashCommand("icanoffi");
	bz_removeCustomSlashCommand("icanofficial");
	bz_removeCustomSlashCommand("icanmixedoffi");
	bz_removeCustomSlashCommand("icanmixedofficial");
	bz_removeCustomSlashCommand("icantmatch");
	bz_removeCustomSlashCommand("icannotmatch");
	bz_removeCustomSlashCommand("ours");
	bz_removeCustomSlashCommand("theirs");
	bz_removeCustomSlashCommand("atk");
	bz_removeCustomSlashCommand("attack");
	bz_removeCustomSlashCommand("def");
	bz_removeCustomSlashCommand("defend");
	bz_removeCustomSlashCommand("ok");
	bz_removeCustomSlashCommand("okay");
	bz_removeCustomSlashCommand("thanks");
	bz_removeCustomSlashCommand("gj");
	bz_removeCustomSlashCommand("gg");

	Remove(bz_ePlayerJoinEvent);
	Remove(bz_eRawChatMessageEvent);
	Remove(bz_eTickEvent);
	Remove(bz_ePlayerPartEvent);

	bz_debugMessage(4,"mutedChatHelper plugin unloaded");
}

void mutedChatHelper::Event(bz_EventData *eventData) {
	if(eventData->eventType == bz_ePlayerJoinEvent) {
		bz_PlayerJoinPartEventData_V1 *joinEventData = (bz_PlayerJoinPartEventData_V1 *) eventData;

		if(bz_hasPerm(joinEventData->playerID, "SPAWN") && ! bz_hasPerm(joinEventData->playerID, "TALK") && ! bz_hasPerm(joinEventData->playerID, "PRIVATEMESSAGE")) {
			bz_sendTextMessage(BZ_SERVER, joinEventData->playerID, " ");
			bz_sendTextMessage(BZ_SERVER, joinEventData->playerID, "You are not permitted to chat due to being muted. However, you may use the following");
			bz_sendTextMessage(BZ_SERVER, joinEventData->playerID, "slash commands to send specific game-related messages:");
			bz_sendTextMessage(BZ_SERVER, joinEventData->playerID, " ");
			bz_sendTextMessage(BZ_SERVER, joinEventData->playerID, "/hello [player]            -  Hello[, player]");
			bz_sendTextMessage(BZ_SERVER, joinEventData->playerID, "/icanfm [duration]         -  I can play a [duration] fun match");
			bz_sendTextMessage(BZ_SERVER, joinEventData->playerID, "/icanoffi [duration]       -  I can play a [duration] official match");
			bz_sendTextMessage(BZ_SERVER, joinEventData->playerID, "/icanmixedoffi [duration]  -  I can play a [duration] mixed official match");
			bz_sendTextMessage(BZ_SERVER, joinEventData->playerID, "/icantmatch                -  I cannot match right now");
			bz_sendTextMessage(BZ_SERVER, joinEventData->playerID, "/ours                      -  Ours!");
			bz_sendTextMessage(BZ_SERVER, joinEventData->playerID, "/theirs                    -  Theirs!");
			bz_sendTextMessage(BZ_SERVER, joinEventData->playerID, "/atk                       -  Attack!");
			bz_sendTextMessage(BZ_SERVER, joinEventData->playerID, "/def                       -  Defend!");
			bz_sendTextMessage(BZ_SERVER, joinEventData->playerID, "/ok                        -  Okay");
			bz_sendTextMessage(BZ_SERVER, joinEventData->playerID, "/thanks                    -  Thanks");
			bz_sendTextMessage(BZ_SERVER, joinEventData->playerID, "/gj                        -  Good job");
			bz_sendTextMessage(BZ_SERVER, joinEventData->playerID, "/gg                        -  Good game");
		}
	} else if(eventData->eventType == bz_eRawChatMessageEvent) {
		bz_ChatEventData_V1 *chatEventData = (bz_ChatEventData_V1 *) eventData;

		if(pendingMessages.find(chatEventData->from) == pendingMessages.end())
			// player wasn't temporarily granted TALK/PRIVATEMESSAGE... allow it through
			return;

		for(size_t i = 0; i < pendingMessages[chatEventData->from].size(); ++i) {
			if(pendingMessages[chatEventData->from][i].message == chatEventData->message.c_str()) { // can't compare destinations due to two different standards for values
				pendingMessages[chatEventData->from].erase(pendingMessages[chatEventData->from].begin() + i);

				// the message matches a pending one... allow it through
				return;
			}
		}

		// otherwise, disallow the message
		chatEventData->message = "";
	} else if(eventData->eventType == bz_eTickEvent) {
		// revoke TALK and PRIVATEMESSAGE permissions for previously muted players whose messages have gone through
		std::map<int, std::vector<PendingMessage> >::iterator itr = pendingMessages.begin();

		while(itr != pendingMessages.end()) {
			if(itr->second.size() == 0) {
				bz_revokePerm(itr->first, "TALK");
				bz_revokePerm(itr->first, "PRIVATEMESSAGE");

				pendingMessages.erase(itr);
				itr = pendingMessages.begin();
			} else {
				++itr;
			}
		}
	} else if(eventData->eventType == bz_ePlayerPartEvent) {
		bz_PlayerJoinPartEventData_V1 *partEventData = (bz_PlayerJoinPartEventData_V1 *) eventData;

		if(pendingMessages.find(partEventData->playerID) != pendingMessages.end())
			// remove pending messages from this player slot
			pendingMessages.erase(pendingMessages.find(partEventData->playerID));
	}
}

bool mutedChatHelper::SlashCommand(int playerID, int sourceChannel, bz_ApiString command, bz_ApiString /* message */, bz_APIStringList* params) {
	if(command == "hello" || command == "hi") {
		if(params->size() == 0) {
			sendForcedTextMessage(playerID, sourceChannel, "Hello");
		} else if(params->size() == 1) {
			bz_BasePlayerRecord *playerRecord = bz_getPlayerBySlotOrCallsign(params->get(0).c_str());

			if(playerRecord != NULL) {
				sendForcedTextMessage(playerID, sourceChannel, (std::string("Hello, ") + playerRecord->callsign.c_str()).c_str());

				bz_freePlayerRecord(playerRecord);
			} else {
				bz_sendTextMessage(BZ_SERVER, playerID, "Player not found.");
			}
		} else {
			bz_sendTextMessage(BZ_SERVER, playerID, "Incorrect command parameters.");
		}

		return true;
	} else if(command == "icanfm" || command == "icanfunmatch") {
		if(params->size() == 0) {
			sendForcedTextMessage(playerID, sourceChannel, "I can play a fun match");
		} else if(params->size() == 1) {
			long int duration = strtol(params->get(0).c_str(), NULL, 10);

			if(duration == 15 || duration == 20 || duration == 30) {
				std::stringstream strStream;

				strStream << "I can play a " << duration << "-minute fun match";

				sendForcedTextMessage(playerID, sourceChannel, strStream.str().c_str());
			} else {
				bz_sendTextMessage(BZ_SERVER, playerID, "Incorrect match duration.");
			}
		} else {
			bz_sendTextMessage(BZ_SERVER, playerID, "Incorrect command parameters.");
		}

		return true;
	} else if(command == "icanoffi" || command == "icanofficial") {
		if(params->size() == 0) {
			sendForcedTextMessage(playerID, sourceChannel, "I can play an official match");
		} else if(params->size() == 1) {
			long int duration = strtol(params->get(0).c_str(), NULL, 10);

			if(duration == 15 || duration == 20 || duration == 30) {
				std::stringstream strStream;

				strStream << "I can play a " << duration << "-minute official match";

				sendForcedTextMessage(playerID, sourceChannel, strStream.str().c_str());
			} else {
				bz_sendTextMessage(BZ_SERVER, playerID, "Incorrect match duration.");
			}
		} else {
			bz_sendTextMessage(BZ_SERVER, playerID, "Incorrect command parameters.");
		}

		return true;
	} else if(command == "icanmixedoffi" || command == "icanmixedofficial") {
		if(params->size() == 0) {
			sendForcedTextMessage(playerID, sourceChannel, "I can play a mixed official match");
		} else if(params->size() == 1) {
			long int duration = strtol(params->get(0).c_str(), NULL, 10);

			if(duration == 15 || duration == 20 || duration == 30) {
				std::stringstream strStream;

				strStream << "I can play a " << duration << "-minute mixed official match";

				sendForcedTextMessage(playerID, sourceChannel, strStream.str().c_str());
			} else {
				bz_sendTextMessage(BZ_SERVER, playerID, "Incorrect match duration.");
			}
		} else {
			bz_sendTextMessage(BZ_SERVER, playerID, "Incorrect command parameters.");
		}

		return true;
	} else if(command == "icantmatch" || command == "icannotmatch") {
		if(params->size() == 0) {
			sendForcedTextMessage(playerID, sourceChannel, "I cannot match right now");
		} else {
			bz_sendTextMessage(BZ_SERVER, playerID, "Incorrect command parameters.");
		}

		return true;
	} else if(command == "ours") {
		if(params->size() == 0) {
			sendForcedTextMessage(playerID, sourceChannel, "Ours!");
		} else {
			bz_sendTextMessage(BZ_SERVER, playerID, "Incorrect command parameters.");
		}

		return true;
	} else if(command == "theirs") {
		if(params->size() == 0) {
			sendForcedTextMessage(playerID, sourceChannel, "Theirs!");
		} else {
			bz_sendTextMessage(BZ_SERVER, playerID, "Incorrect command parameters.");
		}

		return true;
	} else if(command == "atk" || command == "attack") {
		if(params->size() == 0) {
			sendForcedTextMessage(playerID, sourceChannel, "Attack!");
		} else {
			bz_sendTextMessage(BZ_SERVER, playerID, "Incorrect command parameters.");
		}

		return true;
	} else if(command == "def" || command == "defend") {
		if(params->size() == 0) {
			sendForcedTextMessage(playerID, sourceChannel, "Defend!");
		} else {
			bz_sendTextMessage(BZ_SERVER, playerID, "Incorrect command parameters.");
		}

		return true;
	} else if(command == "ok" || command == "okay") {
		if(params->size() == 0) {
			sendForcedTextMessage(playerID, sourceChannel, "Okay");
		} else {
			bz_sendTextMessage(BZ_SERVER, playerID, "Incorrect command parameters.");
		}

		return true;
	} else if(command == "thanks") {
		if(params->size() == 0) {
			sendForcedTextMessage(playerID, sourceChannel, "Thanks");
		} else {
			bz_sendTextMessage(BZ_SERVER, playerID, "Incorrect command parameters.");
		}

		return true;
	} else if(command == "gj") {
		if(params->size() == 0) {
			sendForcedTextMessage(playerID, sourceChannel, "Good job");
		} else {
			bz_sendTextMessage(BZ_SERVER, playerID, "Incorrect command parameters.");
		}

		return true;
	} else if(command == "gg") {
		if(params->size() == 0) {
			sendForcedTextMessage(playerID, sourceChannel, "Good game");
		} else {
			bz_sendTextMessage(BZ_SERVER, playerID, "Incorrect command parameters.");
		}

		return true;
	} else {
		return false;
	}
}

// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8
