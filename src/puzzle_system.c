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

static QuizQuestion set3_riddles2[]={
    {
        "What has a neck but no head?",
        {
            "A snake",
            "A bottle",
            "A shirt",
            "A guitar"
        },
        1
    },
    {
        "I speak without a mouth and hear without ears. I have no body, but I come alive with wind. What am I?",
        {
            "A ghost",
            "A shadow",
            "An echo",
            "A dream"
        },
        2
    },
    {
        "A man pushes his car to a hotel and tells the owner he's bankrupt. Why?",
        {
            "He lost a bet",
            "He's playing Monopoly",
            "His car broke down",
            "He has no money"
        },
        1
    },
    {
        "What gets wetter the more it dries?",
        {
            "A sponge",
            "A towel",
            "The ground",
            "Paint"
        },
        1
    },
    {
        "Forward I am heavy, backward I am not. What am I?",
        {
            "The word 'ton'",
            "A truck",
            "Time",
            "A rock"
        },
        0
    },
    {
        "There's a one-story house where everything is pink. What color are the stairs?",
        {
            "Pink",
            "There are no stairs",
            "White",
            "Brown"
        },
        1
    },
    {
        "You see a boat filled with people, yet there isn't a single person on board. How?",
        {
            "They're all married",
            "It's a ghost ship",
            "They're below deck",
            "It's a model"
        },
        0
    },
    {
        "What can you catch but not throw?",
        {
            "A ball",
            "A cold",
            "A fish",
            "A frisbee"
        },
        1
    },
    {
        "I have branches but no fruit, trunk, or leaves. What am I?",
        {
            "A river",
            "A bank",
            "A family tree",
            "A road"
        },
        1
    },
    {
        "What has one eye but can't see?",
        {
            "A cyclops",
            "A needle",
            "A storm",
            "A potato"
        },
        1
    }
};

static QuizQuestion set4_general[] = {
    {
        "What is the largest ocean on Earth?",
        {
            "Atlantic Ocean",
            "Indian Ocean",
            "Pacific Ocean",
            "Arctic Ocean"
        },
        2
    },
    {
        "Which planet is known as the 'Red Planet'?",
        {
            "Venus",
            "Mars",
            "Jupiter",
            "Saturn"
        },
        1
    },
    {
        "Who painted the Mona Lisa?",
        {
            "Michelangelo",
            "Leonardo da Vinci",
            "Raphael",
            "Donatello"
        },
        1
    },
    {
        "What is the capital city of Australia?",
        {
            "Sydney",
            "Melbourne",
            "Canberra",
            "Brisbane"
        },
        2
    },
    {
        "Which element has the chemical symbol 'Fe'?",
        {
            "Fluorine",
            "Francium",
            "Iron",
            "Fermium"
        },
        2
    },
    {
        "How many continents are there?",
        {
            "Five",
            "Six",
            "Seven",
            "Eight"
        },
        2
    },
    {
        "What is the longest river in the world?",
        {
            "Amazon",
            "Nile",
            "Yangtze",
            "Mississippi"
        },
        1
    },
    {
        "Who wrote 'Romeo and Juliet'?",
        {
            "Charles Dickens",
            "William Shakespeare",
            "Jane Austen",
            "Mark Twain"
        },
        1
    },
    {
        "What is the tallest mountain in the world?",
        {
            "K2",
            "Mount Kilimanjaro",
            "Mount Everest",
            "Mount Fuji"
        },
        2
    },
    {
        "Which country gifted the Statue of Liberty to the United States?",
        {
            "United Kingdom",
            "Germany",
            "France",
            "Spain"
        },
        2
    }
};

static QuizQuestion set5_maths[]={
    {
        "If a hen and a half lays an egg and a half in a day and a half, how many eggs does one hen lay in one day?",
        {
            "0.5 eggs",
            "1 egg",
            "1.5 eggs",
            "2 eggs"
        },
        1
    },
    {
        "I am a three-digit number. My tens digit is five more than my units digit. My hundreds digit is eight less than my tens digit. What number am I?",
        {
            "194",
            "491",
            "950",
            "592"
        },
        0
    },
    {
        "A bat and a ball cost $1.10 together. The bat costs $1.00 more than the ball. How much does the ball cost?",
        {
            "5 cents",
            "10 cents",
            "$1.00",
            "$1.05"
        },
        0
    },
    {
        "If you multiply me by any other number, the answer will always remain the same. What number am I?",
        {
            "One",
            "Zero",
            "Negative one",
            "Infinity"
        },
        1
    },
    {
        "A rope burns unevenly and takes exactly 60 minutes to burn completely. How do you measure exactly 45 minutes using two such ropes?",
        {
            "Light both ends of one rope and one end of the other",
            "Fold the rope in half",
            "Burn them sequentially",
            "Use a stopwatch"
        },
        0
    },
    {
        "Three friends split a $30 bill evenly. They get a $5 refund, take $1 each, and the waiter keeps $2. They paid $9 each = $27, plus $2 tip = $29. Where is the missing $1?",
        {
            "The waiter stole it",
            "There is no missing dollar",
            "It's in the refund",
            "Math error"
        },
        1
    },
    {
        "What is the next number in the sequence: 1, 11, 21, 1211, 111221, ...?",
        {
            "221111",
            "312211",
            "13112221",
            "1113213211"
        },
        1
    },
    {
        "If two's company and three's a crowd, what are four and five?",
        {
            "A party",
            "Nine",
            "Too many",
            "A group"
        },
        1
    },
    {
        "A clock loses 15 minutes every hour. If set correctly at noon, what will it show at midnight (real time)?",
        {
            "9:00 PM",
            "10:00 PM",
            "11:00 PM",
            "Midnight"
        },
        0
    },
    {
        "I'm an even number. Remove one letter and I become odd. What number am I?",
        {
            "Eight",
            "Six",
            "Four",
            "Ten"
        },
        0
    }
};

static QuizQuestion *allSets[5] = {set1_tech, set2_riddles1, set3_riddles2, set4_general, set5_maths};
static int setSizes[5] = {10, 10, 10, 10, 10};

static QuizQuestion *currentQuestions;
static int currentSetSize;
static int currentQuestion;
static int selectedOption;
static int puzzleState;
static int lives;
static int score;
static float timer;
static const float TIME_LIMIT = 15.0f;
static int chapter;
static int questionsPerChapter;