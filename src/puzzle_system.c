#include "raylib.h"
#include "puzzle_system.h"
#include <string.h>

typedef struct{
    const char *question;
    const char *options[4];
    int correctAnswer;
}QuizQuestion;

static QuizQuestion set1_tech[]={
    {
        "What does 'HTTP' stand for?",
        {
            "HyperText Transfer Protocol",
            "High Tech Transfer Process",
            "Home Tool Transfer Program",
            "Hyperlink Text Transmission Protocol"
        },
        0
    },
    {
        "Which company developed Android before Google acquired it?",
        {
            "Apple Inc.",
            "Android Inc. (founded by Andy Rubin)",
            "Microsoft Mobile",
            "Nokia Corporation"
        },
        1
    },
    {
        "What is the name of the first electronic general-purpose computer, completed in 1945?",
        {
            "UNIVAC",
            "Colossus",
            "ENIAC",
            "IBM 701"
        },
        2
    },
    {
        "What does 'RAM' stand for?",
        {
            "Read And Modify",
            "Random Access Memory",
            "Rapid Action Module",
            "Remote Application Manager"
        },
        1
    },
    {
        "Which programming language is named after a type of coffee?",
        {
            "Python",
            "C++",
            "Java",
            "Ruby"
        },
        2
    },
    {
        "What is the term for a malicious program that disguises itself as legitimate software?",
        {
            "Virus",
            "Worm",
            "Trojan horse",
            "Spyware"
        },
        2
    },
    {
        "Who is credited as the co-founder of Apple alongside Steve Jobs?",
        {
            "Bill Gates",
            "Steve Wozniak",
            "Tim Cook",
            "Paul Allen"
        },
        1
    },
    {
        "What does 'URL' stand for?",
        {
            "Universal Resource Link",
            "Uniform Resource Locator",
            "Unified Reference Label",
            "User Requested Location"
        },
        1
    },
    {
        "Which social media platform was originally called 'The Facebook'?",
        {
            "Twitter",
            "Instagram",
            "Facebook",
            "MySpace"
        },
        2
    },
    {
        "What year was the World Wide Web made publicly available?",
        {
            "1989",
            "1990",
            "1991",
            "1993"
        },
        2
    }
};