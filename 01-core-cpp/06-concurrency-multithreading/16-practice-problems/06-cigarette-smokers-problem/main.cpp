#include "agent.hpp"
#include <thread>

int main () {
    Agent agent;

    std::jthread agentThread([&] () {
        while (true) {
            agent.exec();
        }
    });

    std::jthread smoker1([&] () {
        while (true) {
            agent.smoke(1, Agent::MATCHES);
        }
    });

    std::jthread smoker2([&] () {
        while (true) {
            agent.smoke(2, Agent::TOBACCO);
        }
    });

    std::jthread smoker3([&] () {
        while (true) {
            agent.smoke(3, Agent::PAPER);
        }
    });
}