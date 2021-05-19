#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;

void user_input(vector<int>&available, vector<vector<int> >&max, vector<vector<int> >&allocation);
void finish_need(vector<int>available, vector<vector<int> >max, vector<vector<int> >allocation, vector<vector<int> >&need);
void safty_algorithm(vector<int>available, vector<vector<int> >allocation, vector<vector<int> >need, int request);
void resourse_request_algorithm(vector<int>available, vector<vector<int> >allocation, vector<vector<int> >need);
int choose_algorithm();
bool next();

int main()
{
    while(true){
        int processes, resources;

        cout << "\tEnter he number of processes is: ";
        cin >> processes;
        cout << "\tEnter he number of resource is: ";
        cin >> resources;

        vector<int>available(resources);
        vector<vector<int> >max(processes);
        vector<vector<int> >allocation(processes);
        vector<vector<int> >need(processes);

        user_input(available, max, allocation);
        finish_need(available, max, allocation, need);

        bool repeat;
        do {
            int algorithm = choose_algorithm();
            switch (algorithm){
                case 1: safty_algorithm(available, allocation, need, -1);
                        break;
                case 2: resourse_request_algorithm(available, allocation, need);
                        break;
                default: safty_algorithm(available, allocation, need, -1);
            }
            repeat = next();
        } while(repeat);
        system("cls");
    }

    return 0;
}

void user_input(vector<int>&available, vector<vector<int> >&max, vector<vector<int> >&allocation){
    int processes = allocation.size();
    int resources = available.size();
    int input;

    cout << "\n\tEnter the (Allocation) matrix (R0 R1 R2 .....)\n";
    cout << "\t==============================================\n";
    for(int i = 0; i < processes; i++){
        cout << "\tP" << i << ": ";
        for(int j = 0; j <resources; j++){
            cin >> input;
            allocation[i].push_back(input);
        }
    }

    cout << "\n\tEnter the (Max) matrix (R0 R1 R2 .....)\n";
    cout << "\t========================================\n";
    for(int i = 0; i < processes; i++){
        cout << "\tP" << i << ": ";
        for(int j = 0; j <resources; j++){
            cin >> input;
            max[i].push_back(input);
        }
    }

    cout << "\n\tEnter the (Available) matrix (R0 R1 R2 .....): ";
    for (int i = 0; i < resources; i++){
        cin >> available[i];
    }
}

void finish_need(vector<int>available, vector<vector<int> >max, vector<vector<int> >allocation, vector<vector<int> >&need){
    int processes = allocation.size();
    int resources = available.size();

    for(int i = 0; i < processes; i++){
        for (int j = 0; j < resources; j++)
            need[i].push_back(max[i][j] - allocation[i][j]);
    }

    cout << "\n\tThe (Need) matrix is \n";
    cout << "\t=====================\n\t";
    for (int i = 0; i < resources; i++)
        cout << "\tR" << i;
    cout << "\n";
    for(int i = 0; i < processes; i++){
        cout << "\tP" << i <<"\t";
        for (int j = 0; j < resources; j++)
            cout << need[i][j] << "\t";
        cout << "\n";
    }
}

void safty_algorithm(vector<int>available, vector<vector<int> >allocation, vector<vector<int> >need, int request){
    int resources = available.size();
    int processes = need.size();

    vector<int>work;
    for(int i = 0; i < resources; i++)
        work.push_back(available[i]);

    vector<bool>finish(processes, false);
    vector<int>sequence;

    bool done, found;
    do {
        done = false;
        for(int i = 0; i < processes; i++){
            found = true;
            for (int j = 0; j < resources; j++){
                if (need[i][j] > work[j]){
                    found = false;
                    break;
                }
            }
            if (found && !finish[i]){
                done = true;
                finish[i] = true;
                sequence.push_back(i);
                for (int j = 0; j < resources; j++)
                    work[j] += allocation[i][j];
            }
        }
    } while(done);

    if ((int)sequence.size() == processes){
        if (request >=0){
            cout << "\tYes request can be granted with safe state, Safe state <P" << request << "req,";
        } else {
            cout << "\tYes , Safe state <";
        }
        for (int i = 0; i < processes; i++){
            if (i == processes - 1)
                cout << "P" << sequence[i];
            else
                cout << "P" << sequence[i] << ",";
        }
        cout << ">\n";
    } else {
        if (request >= 0){
            cout << "\tNo request can't be granted with safe state, Unsafe state\n";
        } else {
            cout << "\tNo, Unsafe state\n";
        }
    }
}

void resourse_request_algorithm(vector<int>available, vector<vector<int> >allocation, vector<vector<int> >need){
    int resources = available.size();
    int input;

    cout << "\tEnter the number of the process that made the request: ";
    int process_num;
    cin >> process_num;

    cout << "\tEnter the resource request (R0 R1 R2 .....): ";
    vector<int>request;
    for (int i = 0; i < resources; i++){
        cin >> input;
        request.push_back(input);
    }

    bool ok = true;
    for (int i = 0; i < resources; i++){
        if (request[i] > need[process_num][i]){
            ok = false;
            break;
        }
    }
    if(!ok){
        cout << "\tThe process has exceeded its maximum claim!!\n";
        return;
    }

    for (int i = 0; i < resources; i++){
        if (request[i] > available[i]){
            ok = false;
            break;
        }
    }
    if(!ok){
        cout << "\tThe process must wait since the resources are not available right now!!\n";
        return;
    }

    for(int i = 0; i < resources; i++){
        available[i] -= request[i];
        allocation[process_num][i] += request[i];
        need[process_num][i] -= request[i];
    }

    safty_algorithm(available, allocation, need, process_num);
}

int choose_algorithm(){
    cout << "\n\tChoose the algorithm\n";
    cout << "\t===================\n";
    cout << "\tFor Safty Algorithm press 1\n";
    cout << "\tFor Resource Request Algorithm press 2\n";
    cout << "\tEnter your choice: ";
    int choice;
    cin >> choice;
    return choice;
}

bool next(){
    while (true){
        cout << "\n\tWhat is next?\n";
        cout << "\t===================\n";
        cout << "\tFor new query press 1\n";
        cout << "\tFor new input press 2\n";
        cout << "\tEnter your choice: ";
        int choice;
        cin >> choice;

        if (choice == 1)
            return true;
        else if (choice == 2)
            return false;
    }
}
