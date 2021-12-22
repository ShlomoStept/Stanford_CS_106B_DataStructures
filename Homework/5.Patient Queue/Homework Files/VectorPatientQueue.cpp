/*
 * File: VectorPatientQueue.cpp
 * --------------------------
 * Name: Shlomo Stept
 * Section leader: N/A --> Self administered course
 */

#include "VectorPatientQueue.h"
#include <sstream>

VectorPatientQueue::VectorPatientQueue() {
    this->counter = 1;
}

VectorPatientQueue::~VectorPatientQueue() {
}

void VectorPatientQueue::clear() {
    this->queue.clear();
    counter = 1;
}


int VectorPatientQueue::find_front() const{
    int size = this->queue.size();
    if(size < 1){
        throw string("There is no top. I must abort. I never learned how to love...");
    }else{
        int front = 0;
        for(int n = 1; n<size; n++){
            if(this->queue[n].priority < this->queue[front].priority ){
                front = n;
            }else if(this->queue[n].priority == this->queue[front].priority){
                if(this->queue[n].timestamp < this->queue[front].timestamp){
                    front = n;
                }
            }
        }
        return front;
    }
}

string VectorPatientQueue::frontName() {
    int front = find_front();
    return this->queue[front].name;
}

int VectorPatientQueue::frontPriority() {
    int front = find_front();
    return this->queue[front].priority;
}

bool VectorPatientQueue::isEmpty() {
    return (this->queue.size() == 0);
}

void VectorPatientQueue::newPatient(string name, int priority) {
    this->queue.push_back(Patient(name, priority, counter++));
}

string VectorPatientQueue::processPatient() {
    int front = find_front();
    string name = this->queue[front].name;
    this->queue.remove(front);
    return name;
}

void VectorPatientQueue::upgradePatient(string name, int newPriority) {
    int size = this->queue.size();
    if(size < 1){
        throw string("There is no top. I must abort. I never learned how to love...");
    }else{
        bool found = false;
        int max_priority = -1;
        for(int n = 0; n<size; n++){
            if(this->queue[n].name == name){
                if(found){
                      if(this->queue[n].priority < this->queue[n].priority){
                          max_priority = n ;
                      }
                }else{
                    found = true;
                    max_priority = n;
                }
            }
        }
        if(!found){
            throw string("Error: given patient is not in the queue");
        }else{
            this->queue[max_priority].priority = newPriority;
        }
    }

}

string int_to_String(int num){
    ostringstream oss;
    oss << num;
    return oss.str();
}

string VectorPatientQueue::toString() {
    string output = "{";
    int size = this->queue.size();
    if(size == 0){
        output += "}";
    }else{
        output += int_to_String(this->queue[0].priority) + ":" + this->queue[0].name;
        for(int i = 1; i<size; i++ ){
            output += ", " + int_to_String(this->queue[i].priority) + ":" + this->queue[i].name;
        }
        output += "}";
    }

    return output;
}
