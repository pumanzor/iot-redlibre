#include "MQTTclient.h"

MQTTclient::MQTTclient() { }

void MQTTclient::begin(const String& host, const unsigned short& port) {
	_host = host;
	_port = String(port);
}

void MQTTclient::monitor() {

	for(int i = 0; i < MAX_TOPICS; i++) {
		if (_cbs[i].assigned) {
			if (_cbs[i].input.available() > 0 && !_cbs[i].reading) {
				read(_cbs[i].input, _cbs[i].listenTopic, _cbs[i].callback, _cbs[i].reading);
			}
		}
	}

}

bool MQTTclient::publish(const String& topic, const int& msg) {
	return publish(topic, String(msg));
}

bool MQTTclient::publish(const String& topic, const float& msg) {
	return publish(topic, String(msg));
}

bool MQTTclient::publish(const String& topic, const String& msg) {

	_output.begin(MQTT_PUB);
    _output.addParameter(MQTT_PARAM_HOST);
    _output.addParameter(_host);
    _output.addParameter(MQTT_PARAM_PORT);
    _output.addParameter(_port);
    _output.addParameter(MQTT_PARAM_TOPIC);
    _output.addParameter(topic);
    _output.addParameter(MQTT_PARAM_MSG);
    _output.addParameter(msg);
    _output.run();

    if(_output.exitValue() == 0) {
    	return true;
    }
    else {
    	return false;
    }

}

void MQTTclient::subscribe(const String& listenTopic, callbackFunction callback) {

	bool assigned = false;
	char i = 0;

	while (!assigned) {
        if(!_cbs[i].assigned) {
            _cbs[i].listenTopic = listenTopic;
            _cbs[i].callback = callback;
            _cbs[i].assigned = true;
            _cbs[i].reading = false;

            _cbs[i].input.begin(MQTT_SUB);	
    		_cbs[i].input.addParameter(MQTT_PARAM_HOST);
    		_cbs[i].input.addParameter(_host);
    		_cbs[i].input.addParameter(MQTT_PARAM_PORT);
    		_cbs[i].input.addParameter(_port);
    		_cbs[i].input.addParameter(MQTT_PARAM_TOPIC);
    		_cbs[i].input.addParameter(listenTopic);
    		_cbs[i].input.addParameter(MQTT_PARAM_VERBOSE); 
    		_cbs[i].input.runAsynchronously();	

            assigned = true;

        }
        i++;
    }

}

void MQTTclient::unsubscribe(const String& topic) {

	for(int i = 0; i < MAX_TOPICS; i++) {
		if (_cbs[i].assigned) {
			if (_cbs[i].listenTopic == topic) {
				_cbs[i].input.close();
				_cbs[i].assigned = false;
				_cbs[i].listenTopic = "";
			}
		}
	}

}

void MQTTclient::read(Process& input, const String& listenTopic, callbackFunction callback, bool& reading) {

	String inc;
	String topic;
	String msg;
	char inchar;
	bool tread = false;
	bool done = false;



	while(input.available() > 0 && (inchar = input.read()) != '\n') {

		if (inchar != '\r' && inchar != -1) {
		    if(inchar == ' ' && !tread) {
		       	topic = inc;
		       	inc = "";
		       	tread = true;
		    }
		    else {
		      	if(tread) {
		          	msg += inchar; 
		          	inc = "";
		          	done = true;
		      	}
		      	else {
		          	inc += inchar; 
		      	}
		    }    
		}        
	}

	if(done) {

		String subtopic = topic.substring(listenTopic.length()-1);
		topic = listenTopic.substring(0, listenTopic.length()-2);

        callback(topic, subtopic ,msg);
        reading = false;
    	
	  	input.flush(); 

	}

}

MQTTclient mqtt;
