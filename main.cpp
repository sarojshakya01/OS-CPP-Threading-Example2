#include <iostream>
#include <pthread.h>
#include <cstring>
#include <unistd.h>

#define MAXCUSTOMERS 100

using namespace std;

static int nFreeNurses = 0, nVaccinated  = 0, nHadToWait = 0;

static pthread_mutex_t mtx_access = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_next = PTHREAD_COND_INITIALIZER;


struct cust
{
    int vaccinatedTime, observationTime;
    string name;
};

// function to get the special character separated value with the help of index
string entry(string str, int idx)
{

    str += '\0';
    string value = "";
    int countValue = 0;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == ' ' || str[i] == '    ')
        {
            if (idx == countValue)
                break;
            countValue++;
            value = "";
        }
        else
        {
            value += str[i];
        }
    }
    return value;
} // entry


string sanitize_line(string line)
{
    char ch;
    int count = 0, i = 0;
    string value = "";

    while(line[i] != '\0')
    {
        ch = line[i];
        if(ch == '\t')
            ch = ' ';
        
        if(ch == ' ')
            count++;
        else
           count = 0;

        if(count <= 1)    
            value += ch;

        i++;
    }
    return value;
}


void *customer(void *arg)
{
    struct cust *cData = (struct cust *)arg;

    cout << cData->name << " arrives at the center." << endl;

    if (nFreeNurses == 0)
    {
        nHadToWait++;
        cout << "-- " << cData->name << " has to wait." << endl;
    }



    pthread_mutex_lock(&mtx_access);

    while (nFreeNurses == 0)
    {
        pthread_cond_wait(&cond_next, &mtx_access);
    }

    nFreeNurses--;

    cout << cData->name << " is getting the vaccine." << endl;

    pthread_mutex_unlock(&mtx_access);



    sleep(cData->vaccinatedTime);



    pthread_mutex_lock(&mtx_access);
    
    nFreeNurses++;

    pthread_cond_signal(&cond_next);

    pthread_mutex_unlock(&mtx_access);
    


    cout << cData->name << " got vaccinated." << endl;
    
    nVaccinated++;

    sleep(cData->observationTime);

    cout << cData->name << " leaves the center." << endl;

    pthread_exit((void *)0);
} // customer


int main(int argc, char *argv[])
{

    string *input = new string[MAXCUSTOMERS];
    int nSTAFF = 0, nCustomers = 0;

    try
    {
        if (argc < 2) throw runtime_error("Number of staff members is not provided");

        try
        {
            nSTAFF = stoi(argv[1]);
        }
        catch (...) {
            throw runtime_error("Number of staff members is not valid");
        }

        string line, filename;

        while (getline(cin, line))
        {
            if (nCustomers >= MAXCUSTOMERS) throw runtime_error("Customer Number limited to " + to_string(MAXCUSTOMERS));
            input[nCustomers] = sanitize_line(line);
            nCustomers++;
        }

        cout << "-- Center has " << nSTAFF << " nurse(s) on duty.\n";

        nFreeNurses = nSTAFF;

        pthread_t custtid[nFreeNurses];

        int error = pthread_mutex_init(&mtx_access, NULL);

        if (error) throw runtime_error("Mutex initialization failed");

        for (int i = 0; i < nCustomers; i++)
        {
            
            struct cust *cData = new struct cust;

            cData->name = entry(input[i], 0);
            cData->vaccinatedTime = stoi(entry(input[i], 2));
            cData->observationTime = stoi(entry(input[i], 3));

            int timeDelay = 0;
            timeDelay = stoi(entry(input[i], 1));

            sleep(timeDelay);

            error = pthread_create(&custtid[i], NULL, customer, (void *)cData);

            if (error) throw runtime_error("Thread can not be created");

        }

        for (int i = 0; i < nVaccinated; i++)
        {
            pthread_join(custtid[i], NULL);
        }

        pthread_mutex_destroy(&mtx_access);

        cout << nVaccinated << " customer(s) were vaccinated." << endl;
        cout << nHadToWait << " of them had to wait." << endl;
        exit(0);
    }
    catch(exception &err) {
        cerr<< err.what() << "!\n";
    }

    delete input;
        
    return 0;
}