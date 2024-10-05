#include<iostream>
#include<String>
using namespace std;

struct Node{

    string process_id;
    int exec_time;
    int remaining_time;

    Node* next;


};

class CircularLinkedList {
private:
    
public:
    Node* head;
    int num_of_processes;
    int process_time=0;
    // Constructor to initialize head
    CircularLinkedList() {
        head = nullptr;
        num_of_processes=0;
        
        process_time=process_execution(process_time);
        
    }

    // Function to add a new node at the end of the list
    void append(int value) {
        Node* newNode = new Node();
        newNode->remaining_time = value;
        newNode->next=head;
        

        num_of_processes+=1;
        newNode->process_id="P"+to_string(num_of_processes);

        if (head == nullptr) {
            head = newNode; // If the list is empty, make the new node the head
            head->next=head;
        } else {
            Node* temp = head;
            // Traverse to the last node
            while (temp->next != head) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    // Function to print all elements of the list
    void cycleList() {
        if(head==nullptr){
            cout<<"List is empty."<<endl;
            return;
        }
        Node* temp = head;
        
       
        do{if((temp->remaining_time-process_time)>0)
        {
            cout <<"("<< temp->process_id<<","<<(temp->remaining_time-process_time)<<")" << " -> ";
            temp->remaining_time=temp->remaining_time-process_time;
        }
        else{//Process Completed, removing relevant node
            cout <<"("<< temp->process_id<<","<<"(Completes)"<<")" << " -> ";

             // If this is the only node in the list
            if (temp == head) {
                head = (temp->next == head) ? nullptr : temp->next;
            }

            Node* prev_node=head;
            while (prev_node->next!=temp)
            {
                prev_node=prev_node->next;
            }
            prev_node->next=temp->next;
            Node* toDelete=temp;
            delete toDelete;
            
            

        }
            temp = temp->next;
            
        }while (temp != head);
           
            
    }

    int process_execution(int exec_time){
            cout<<"Enter execution time per process:";
            cin>>exec_time;
            return exec_time;
        
        }
};
int main(){

    CircularLinkedList list;
    int num_cycles;
    char response;

    list.append(10);
    list.append(5);
    list.append(8);
    
    

    cout<<"How many cycles to run?";
    cin>>num_cycles;

for(int i=0; i<num_cycles;i++){

    cout<<"Cycle "<<i+1<<" :"<<endl;
    list.cycleList();

    cout<<"Want to enter new Process in system? (Y/N)";
    cin>>response;

    if(response=='Y'||response=='y'){
        int process_time;
        cout<<"Enter total time for new Process to execute completely: ";
        cin>>process_time;
        list.append(process_time);
        cout<<"New process arrives:"<<endl;
    }


}
    cout<<"cycles completed."<<endl;

    return 0;
}