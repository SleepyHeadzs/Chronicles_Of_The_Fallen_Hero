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

static QuizQuestion set2_riddles1[] = {
    {
        "What has keys but can't open locks?",
        {
            "A map",
            "A piano",
            "A keyboard",
            "A code"
        },
        1
    },
    {
        "I am an odd number. Take away one letter and I become even. What number am I?",
        {
            "Seven",
            "Five",
            "Nine",
            "Eleven"
        },
        0
    },
    {
        "A man looks at a photo. Someone asks who it is. He says: 'Brothers and sisters I have none, but this man's father is my father's son.' Who is in the photo?",
        {
            "His brother",
            "His father",
            "His son",
            "Himself"
        },
        2
    },
    {
        "What comes once in a minute, twice in a moment, but never in a thousand years?",
        {
            "The letter 'M'",
            "The letter 'O'",
            "The number 1",
            "Time"
        },
        0
    },
    {
        "The more you take, the more you leave behind. What am I?",
        {
            "Memories",
            "Footsteps",
            "Breath",
            "Time"
        },
        1
    },
    {
        "Two fathers and two sons go fishing. They catch exactly 3 fish and each person gets one. How?",
        {
            "They shared one fish",
            "It's a grandfather, father, and son",
            "One son didn't eat",
            "They found an extra fish"
        },
        1
    },
    {
        "What can travel around the world while staying in a corner?",
        {
            "A bird",
            "A plane",
            "A stamp",
            "The internet"
        },
        2
    },
    {
        "If you have me, you want to share me. If you share me, you don't have me. What am I?",
        {
            "Money",
            "A secret",
            "Knowledge",
            "Love"
        },
        1
    },
    {
        "What building has the most stories?",
        {
            "A skyscraper",
            "A castle",
            "The library",
            "A hotel"
        },
        2
    },
    {
        "A farmer has 17 sheep, and all but 9 die. How many are left?",
        {
            "8",
            "9",
            "17",
            "0"
        },
        1
    }
};