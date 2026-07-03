#include <iostream>
#include <thread>
#include <sys/signal.h>
#include "queue.hpp"
#include "producer.hpp"
#include "consumer.hpp"

bool shutdown = false;
std::condition_variable conditionVarConsumer;
std::condition_variable conditionVarProducer;

void interruptHandler(int sig) {
    std::cout << "Signal here" << std::endl;
    shutdown = true;
    conditionVarConsumer.notify_all();
    conditionVarProducer.notify_all();
}

int main () {
    signal(SIGINT, interruptHandler);

    Queue queue(10, &conditionVarConsumer, &conditionVarProducer, &shutdown);
    Producer producer(&queue, &shutdown);
    Consumer consumer(&queue, &shutdown);

    std::jthread producerThread([&] () {
        producer.exec();
    });

    std::jthread consumerThread([&] () {
        consumer.exec();
    });
}