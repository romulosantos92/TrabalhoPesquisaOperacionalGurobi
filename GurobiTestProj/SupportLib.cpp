#include "SupportLib.h"

void SupportLib::leArquivo()
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

        while (registros.size() > 0)
        {
            aux = registros.back();
            registros.pop_back();
            SupportLib::printRegister(aux);
        }

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
    if (n == 1)
        return n;
    else
        return n*SupportLib::fatorial(n - 1);
}