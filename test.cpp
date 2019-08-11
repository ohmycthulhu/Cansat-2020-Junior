#include <iostream>
#include "include/packet.cpp"
#include <stdlib.h>

using namespace std;

namespace tests {
    bool checkPackets();   
}

int main(int argc, char** argv) {
        cout << "Starting tests..." << endl;
        // Checking packets
        auto packetsState = tests::checkPackets();
        cout << "Packets state: " << (packetsState ? "work" : "error") << endl;

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