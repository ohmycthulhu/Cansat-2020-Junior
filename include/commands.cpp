#ifndef __COMMANDS__
#define __COMMANDS__
#include "commands.hpp"
#include "md5.hpp"
#if IS_NOT_CONTROLLER
#include <iostream>
#endif

namespace commands {
    Statuses CommandsInterface::execute(const STRING_TYPE& s) {
        if (!isHashValid(s)) {
            return Statuses::HASH_FAILED;
        }
        Commands c = extractCommand(s);
        return execute(c);
    }
    Statuses CommandsInterface::execute(const Packet& s) {
        return Statuses::NO_COMMAND; // TODO: Write conditions for commands
    }
    Statuses CommandsInterface::execute(const Commands& state) {
        switch (state)
        {
        case Commands::DETACH:
            detach();
            return Statuses::OK;

        default:
            return Statuses::NO_COMMAND;
        }
    }
    bool CommandsInterface::isHashValid(const STRING_TYPE& s) {
    /*
        Command string is structured like that: 
        command_id|payload|hash
        For checking hash, we're dividing message by last delimiter (|), and getting command_id|payload and hash
        Then we are calculating hash of first and checking if hashes are equal
    */
    #if IS_NOT_CONTROLLER
        auto lastDelimiterIndex = s.find_last_of(delimiter);
        auto hash = s.substr(lastDelimiterIndex + 1, s.size());
        auto body = s.substr(0, lastDelimiterIndex);
        md5 hashGenerator(body.begin(), body.end());
        return hashGenerator.hex_digest<STRING_TYPE>() == hash;
    #else

    #endif
    
    }
    Commands CommandsInterface::extractCommand (const STRING_TYPE& s) {
    /*
        Command string is structured like that: 
        command_id|payload|hash
        For extracting command, we're dividing message by first delimiter (|)
        and checking string part before first delimiter
    */
   #if IS_NOT_CONTROLLER
        auto firstDelimiterIndex = s.find(delimiter);
        STRING_TYPE commandId = s.substr(0, firstDelimiterIndex);
        int i = atoi(commandId.c_str());
    #else
        // TODO: Write this part for Arduino
    #endif
        if (i & (int)Commands::DETACH) {
            return Commands::DETACH;
        }
        return Commands::UNDEFINED;
    }

    void CommandsInterface::detach() {
    #if IS_CONTROLLER
        // TODO: Write here actions for detaching
    #else
        std::cout << "Detached" << std::endl;
    #endif
    }

}
#endif