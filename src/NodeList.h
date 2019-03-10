// NodeList.h

#ifndef _NODELIST_h
#define _NODELIST_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

enum node_status {
    UNREGISTERED,
    INIT,
    WAIT_FOR_SERVER_HELLO,
    WAIT_FOR_KEY_EXCH_FINISHED,
    WAIT_FOR_CIPHER_FINISHED,
    WAIT_FOR_DOWNLINK,
    REGISTERED,
    SLEEP
};

typedef enum node_status status_t;

struct node_instance {
    uint8_t mac[6];
    uint16_t nodeId;
    uint8_t key[32];
    uint16_t lastMessageCounter;
    time_t keyValidFrom;
    time_t lastMessageTime;
    status_t status = UNREGISTERED;
    bool keyValid = false;
    bool sleepyNode = true;
};

typedef struct node_instance node_t;

class Node {
public:
    Node ();
    Node (node_t nodeData);
    uint8_t *getMacAddress () {
        return mac;
    }
    uint16_t getNodeId () {
        return nodeId;
    }
    void setNodeId (uint16_t nodeId) {
        this->nodeId = nodeId;
    }
    uint8_t *getEncriptionKey () {
        return key;
    }
    void setEncryptionKey (const uint8_t* key);

    time_t getKeyValidFrom () {
        return keyValidFrom;
    }
    void setKeyValidFrom (time_t keyValidFrom) {
        this->keyValidFrom = keyValidFrom;
    }
    time_t getLastMessageTime () {
        return lastMessageTime;
    }
    void setLastMessageTime () {
        lastMessageTime = millis ();
    }
    uint16_t getLastMessageCounter () {
        return lastMessageCounter;
    }
    void setLastMessageCounter (uint16_t counter) {
        lastMessageCounter = counter;
    }

    void setMacAddress (uint8_t *macAddress) {
        if (macAddress) {
            memcpy (mac, macAddress, 6);
        }
    }

    bool isKeyValid () {
        return keyValid;
    }
    void setKeyValid (bool status) {
        keyValid = status;
    }
    bool isRegistered () {
        return status == REGISTERED;
    }
    status_t getStatus () {
        return status;
    }
    void setStatus (status_t status) {
        this->status = status;
    }
    node_t getNodeData ();

    void printToSerial (Stream *port);

    void reset ();

    void setSleepy (bool sleepy) {
        sleepyNode = sleepy;
    }

    bool getSleepy () {
        return sleepyNode;
    }

protected:
#define KEYLENGTH 32
    uint8_t mac[6];
    uint8_t key[KEYLENGTH];
    uint16_t lastMessageCounter;
    uint16_t nodeId;
    timer_t keyValidFrom;
    bool keyValid;
    status_t status = UNREGISTERED;
    timer_t lastMessageTime;
    bool sleepyNode = true;

    friend class NodeList;
};



class NodeList {
#define NUM_NODES 20
public:
    NodeList ();

    Node *getNodeFromID (uint16_t nodeId);

    Node *getNodeFromMAC (const uint8_t* mac);
    
    Node *findEmptyNode ();
    
    uint16_t countActiveNodes ();
    
    bool unregisterNode (uint16_t nodeId);
    
    bool unregisterNode (const uint8_t* mac);
    
    bool unregisterNode (Node *node);

    Node *getNextActiveNode (uint16_t nodeId);

    Node *getNextActiveNode (Node node);

    Node *getNewNode (const uint8_t* mac);

    void printToSerial (Stream *port);

protected:
    Node nodes[NUM_NODES];

};


#endif

