/*
 * File: VectorPatientQueue.h
 * --------------------------
 * Name: Shlomo Stept
 * Section leader: N/A --> Self administered course
 */

#pragma once

#include <iostream>
#include <string>
#include "patientqueue.h"
#include <vector.h>
using namespace std;

struct Patient{
    string name;
    int priority;
    int timestamp;

    Patient(): name(""), priority(-1), timestamp(-1){}
    Patient(string name_, int priority_, int timestamp_): name(name_), priority(priority_), timestamp(timestamp_){}
};

class VectorPatientQueue : public PatientQueue {
public:
    VectorPatientQueue();
    ~VectorPatientQueue();
    string frontName();
    void clear();
    int frontPriority();
    bool isEmpty();
    void newPatient(string name, int priority);
    string processPatient();
    void upgradePatient(string name, int newPriority);
    string toString();

private:
    Vector<Patient> queue;
    int counter;
    int find_front() const;

};
