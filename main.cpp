#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <bits/stdc++.h>
#include <random>
#include <nlohmann/json.hpp>

struct RockoWord
{
    std::string word;
    std::vector<std::string> relatedWords;
};

std::vector<std::string> splitSentence(std::string sentence);
void saveRockoData(std::vector<RockoWord>& data, const std::string& filename);
bool loadRockoData(std::vector<RockoWord>& data, const std::string& filename);

int main()
{
    //random value initialisation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1000000);

    //initialise rocko data
    std::vector<RockoWord> rockoData;

    std::string filename = "/home/Refenestrated/projects/Rocko/data/rockoData.json";
    loadRockoData(rockoData, filename);

    //initialise responses
    std::string userResponse { "" };
    std::string rockoResponse { "" };

    while (userResponse != "/")
    {
        rockoResponse.clear();

        std::getline(std::cin, userResponse);
        if (userResponse == "/") break;

        std::vector<std::string> userInputArray = splitSentence(userResponse);

        //search rocko data for input words
        for (int i = 0; i < userInputArray.size(); i++)
        {
            bool wordFound { false };
            for (int j = 0; j < rockoData.size(); j++)
            {
                if (rockoData[j].word == userInputArray[i])
                {
                    wordFound = true;
                }
            }
            if (!wordFound)
            {
                //add word to rocko dataset if not already present
                RockoWord newWord;
                newWord.word = userInputArray[i];
                if (i + 1 < userInputArray.size())
                {
                    newWord.relatedWords.push_back(userInputArray[i+1]);
                }
                rockoData.push_back(newWord);
            }
        }

        std::string lastWord { "" };
        int maxResponseLength = (distrib(gen) % 12) + 6; // Max 8 words in the response
        for (int i = 0; i < maxResponseLength; i++)
        {
            std::string wordToAdd;
            int randItem;

            //if it is the start of a sentence it picks a random start
            if (i == 0)
            {
                randItem = distrib(gen) % rockoData.size();
                wordToAdd = rockoData[randItem].word;
            }
            else //otherwise the answer is influenced by relatedWords
            {
                std::vector<RockoWord> likelyWords;
                for (int j = 0; j < rockoData.size(); j++)
                {
                    int wordCount = count(rockoData[j].relatedWords.begin(), rockoData[j].relatedWords.end(), lastWord);

                    if (wordCount > 0)
                    {
                        likelyWords.push_back(rockoData[j]);
                    }
                }

                if (likelyWords.empty() || distrib(gen) % 2 == 1)
                {
                    randItem = distrib(gen) % rockoData.size();
                    wordToAdd = rockoData[randItem].word;
                }
                else
                {
                    randItem = distrib(gen) % likelyWords.size();
                    wordToAdd = likelyWords[randItem].word;
                }
            }

            //add word to response
            rockoResponse.append(" " + wordToAdd);
            lastWord = wordToAdd;
        }

        std::cout << rockoResponse << std::endl;
    }

    saveRockoData(rockoData, filename);

    return 0;
}

//sentence splitter function
std::vector<std::string> splitSentence(std::string sentence)
{
    std::stringstream ss(sentence);
    std::string word;
    std::vector<std::string> words;
    while (ss >> word)
    {
        words.push_back(word);
    }
    return words;
}

//saving function
void saveRockoData(std::vector<RockoWord>& data, const std::string& filename)
{
    nlohmann::json jsonData;

    for (const auto& word : data)
    {
        nlohmann::json jWord;
        jWord["word"] = word.word;
        jWord["relatedWords"] = word.relatedWords;
        jsonData.push_back(jWord);
    }

    std::ofstream outFile(filename);
    if (outFile.is_open())
    {
        outFile << jsonData.dump(4);
        outFile.close();
    }
    else
    {
        std::cerr << "Error saving to file: " << filename << std::endl;
    }
}

bool loadRockoData(std::vector<RockoWord>& data, const std::string& filename)
{
    std::ifstream inFile(filename);

    if (!inFile.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    nlohmann::json jsonData;
    inFile >> jsonData;
    inFile.close();

    data.clear();

    for (const auto& jWord : jsonData)
    {
        RockoWord word;
        word.word = jWord["word"];
        word.relatedWords = jWord["relatedWords"];
        data.push_back(word);
    }

    return true;
}
