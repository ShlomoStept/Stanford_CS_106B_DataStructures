// heappatientqueue.cpp
/*
 * File: heappatientqueue.cpp
 * --------------------------
 * Name: Shlomo Stept
 * Section leader: N/A --> Self administered course
 */

#include "HeapPatientQueue.h"

HeapPatientQueue::HeapPatientQueue() {
    front = new string[10];
    capacity= 10;
    size =0;
}

HeapPatientQueue::~HeapPatientQueue() {
    delete[] front;
}

void HeapPatientQueue::clear() {
    size =0;
    capacity = 10;
    delete[] front;
    front = new string[10];
}

string HeapPatientQueue::frontName() {
    if(isEmpty()){
        throw string("There is no top. I must abort. I never learned how to love...");
    }
    return get_name(front[1]);
}

int HeapPatientQueue::frontPriority() {
    if(isEmpty()){
        throw string("There is no top. I must abort. I never learned how to love...");
    }
    return get_pri(front[1]);
}

bool HeapPatientQueue::isEmpty() {
    return (  (size ==0)? true: false);
}

void HeapPatientQueue::newPatient(string name, int priority) {
    if(capacity-1 == size){
        resize();
    }
    // ADD THE pateient to the bottom of the heap and bubble up
    string new_p =  to_string(priority)+ ':' + name;
    this->size +=1;
    front[size]= new_p;
    bubble_up(size);
}

string HeapPatientQueue::processPatient() {
    // Swap the Top and the bottom, return the bottom and then bubble down the top as appropriate
    if(isEmpty()){
        throw string("There is no top. I must abort. I never learned how to love...");
    }
    swap(front[1], front[size]);
    string temp = front[size];
    front[size] = "";
    --size;
    bubble_down();
    return get_name(temp);
}

void HeapPatientQueue::upgradePatient(string name, int newPriority) {

    int old_index =0;
    for(int i =1; i<=size; i++){
        if(get_name(front[i]) == name){
            old_index = i;
            break;
        }
    }
    if(old_index ==0 || newPriority>= get_pri(front[old_index])){
        throw string("Invalid Patient Name/New Priority is downgrading Patient. I must abort.");
    }else{
        front[old_index] = to_string(newPriority)+ ':' + get_name(front[old_index]);
        bubble_up(old_index);
    }
    // Bubble up the patient as nessecary
}

string HeapPatientQueue::toString() {
    if(size==0){
        return "{}";
    }else{
        int index =1;
                string output = "{" + front[index++];
        while(index <=size){
            output += ", " + front[index++];
        }
        output += "}";
        return output;
    }
}


int HeapPatientQueue::get_pri(string value){
    return stoi(value.substr(0, value.find_last_of(':')));

}
string HeapPatientQueue::get_name(string value){
    return value.substr(value.find_last_of(':')+1);
}

void HeapPatientQueue::resize(){
    string *temp = this->front;
    this->capacity = capacity*2;
    this->front = new string[capacity];
    for(int i =1; i <=this->size; i++){
        front[i] = temp[i];
    }

}

void HeapPatientQueue::bubble_up(int index){
    int parent = (index/2);
    while(parent>0 && index>0){
        if( get_pri(front[index]) < get_pri(front[parent]) ){
            swap(front[parent], front[index]);
            index = parent;
            parent = (parent/2);
        }else if(get_pri(front[index]) == get_pri(front[parent]) ){
            if(get_name(front[index]) < get_name(front[parent]) ){
                swap(front[parent], front[index]);
                index = parent;
                parent = (parent/2);
            }else{
                return;
            }
        }else{
            return;
        }
    }
}


void HeapPatientQueue::bubble_down(){
    int top = 1;
    while(top<=size){
        int left_child = top*2;
        int right_child = top*2 + 1;

        if(left_child<=size && right_child<=size ){
            int choice;
            if(get_pri(front[left_child]) < get_pri(front[right_child])){
                choice = left_child;
            }else if( get_pri(front[left_child]) == get_pri(front[right_child])){
                if( get_name(front[left_child]) < get_name(front[right_child]) ){
                    choice = left_child;
                }else{
                    choice = right_child;
                }
             }else{
                choice = right_child;
            }


            if(get_pri(front[choice]) < get_pri(front[top])){
                swap(front[choice], front[top]);
                top = choice;
            }else if (get_pri(front[choice]) == get_pri(front[top])){
                // 2 Cases: (1) string lower (2) string higher
                if(get_name(front[choice]) < get_name(front[top])){
                    swap(front[choice], front[top]);
                    top = choice;
                }else{
                    //do nothing
                    return;
                }
            }

        }else if(left_child<=size ){
            if(get_pri(front[left_child]) < get_pri(front[top])){
                swap(front[left_child], front[top]);
                top = left_child;
            }else if (get_pri(front[left_child]) == get_pri(front[top])){
                // 2 Cases: (1) string lower (2) string higher
                if(get_name(front[left_child]) < get_name(front[top])){
                    swap(front[left_child], front[top]);
                    top = left_child;
                }else{
                    //do nothing
                    return;
                }
            }else{
                //do nothing
                return;
            }

        }else if(right_child<=size){
            if(get_pri(front[right_child]) < get_pri(front[top])){
                swap(front[right_child], front[top]);
                top = right_child;
            }else if (get_pri(front[right_child]) == get_pri(front[top])){
                // 2 Cases: (1) string lower (2) string higher
                if(get_name(front[right_child]) < get_name(front[top])){
                    swap(front[right_child], front[top]);
                    top = right_child;
                }else{
                    //do nothing
                    return;
                }
            }
        }else{
            return;
        }
    }
}
