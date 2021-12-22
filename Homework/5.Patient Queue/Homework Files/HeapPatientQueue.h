// heappatientqueue.h
/*
 * File: heappatientqueue.h
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

class HeapPatientQueue : public PatientQueue  {
public:
    HeapPatientQueue();
    ~HeapPatientQueue();
    string frontName();
    void clear();
    int frontPriority();
    bool isEmpty();
    void newPatient(string name, int priority);
    string processPatient();
    void upgradePatient(string name, int newPriority);
    string toString();

private:
    string *front;
    int capacity;
    int size;
    int get_pri(string value);
    string get_name(string value);
    void resize();
    void bubble_up(int index);
    void bubble_down();
};
