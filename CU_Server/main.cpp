#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <netinet/in.h>

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

string get_send_data(int s, string tmp_string);


struct customer_information
{
    string name;
    string address;
    string mail;
    string telephone;
    bool operator< (const customer_information& a) const{
        return name < a.name;
    }

};

vector <customer_information> clients(0);

namespace to_string_patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

using namespace to_string_patch;


int Func(int newS)
{
    cout << "New client" << endl;
    while(true) {
        string tmp_pick = get_send_data(newS," ");
        char p = tmp_pick[0];
        cout << p << endl;
        switch (p)
        {
            case '1':
            {
                string name = get_send_data(newS, "get name");
                cout << name << endl;

                string address = get_send_data(newS, "get address");
                cout << address << endl;

                string mail = get_send_data(newS, "get mail");
                cout << mail << endl;

                string telephone = get_send_data(newS, "get telephone number");
                cout << telephone << endl;

                unsigned long int size = clients.size();
                clients.resize(++size);
                size--;
                clients[size].name = name;
                clients[size].address = address;
                clients[size].mail = mail;
                clients[size].telephone = telephone;
                break;
            }
            case '2':
            {
                unsigned long int size = clients.size();
                string str_size = to_string(size);
                cout << get_send_data(newS,str_size) << endl;
                for (int i = 0; i< size; i++)
                {
                    string name = clients[i].name;
                    string address = clients[i].address;
                    string mail = clients[i].mail;
                    string telephone = clients[i].telephone;

                    cout << get_send_data(newS,name) << endl;
                    cout << get_send_data(newS,address) << endl;
                    cout << get_send_data(newS,mail) << endl;
                    cout << get_send_data(newS,telephone) << endl;
                }
                break;
            }
            case '3':
            {
                unsigned long int size = clients.size();
                string str_size = to_string(size);
                cout << get_send_data(newS,str_size) << endl;
                string str_del = get_send_data(newS,"Record is deleted");
                int del = atoi(str_del.c_str());
                clients.erase(clients.begin()+del);
                break;
            }
            case '4':
            {
                unsigned long int size = clients.size();
                string str_size = to_string(size);
                cout << get_send_data(newS,str_size) << endl;
                string str_del = get_send_data(newS,"Record is is ready to change");
                int pick = atoi(str_del.c_str());

                string name = get_send_data(newS, "Successful get name");
                cout << name << endl;

                string address = get_send_data(newS, "Successful get address");
                cout << address << endl;

                string mail = get_send_data(newS, "Successful get mail");
                cout << mail << endl;

                string telephone = get_send_data(newS, "Successful get telephone number");
                cout << telephone << endl;


                pick --;
                cout << "!! " << pick << "!!" << endl;
                clients[pick].name = name;
                clients[pick].address = address;
                clients[pick].mail = mail;
                clients[pick].telephone = telephone;

                break;
            }
            case '5':
            {
                string str_pick = get_send_data(newS," ");
                int search_pick = atoi(str_pick.c_str());
                cout << search_pick;
                vector<int> items;
                string search_element = get_send_data(newS," ");
                cout << search_element << endl;
                    for(int i = 0; i < clients.size(); i++)
                    {
                        if (search_pick == 1) {
                            if (clients[i].name == search_element) items.push_back(i);
                        }
                        if (search_pick == 2) {
                            if (clients[i].address == search_element) items.push_back(i);
                        }
                        if (search_pick == 3) {
                            if (clients[i].mail == search_element) items.push_back(i);
                        }
                        if (search_pick == 4) {
                            if (clients[i].telephone == search_element) items.push_back(i);
                        }
                    }
                    unsigned long int size = items.size();
                    string str_size = to_string(size);
                    get_send_data(newS, str_size);

                for (int i = 0; i< size; i++)
                {
                    string name = clients[items[i]].name;
                    string address = clients[items[i]].address;
                    string mail = clients[items[i]].mail;
                    string telephone = clients[items[i]].telephone;

                    cout << get_send_data(newS,name) << endl;
                    cout << get_send_data(newS,address) << endl;
                    cout << get_send_data(newS,mail) << endl;
                    cout << get_send_data(newS,telephone) << endl;
                }

                break;
            }
            case '6':
            {
                string str_pick = get_send_data(newS," ");
                int search_pick = atoi(str_pick.c_str());
                cout << search_pick;
                if (search_pick == 1)
                {
                    sort(clients.begin(),clients.end());
                }
                break;
            }
            default:
                cout << "client is disconnected" << endl;
                return 0;
        }
    }

}
void reaper(int sig)
{
    int status;
    while (wait3(&status, WNOHANG, (struct rusage*)0) >= 0);
}
int main()
{


    struct sockaddr_in local;
    int s, newS, rc;

    local.sin_family = AF_INET;
    local.sin_port = htons(7500);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    s = socket(AF_INET, SOCK_STREAM, 0);
    rc = bind(s, (struct sockaddr *)&local, sizeof(local));
    rc = listen(s, 5);
    (void)signal(SIGCHLD, reaper);
    while (true)
    {
        newS = accept(s, NULL, NULL);
        switch(fork())
        {
            case 0:
                (void)close(s);
                exit(Func(newS));
            default:
                (void)close(newS);
        }
    }
    return 0;
}

string get_send_data(int s, string tmp_string)
{
    char char_size[10];
    recv(s,char_size,sizeof(char_size),0);
    send(s,char_size,sizeof(char_size),0);
    int size=atoi(char_size);


    char *tmp_receive = new char[size];

    recv(s,tmp_receive,size,0);



    char *tmp_send = new char [tmp_string.size()+1];
    strcpy(tmp_send,tmp_string.c_str());
    size=tmp_string.length();
    sprintf(char_size,"%d",size);
    send(s,char_size,sizeof(char_size),0);
    recv(s,char_size,sizeof(char_size),0);


    send(s,tmp_send,size,0);


    string tmp_result(tmp_receive);
    return tmp_result;
}