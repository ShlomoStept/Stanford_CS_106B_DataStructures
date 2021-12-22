/*
 * File: LinkedListPatientQueue.cpp
 * --------------------------
 * Name: Shlomo Stept
 * Section leader: N/A --> Self administered course
 */

#include "LinkedListPatientQueue.h"
#include <sstream>
LinkedListPatientQueue::LinkedListPatientQueue() {
    this->front = nullptr;
}


void LinkedListPatientQueue::delete_all(PatientNode *curr){
    if(curr!=nullptr){
        delete_all(curr->next);
        delete curr;
    }
}

LinkedListPatientQueue::~LinkedListPatientQueue() {
    if(this->front == nullptr){

    }else{
        delete_all(this->front);
    }
}

void LinkedListPatientQueue::clear() {
    delete_all(this->front);
    this->front = nullptr;
}

string LinkedListPatientQueue::frontName() {
    if(this->front == nullptr){
        throw string("There is no top. I must abort. I never learned how to love...");
    }else{
        return this->front->name;
    }
}

int LinkedListPatientQueue::frontPriority() {
    if(this->front == nullptr){
        throw string("There is no top. I must abort. I never learned how to love...");
    }else{
        return this->front->priority;
    }
}

bool LinkedListPatientQueue::isEmpty() {
    if(this->front == nullptr){
        return true;
    }else{
        return false;
    }
}


void LinkedListPatientQueue::newPatient(string name, int priority) {
    // Case 1: The Queue is empty
    if(this->front == nullptr){
        this->front = new PatientNode(name, priority);
    }else{
        // Case 2: New Paitent goes first (has the lowest priority)
        if(this->front->priority > priority){
            PatientNode *next = this->front;
            this->front = new PatientNode(name, priority);
            this->front->next = next;
            return;
        } // Case 3: Non empty queue and the new patient is not lowest priority
        else{

            PatientNode *curr = this->front, *last= curr;

            while(curr!= nullptr){
                if(curr->priority<=priority){
                    last = curr;
                    curr= curr->next;
                }else{
                    last->next = new PatientNode(name, priority);
                    last->next->next =curr;
                    return;
                }
            }
            if(curr == nullptr){
                last->next = new PatientNode(name, priority);
            }
        }
    }
}

string LinkedListPatientQueue::processPatient() {
    if(this->front == nullptr){
        throw string("There is no top. I must abort. I never learned how to love...");
    }else{
        string processing = this->front->name;
        this->front = this->front->next;
        return processing;    }
}


// Question: what should i do if the same name occurs multiple times in a row and the first name has lower priority--> currrently this is an error
void LinkedListPatientQueue::upgradePatient(string name, int newPriority) {
    // Case 1: The Queue is empty
    if(this->front == nullptr){
        throw string("There is no top. I must abort. I never learned how to love...");
    }else{
        // Case 2: New Paitent goes first (has the lowest priority) (future upgrade: combine case 2 and 3 using bool value for second while loop and new posittion indicator)
        if(this->front->priority>newPriority){
            PatientNode *curr = this->front, *p_to_l=nullptr;
            while(curr!=nullptr && curr->name!=name){
                p_to_l = curr;
                curr = curr->next;
            }
            if(curr == nullptr){
                throw string("Invalid Patient name. I must abort. I never learned how to love...");
            }else{
                if(p_to_l != nullptr){
                    p_to_l->next= curr->next;
                }
                curr->priority = newPriority;
                if(this->front->name != name){
                    curr->next = this->front;
                    this->front = curr;
                }
            }
        // Case 3: Non empty queue and the new patient is not lowest priority
        }else{
            PatientNode *curr = this->front, *p_to_l=nullptr, *pos=nullptr;
            while(curr!=nullptr && curr->priority<=newPriority){
                pos = curr;
                curr = curr->next;
            }
            while(curr!= nullptr && curr->name!=name){
                p_to_l = curr;
                curr = curr->next;
            }
            if(curr == nullptr){
                throw string("Invalid Patient Name/New Priority is downgrading Patient. I must abort.");
            }else{
                if(p_to_l != nullptr){
                    p_to_l->next= curr->next;
                }
                curr->priority = newPriority;
                if(pos->next->name != name){
                    curr->next = pos->next;
                    pos->next = curr;
                }
            }
        }
    }
}


string LinkedListPatientQueue::int_to_String(int num){
    ostringstream oss;
    oss << num;
    return oss.str();
}

string LinkedListPatientQueue::toString() {
    PatientNode *curr = this->front;
    if(curr==nullptr){
        return "{}";
    }else{
        string output = "{" + int_to_String(curr->priority) + ":" + curr->name;
        curr= curr->next;
        while(curr !=nullptr){
            output += ", " + int_to_String(curr->priority) + ":" + curr->name;
            curr = curr->next;
        }
        output += "}";

        return output;
    }
}
