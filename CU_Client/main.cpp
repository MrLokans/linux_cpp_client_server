#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <sys/time.h>
#include <fstream>

using namespace std;

int menu(string info, int min, int max);
int get_int(string phrase);

string get_string(string phrase);
string get_time();
string send_get(int s, string send);
string ID=" ";

void do_log(string info);
void do_log(string info, bool no_cout);

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

int main()
{
    struct sockaddr_in peer;
    int s;
    int rc;

    peer.sin_family = AF_INET;
    peer.sin_port = htons(7500);
    peer.sin_addr.s_addr = inet_addr("127.0.0.1");

    s = socket(AF_INET, SOCK_STREAM, 0);
    rc = connect(s, (struct sockaddr *)&peer, sizeof(peer));

    while(true)
    {
        int pick = menu("1) Add note\n2) Show all note's\n3) Delete note\n4) Change note\n5) Search note's\n6) Sort\n0) Exit\n",0,6);
        string str_pick = to_string(pick);
        send_get(s,str_pick);
        switch (pick)
        {
            case 1:
            {
                cout << send_get(s, get_string("Name: ")) << endl;
                cout << send_get(s, get_string("Address: ")) << endl;
                cout << send_get(s, get_string("Mail: ")) << endl;
                cout << send_get(s, get_string("Telephone: ")) << endl;
                break;
            }
            case 2:
            {
                string str_size =  send_get(s, "How many structures need to get");
                int size = atoi(str_size.c_str());

                for (unsigned int i = 0; i<size;i++)
                {
                    unsigned int j = i+1;
                    cout << "Note №" << j << endl;
                    cout << "Name: " << send_get(s,"Send name") << endl;
                    cout << "Address: " << send_get(s,"Send address") << endl;
                    cout << "Mail: " << send_get(s,"Send mail") << endl;
                    cout << "Telephone: " << send_get(s,"Send telephone") << endl << endl;
                }
                break;
            }
            case 3:
            {
                string str_size = send_get(s, "How many structures");
                int size = atoi(str_size.c_str());
                cout << "Available " << size << " notes" << endl;
                int pick = menu("Number of record to be deleted (0 - quit)",0,size);
                string str_pick = to_string(pick);
                cout << send_get(s,str_pick) << endl;
                break;
            }
            case 4:
            {
                string str_size = send_get(s, "How many structures");
                int size = atoi(str_size.c_str());
                cout << "Available " << str_size << " notes" << endl;

                int pick = menu("Number of record to be changed (0 - quit)",0,size);
                string str_pick = to_string(pick);
                cout << send_get(s,str_pick) << endl;

                cout << send_get(s, get_string("Name: ")) << endl;
                cout << send_get(s, get_string("Address: ")) << endl;
                cout << send_get(s, get_string("Mail: ")) << endl;
                cout << send_get(s, get_string("Telephone: ")) << endl;

                break;
            }
            case 5:
            {
                int pick = menu("Select the search field\n1) Name\n2) Address\n3) Mail\n4) Telephone\n",1,4);
                string search_pick = to_string(pick);
                send_get(s,search_pick);
                string search_element;
                if (pick == 1) search_element = get_string("Name: ");
                if (pick == 2) search_element = get_string("Address: ");
                if (pick == 3) search_element = get_string("Mail: ");
                if (pick == 4) search_element = get_string("Telephone: ");
                send_get(s,search_element);
                string str_size = send_get(s,"How many structures");
               // cout << str_size;
                int size = atoi(str_size.c_str());
                for (unsigned int i = 0; i<size;i++)
                {
                    unsigned int j = i+1;
                    cout << "Note №" << j << endl;
                    cout << "Name: " << send_get(s,"Send name") << endl;
                    cout << "Address: " << send_get(s,"Send address") << endl;
                    cout << "Mail: " << send_get(s,"Send mail") << endl;
                    cout << "Telephone: " << send_get(s,"Send telephone") << endl << endl;
                }
                break;
            }
            case 6:
            {
                int pick = menu("Select the sort field\n1) Name\n2) Address\n3) Mail\n4) Telephone\n",1,4);
                string search_pick = to_string(pick);
                send_get(s,search_pick);
                break;
            }
            case 0:
            {
                return 0;
            }
            default:
            {
                cout << "No such pick" << endl;
                break;
            }

        }
    }

    close(s);

}

int menu(string info, int min, int max)
{
    while(true)
    {
        cout << info << endl;
        int pick = get_int("Your choice: ");
        if (pick < min || pick > max)
        {
            //system("clear");
            cout << "No such pick, try again" << endl;
        }
        else return pick;
    }
    return 0;
}

int get_int(string phrase)
{
    int value;
    while(true)
    {
        cout << phrase;
        cin.clear();
        while(!(cin >> value))
        {
            cin.clear();
            while(cin.get()!='\n') continue;
            cout << "Please enter a valid integer" << endl;
        }
        while(cin.get() != '\n');
        if (value < 0) cout << "Enter a positive number" << endl;
        else break;
    }
    return value;
}

string get_string(string phrase)
{
    string tmp_string;
    //do_log(phrase);
    string time = get_time();
    cout << time << " " << phrase;
    cin.clear();
    cout.flush();
    tmp_string.clear();
    getline(cin,tmp_string);
    tmp_string.resize(20);
    cin.clear();
    cout.flush();
    string log = phrase+tmp_string;
    do_log(log, true);
    return tmp_string;
}

string send_get(int s, string tmp_string)
{
    //размер
    char *tmp_send = new char [tmp_string.size()+1];
    strcpy(tmp_send,tmp_string.c_str());

    int str_size=tmp_string.length();

    char char_size[10];
    sprintf(char_size,"%d",str_size);

    send(s,char_size,sizeof(char_size),0);
    recv(s,char_size,sizeof(char_size),0);

    send(s,tmp_send, str_size,0);

    recv(s,char_size,sizeof(char_size),0);

    send(s,char_size,sizeof(char_size),0);
    str_size=atoi(char_size);

    char *tmp_receive = new char[str_size];
    recv(s,tmp_receive,str_size,0);

    //recv(s,tmp_char,sizeof(tmp_char),0);
    string tmp_result = tmp_receive;
    return tmp_result;
}

string get_time()
{
    char cBuffer[100];
    time_t zaman;
    struct tm *ltime;
    static struct timeval _t;
    static struct timezone tz;
    time(&zaman);
    ltime = (struct tm *) localtime(&zaman);
    gettimeofday(&_t, &tz);
    strftime(cBuffer,100,"%d.%m.%y %H:%M:%S",ltime);
    sprintf(cBuffer, "%s.%d", cBuffer,(int)_t.tv_usec);
    string time = "[";
    time.append(cBuffer);
    time.append("]");
    return time;
}

void do_log(string info)
{
    ofstream log_file;
    string my_time = get_time();
    log_file.open("client_log.txt", ios_base::app);
    cout << my_time << info;
    log_file << my_time << " (" << ID <<") " << info << endl;
    log_file.close();
}

void do_log(string info, bool no_cout)
{
    fstream log_file;
    string my_time = get_time();
    log_file.open("client_log.txt");
  //  cout << "1";
    log_file << my_time << " (" << ID <<") " << info << endl;
 //   cout << "2";
    log_file.close();
}