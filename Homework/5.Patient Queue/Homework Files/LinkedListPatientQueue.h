/*
 * File: LinkedListPatientQueue.h
 * --------------------------
 * Name: Shlomo Stept
 * Section leader: N/A --> Self administered course
 */

#pragma once

#include <iostream>
#include <string>
#include "patientnode.h"
#include "patientqueue.h"
using namespace std;

class LinkedListPatientQueue : public PatientQueue {
public:
    LinkedListPatientQueue();
    ~LinkedListPatientQueue();
    string frontName();
    void clear();
    int frontPriority();
    bool isEmpty();
    void newPatient(string name, int priority);
    string processPatient();
    void upgradePatient(string name, int newPriority);
    string toString();

private:
    PatientNode *front;
    void delete_all(PatientNode *curr);
    string int_to_String(int num);

};
