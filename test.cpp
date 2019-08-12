#include <iostream>
#include <stdlib.h>
#include "include/packet.cpp"
#include "include/sensors.cpp"
#include "include/md5.hpp"
#include "include/commands.cpp"

using namespace std;

namespace tests {
    bool checkPackets();
    bool checkSensors();
    bool testHash();
    bool testCommands();
}

int main(int argc, char** argv) {
        cout << "Starting tests..." << endl;
        
        cout << endl << "*****************************" << endl << endl;
         
        // Test hashing function
        auto hashState = tests::testHash();

        cout << "Hash state: " << (hashState ? "work" : "error") << endl;

        cout << endl << "*****************************" << endl << endl;

        // Checking packets
        auto packetsState = tests::checkPackets();
        
        cout << "Packets state: " << (packetsState ? "work" : "error") << endl;

        cout << endl << "*****************************" << endl << endl;

        // Checking sensors

        auto sensorsState = tests::checkSensors();
        
        cout << "Sensors state: " << (sensorsState ? "work" : "error") << endl;
        
        cout << endl << "*****************************" << endl << endl;

        // Test commands
        auto commandsState = tests::testCommands();

        cout << "Commands state: " << (commandsState ? "work" : "error") << endl; 

        cout << endl << "*****************************" << endl << endl;

        return 0;
    }

// Functions for testings
bool tests::checkPackets() {
    float temp = rand() % 300 / 10.0f;
    float press = rand() % 1000 + 99500.0f;
    float humidity = rand() % 1000 / 10.0f;
    float voltage = rand() % 10 / 5.0f + 3;

    // Check automatic id increase
    for (int index = 1; index < 100; index++) {
        Packet p1 = Packet(temp, press, voltage, humidity);
        if (index != p1.getId()) {
            return false;
        }
    }

    return true;
}

bool tests::checkSensors() {
    using namespace sensors;
    try {
        Sensors::initialize();
        Packet p1 = Sensors::getPacket();
        Packet* p2 = Sensors::getLastPacket();
        if (p1 != *p2) {
            return false;
        }
        for (int i = 0; i < 10; i++) {
            Packet temp = Sensors::getPacket();
            cout << "Packet #" << i << " :" << temp.toString() << endl;
        }
    } catch(exception e) {
        return false;
    }
    return true;
}

bool tests::testHash() {
    string strings[] = {
        "First", "Second", "Third", "Fourth", "Fifth"
    };
    string hashes[] = {
        "7fb55ed0b7a30342ba6da306428cae04", "c22cf8376b1893dcfcef0649fe1a7d87",
        "168909c0b6f1dfbd48f679d47059c1d6", "6e599f7a2a9186d391be4537f105be98",
        "0abdfd715970bd0ef7d5574daa0e6d0b"
    };

    string hashResult;
    string initialText;

    for (int i = 0; i < 5; i++) {
        initialText = strings[i];
        md5 h(strings[i].begin(), strings[i].end());
        hashResult = h.hex_digest<string>();
        cout << initialText << "|" << hashResult << endl;
        if (hashes[i] != hashResult) {
            return false;
        }
    }

    return true;
}

bool tests::testCommands() {
    // TODO: Write tests to check commands
    string messages[] =  {
        "1|c4ca4238a0b923820dcc509a6f75849b",
        "2|[doaspdash",
        "1|dsadsadas[pk",
        "flaslkjjkasn",
        "1|1|[pwiqehq",
        "12|c20ad4d76fe97759aa27a0c99bff6710"
    };
    commands::Statuses statuses[] = {
        commands::Statuses::OK,
        commands::Statuses::HASH_FAILED,
        commands::Statuses::HASH_FAILED, 
        commands::Statuses::HASH_FAILED, 
        commands::Statuses::HASH_FAILED,  
        commands::Statuses::NO_COMMAND
    };
    
    commands::Statuses result;
    for (int i = 0; i < sizeof(statuses) / sizeof(commands::Statuses); i++) {
        result = commands::CommandsInterface::execute(messages[i]);
        cout << "Result of executing ("<< messages[i] <<") is "
            << (int)result << " while should be " << (int)statuses[i] << endl;
        if (statuses[i] != result) {
            return false;
        }
    }

    return true;
}