#ifndef MQTTclient_h
#define MQTTclient_h

#include <Arduino.h>  
#include <Process.h>

#define MQTT_PUB "mosquitto_pub"
#define MQTT_SUB "mosquitto_sub"

#define MQTT_PARAM_HOST "-h"
#define MQTT_PARAM_TOPIC "-t"
#define MQTT_PARAM_MSG "-m"
#define MQTT_PARAM_VERBOSE "-v"
#define MQTT_PARAM_PORT "-p"

#define MAX_TOPICS 5

typedef void (*callbackFunction)(const String& topic, const String& subtopic, const String& message);

struct Callbacks {
    
    String listenTopic;
    callbackFunction callback;
    bool assigned;
    Process input;
    bool reading;
    
};

class MQTTclient {

	public:

		MQTTclient();

		void begin(const String& host, const unsigned short& port);
		void monitor();

		bool publish(const String& topic, const String& msg);
		bool publish(const String& topic, const int& msg);
		bool publish(const String& topic, const float& msg);

		void subscribe(const String& topic, callbackFunction callback);
		void unsubscribe(const String& topic);

	private:

		void read(Process& input, const String& listenTopic, callbackFunction callback, bool& reading);

		Callbacks _cbs[MAX_TOPICS];

		Process _output;
		String _host;
		String _port;

};

extern MQTTclient mqtt;

#endif
