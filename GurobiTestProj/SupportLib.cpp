#include "SupportLib.h"
#include <cmath>


list<erlangRegister> SupportLib::leArquivo()
{
    
    char fileName[50] = "myFile.csv";
    /*
    cout << "\nDigite o nome do arquivo:\n";
    string str;
    //scanf("%s", fileName);
    gets(fileName);*/

    //filename = ;

    ifstream Myfile(fileName);
    if (!Myfile.is_open())
    {
        cout << "\nERROR!\n";
    }
    else
    {
        string numberOfCalls, periodOfMinutes, averageHandlingTimeMinutes, 
            requiredServiceLevel, targetAnswerTimeSeconds, maximumOccupancy, shrinkage;
        float serviceLvl, occupancy, shrink, avgHandlingTm;
        int nCalls, periodMin, tgtAnswerTm;
        string line, myStr;
        
        list<erlangRegister> registros;
        erlangRegister aux;

        while (getline(Myfile, line))
        {
            stringstream ss(line);
            getline(ss, numberOfCalls, ',');
            getline(ss, periodOfMinutes, ',');
            getline(ss, averageHandlingTimeMinutes, ',');
            getline(ss, requiredServiceLevel, ',');
            getline(ss, targetAnswerTimeSeconds, ',');
            getline(ss, maximumOccupancy, ',');
            getline(ss, shrinkage, ',');

            nCalls = atoi(numberOfCalls.c_str());
            periodMin = atoi(periodOfMinutes.c_str());
            avgHandlingTm = atof(averageHandlingTimeMinutes.c_str());
            serviceLvl = atof(requiredServiceLevel.c_str());
            tgtAnswerTm = atoi(targetAnswerTimeSeconds.c_str());
            occupancy = atof(maximumOccupancy.c_str());
            shrink = atof(shrinkage.c_str());

            aux = { nCalls, periodMin, avgHandlingTm, serviceLvl, tgtAnswerTm, occupancy,shrink };
            registros.push_back(aux);
            
        }
        return registros;
    }
}

void SupportLib::printRegister(erlangRegister e)
{
    cout << endl <<
        e.numberOfCalls << " - " <<
        e.periodOfMinutes << " - " <<
        e.averageHandlingTimeMinutes << " - " <<
        e.requiredServiceLevel << " - " <<
        e.targetAnswerTimeSeconds << " - " <<
        e.maximumOccupancy << " - " <<
        e.shrinkage;
}

int SupportLib::fatorial(int n)
{
    if (n <= 1)
        return 1;
    else
        return n*SupportLib::fatorial(n - 1);
}

double SupportLib::A(erlangRegister e)
{
    if (e.periodOfMinutes != 60)
    {
        int aux = 60 / e.periodOfMinutes;
        e.periodOfMinutes *= aux;
        e.numberOfCalls *= aux;
    }
    int erlang = ((e.numberOfCalls * e.averageHandlingTimeMinutes)/60);
    
    return erlang;
}

double SupportLib::N(erlangRegister e)
{
    int agents = A(e)+1;
    return agents;
}

double SupportLib::erlang(erlangRegister e, double N)
{
    double probability, dividend, divider, sum = 0;

    dividend = ((pow(A(e),N) / SupportLib::fatorial(N)) * ((N) / (N - A(e))));

    for (int i=0; i<=(N-1); i++)
    {
        sum += ((pow(A(e), i)) / (SupportLib::fatorial(i)));
    }

    divider = ((sum) + (dividend));
    probability = dividend / divider;

    try {
        probability = dividend / divider;
    }
    catch (exception e)
    {
        cout << "Divisão por zero" << endl;
    }
    return probability;
}

double SupportLib::serviceLevel(erlangRegister e, double N)
{
    double serviceLevel, euler, x;
    x = ((N - A(e)) * (e.targetAnswerTimeSeconds / (double)(e.averageHandlingTimeMinutes*60)));
    euler = exp(1);
    serviceLevel = (1 - (erlang(e,N) * exp(-1*x)));
    return serviceLevel;
}

double SupportLib::verificaServiceLevel(erlangRegister e)
{
    double newN = N(e);

    while (serviceLevel(e, newN) < e.requiredServiceLevel)
    {
        newN += 1;
        erlang(e, newN);
        serviceLevel(e, newN);
    }
    return serviceLevel(e, newN);
}