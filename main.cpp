#include <iostream>

#include <cstdlib>
#include <string>
#include "mqtt/async_client.h"

using namespace std;

const string SERVER_ADDRESS{"mqtt://broker.emqx.io"};
const string CLIENT_ID{"paho-crash-test-client"};
const string TOPIC{"paho/client/test/crash12345"};
const string MESSAGE("I will crash your app");

constexpr int QOS = 1;

int main() {
    mqtt::create_options createOpts(MQTTVERSION_5);
    mqtt::async_client cli(SERVER_ADDRESS, CLIENT_ID, createOpts);

    auto connOpts = mqtt::connect_options_builder()
            .clean_start(false)
            .properties({
                {mqtt::property::SESSION_EXPIRY_INTERVAL, 604800}
            })
            .finalize();

    try {
        cli.set_connection_lost_handler([](const std::string &) {
            cout << "*** Connection Lost ***" << endl;
        });

        cli.set_disconnected_handler([](const mqtt::properties &, mqtt::ReasonCode reason) {
            cout << "*** Disconnected. Reason: " << reason << " ***" << endl;
        });

        cli.start_consuming();

        cout << "Connecting to the MQTT server..." << flush;
        auto tok = cli.connect(connOpts);

        auto rsp = tok->get_connect_response();

        // Make sure we were granted a v5 connection.
        if (rsp.get_mqtt_version() < MQTTVERSION_5) {
            cout << "Did not get an MQTT v5 connection." << endl;
            exit(1);
        }

        if (!rsp.is_session_present()) {
            cout << "Session not present on broker. Subscribing." << endl;
            cli.subscribe(TOPIC, QOS)->wait();
        }

        cout << "OK" << endl;

        cout << "Sending test message" << endl;

        cli.publish(TOPIC, MESSAGE.c_str(), MESSAGE.length(), QOS, false);

        cout << "Waiting for messages on topic: '" << TOPIC << "'" << endl;

        while (true) {
            auto msg = cli.consume_message();
            if (!msg) break;
            cout << msg->get_topic() << ": " << msg->to_string() << endl;
        }

        if (cli.is_connected()) {
            cout << "\nShutting down and disconnecting from the MQTT server..." << flush;
            cli.stop_consuming();
            cli.disconnect()->wait();
            cout << "OK" << endl;
        } else {
            cout << "\nClient was disconnected" << endl;
        }
    } catch (const mqtt::exception &exc) {
        cerr << "\n  " << exc << endl;
        return 1;
    }

    return 0;
}
