//
//  main.cpp
//  Project 5
//
//  Created by nitya khanna on 11/05/22.
//

#include <iostream>
#include <cassert>
#include <cstring>
using namespace std;
const int MAX_WORD_LENGTH = 20;
const int maxJeetLength= 280;

int editStandards(int distance[],
                  char word1[][MAX_WORD_LENGTH+1],
                  char word2[][MAX_WORD_LENGTH+1],
                  int nStandards)
{
    if (nStandards<=0)
        return 0;
    
    
    int validStandardsCounter=nStandards;
    //remove standards w invalid distance or empty cstrings for words
    for (int i=0 ; i<nStandards ; i++)
    {
        if (distance[i]<=0 || strcmp(word1[i], "")==0 || strcmp(word2[i], "")==0)
        {
            validStandardsCounter--;
            for (int j=i ; j<nStandards-1 ; j++)
            {
                distance[j]=distance[j+1];
                strcpy(word1[j], word1[j+1]);
                strcpy(word2[j], word2[j+1]);
            }
            i--;
        }
    }
    
    //retain only valid standards w valid word1 cstrings
    //reduce valid standard counter accordingly
    int validStandardsCounterWord1= validStandardsCounter;
    int validWord1Counter=0;
    for (int i = 0 ; i < validStandardsCounter; i++)
    {
        int validWord1Check=0;
        for (int j = 0 ; j < strlen(word1[i]); j++)
        {
            if (isalpha(word1[i][j]))
            {
                validWord1Check++;
                word1[i][j]= tolower(word1[i][j]);
            }
        }
        if (validWord1Check == strlen(word1[i]))
        {
            if (i != validWord1Counter)
            {
                strcpy(word1[validWord1Counter],word1[i]);
                strcpy(word2[validWord1Counter],word2[i]);
                distance[validWord1Counter]= distance[i];
                validWord1Counter++;
            }
            else
            {
                distance[validWord1Counter]= distance[i];
                validWord1Counter++;
                continue;
            }
        }
        else
            validStandardsCounterWord1--;
    }
    
    //retain only valid standards w valid word2 cstrings
    //reduce valid standard counter accordingly
    int validStandardsCounterWord2= validStandardsCounterWord1;
    int validWord2Counter=0;
    for (int i = 0 ; i < validStandardsCounterWord1; i++)
    {
        int validWord2Check=0;
        for (int j = 0 ; j < strlen(word2[i]); j++)
        {
            if (isalpha(word2[i][j]))
            {
                validWord2Check++;
                word2[i][j]= tolower(word2[i][j]);
            }
        }
        if (validWord2Check == strlen(word2[i]))
        {
            if (validWord2Counter != i)
            {
                strcpy(word1[validWord2Counter],word1[i]);
                strcpy(word2[validWord2Counter],word2[i]);
                distance[validWord2Counter]= distance[i];
                validWord2Counter++;
            }
            else
            {
                distance[validWord2Counter]= distance[i];
                validWord2Counter++;
                continue;
            }
        }
        else
            validStandardsCounterWord2--;
    }
    
    //initialize all the temp arrays and temp array counters
    int validStandardsCounterFinal=validStandardsCounterWord2;
    int duplicateCounter;
    int tempDist[maxJeetLength+1]={};
    char tempWord1[maxJeetLength+1][MAX_WORD_LENGTH+1]= {{}};
    char tempWord2[maxJeetLength+1][MAX_WORD_LENGTH+1]= {{}};
    int tempArrayIndex=0;
    
    //copy appropriate standards to temp arrays with repeated word1 and word2 depending on distance
    for (int i = 0 ; i < validStandardsCounterWord2; i++)
    {
        duplicateCounter=0;
        for (int j = 0; j < i ; j++)
        {
            if (strcmp(tempWord1[j], word1[i])==0 && strcmp(tempWord2[j], word2[i])==0)
            {
                if (distance[i]>tempDist[j])
                {
                    tempDist[j]= distance[i];
                }
                else
                {
                    duplicateCounter=1;
                    validStandardsCounterFinal--;
                }
            }
        }
        if (duplicateCounter == 0)
        {
            strcpy(tempWord1[tempArrayIndex], word1[i]);
            strcpy(tempWord2[tempArrayIndex], word2[i]);
            tempDist[tempArrayIndex]= distance[i];
            tempArrayIndex++;
        }
    }
    
    //copy elements from temp arrays to actual word1, word2 and distance arrays
    for (int k = 0 ; k < validStandardsCounterFinal ; k++)
    {
        for (int l= 0 ; tempWord1[k][l] !='\0' ; l++)
        {
            word1[k][l]= tempWord1[k][l];
            word1[k][l+1]='\0';
        }
        for (int l= 0 ; tempWord2[k][l] !='\0' ; l++)
        {
            word2[k][l]= tempWord2[k][l];
            word2[k][l+1]='\0';
        }
        distance[k]= tempDist[k];
    }
     
    return validStandardsCounterFinal;
}

int determineMatchLevel(const int distance[],
                        const char word1[][MAX_WORD_LENGTH+1],
                        const char word2[][MAX_WORD_LENGTH+1],
                        int nStandards,
                        const char jeet[])
{
    int matchLevel=0;
    char newJeet[maxJeetLength+1]= {};
    int normalJeetCharCounter=0 ;
    
    if (strcmp(jeet, "")==0)
        return 0;
    
    //eliminate invalid characters from jeet
    for(int i = 0 ; i < strlen(jeet) ; i++)
    {
        if(isalpha(jeet[i]) && !islower(jeet[i]))
        {
            newJeet[normalJeetCharCounter]= tolower(jeet[i]);
            normalJeetCharCounter++;
        }
        else if (isspace(jeet[i]) || (isalpha(jeet[i]) && islower(jeet[i])))
        {
            newJeet[normalJeetCharCounter]=jeet[i];
            normalJeetCharCounter++;
        }
    }
    
    //if newJeet is only spaces return 0
    int onlySpaceCounter = 0;
    for (int i = 0 ; i < normalJeetCharCounter; i++)
    {
        if ( isspace(newJeet[i])){
            onlySpaceCounter++;
        }
    }
    if (onlySpaceCounter==normalJeetCharCounter)
    {
        return 0;
    }
    
    //add each word from Jeet to wordsInJeet array
    char wordsInJeet[maxJeetLength+1][maxJeetLength+1]={{}};
    int wordNumber=0;
    int letterInWord=0;
    for (int i = 0 ; i < normalJeetCharCounter ; i++ )
    {
        if (!isspace(newJeet[i]))
        {
            wordsInJeet[wordNumber][letterInWord]= newJeet[i];
            letterInWord++;
        }
        else if (i!=normalJeetCharCounter && isspace(newJeet[i]) && isspace(newJeet[i+1]))
        {
            continue;
        }
        else if (isspace(newJeet[i]))
        {
            wordsInJeet[wordNumber][letterInWord]='\0';
            wordNumber++;
            letterInWord=0;
        } else if (i==normalJeetCharCounter-1)
        {
            break;
        }
    }
    
    //calculate match level
    int wordsInJeetLen=wordNumber+1;
    int multipleMatchCheckEnable= -1;
    for (int i = 0 ; i < nStandards ; i++)
    {
        for (int j = 0 ; j <wordsInJeetLen ; j++)
        {
            if (strcmp(wordsInJeet[j],word1[i])==0)
            {
                for (int k = j+1 ; k < j + 1 + distance[i] ; k++)
                {
                    multipleMatchCheckEnable=-1;
                    if (strcmp(wordsInJeet[k], word2[i])==0)
                    {
                        matchLevel++;
                        multipleMatchCheckEnable++;
                        break;
                    }
                }
                if (multipleMatchCheckEnable!=-1)
                {
                    break;
                }
            }
        }
    }
    return matchLevel;
}


int main() {
    
    
    const int nstand2=10;
    int distance2[nstand2]= {0, -1, 4, 5, 3, 8, 4, 9, 7, 5};
    char word12[nstand2][MAX_WORD_LENGTH+1]={"the","show","true","comedy","musical","jake","holt","sister","blue","comedy"};
    char word22[nstand2][MAX_WORD_LENGTH+1]={"shoe","horror","system","show","boring","amy","jake","white","police","show"};
    const int n2 = editStandards(distance2, word12, word22, nstand2);
    
    assert(editStandards(distance2, word12, word22, nstand2)==7);
    assert(determineMatchLevel(distance2, word12, word22, n2, "the show that had horror in it was actually a jake & amy musical that wasn't boring") == 2 );
    
    assert(determineMatchLevel(distance2, word12, word22, n2, "") ==  0);
    
    assert(determineMatchLevel(distance2, word12, word22, n2, "sister 's wear white in church and police wears blue at work") ==  1);
   
    const int TEST1_NSTANDARDS = 4;
    int test1dist[TEST1_NSTANDARDS] = {
        2,             4,         1,         13
    };
    char test1w1[TEST1_NSTANDARDS][MAX_WORD_LENGTH+1] = {
        "eccentric",   "space",  "electric", "were"
    };
    char test1w2[TEST1_NSTANDARDS][MAX_WORD_LENGTH+1] = {
        "billionaire", "capsule", "car",     "eccentric"
    };
    assert(determineMatchLevel(test1dist, test1w1, test1w2, TEST1_NSTANDARDS,
        "The eccentric outspoken billionaire launched a space station cargo capsule.") == 2);
    assert(determineMatchLevel(test1dist, test1w1, test1w2, TEST1_NSTANDARDS,
        "The eccentric outspoken billionaire launched    a space capsule.") == 2);
    assert(determineMatchLevel(test1dist, test1w1, test1w2, TEST1_NSTANDARDS,
        "**** 2022 ****") == 0);
    assert(determineMatchLevel(test1dist, test1w1, test1w2, TEST1_NSTANDARDS,
        "  It's an ELECTRIC car!") == 1);
    assert(determineMatchLevel(test1dist, test1w1, test1w2, TEST1_NSTANDARDS,
        "space space capsule space capsule capsule") == 1);
    assert(determineMatchLevel(test1dist, test1w1, test1w2, TEST1_NSTANDARDS,
        "Two eccentric billionaires were space-capsule riders.") == 0);
    
    const int nstand3=6;
    int distance3[nstand3]= {1,2,3,4,5, 2};
    char word13[nstand3][MAX_WORD_LENGTH+1]={"table", "chair", "table", "bottle", " ", "table"};
    char word23[nstand3][MAX_WORD_LENGTH+1]={"wood", "metal","wood","black", "blue", "wood"};
    
    const int n3 = editStandards(distance3, word13, word23, nstand3);
    
    assert(editStandards(distance3, word13, word23, nstand3)==3);
    assert(determineMatchLevel(distance3, word13, word23, n3,
        "The table is made of wood. the me_t_al of the chair is sharp.") == 0);
    assert(determineMatchLevel(distance3, word13, word23, n3,
        "the bottle is a shade    black") == 1);
    
    cout << "All tests succeeded" << endl;
    
}
