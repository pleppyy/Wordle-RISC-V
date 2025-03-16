#include <stdio.h>
#include <stdlib.h>

#define WORD_COUNT 219
#define WORD_LENGTH 5

// Unique word bank (219 five-letter words)
const char *word_bank[WORD_COUNT] = {
    "abide", "alien", "align", "allow", "alpha", "angel", "angle", "angry", "apple", "aroma",
    "blaze", "blend", "bloom", "bluff", "brawn", "brick", "brisk", "brood", "chair", "chalk",
    "charm", "chase", "chime", "cider", "civic", "cliff", "cloud", "crisp", "crown", "daisy",
    "dealt", "delve", "diver", "douse", "dream", "drift", "drive", "drown", "dunce", "dwarf",
    "eager", "elbow", "elite", "elude", "ember", "equal", "evoke", "exile", "fable", "feast",
    "fjord", "flame", "flint", "flock", "flute", "forge", "frisk", "frost", "frown", "giant",
    "glade", "gleam", "glint", "globe", "gloom", "grain", "grape", "grasp", "gravy", "grind",
    "harsh", "hasty", "hatch", "haven", "hazel", "heart", "hitch", "honor", "hover", "ideal",
    "image", "inbox", "infer", "inlet", "inner", "input", "ivory", "jazzy", "joint", "joker",
    "jolly", "joust", "jumbo", "jumpy", "karma", "kayak", "knack", "knead", "kneel", "knoll",
    "latch", "laugh", "lemon", "light", "lithe", "lodge", "logic", "lunar", "magic", "mango",
    "marsh", "medal", "mercy", "mirth", "moist", "navel", "nerve", "nifty", "noble", "north",
    "novel", "nudge", "nylon", "oasis", "ocean", "olive", "omega", "optic", "orbit", "overt",
    "oxide", "peace", "pearl", "petal", "piano", "plume", "prank", "pride", "print", "prize",
    "quail", "quake", "quark", "quest", "quick", "quiet", "quilt", "quirk", "raven", "rebel",
    "rider", "ridge", "risky", "rival", "rogue", "rover", "rumor", "shade", "silly", "siren",
    "slope", "spine", "spore", "stone", "storm", "sword", "table", "tango", "tease", "thorn",
    "tiger", "token", "trace", "treat", "trend", "trump", "trunk", "tweak", "twist", "ultra",
    "umbra", "uncle", "uncut", "under", "unfit", "unity", "unzip", "upset", "urban", "usher",
    "vapor", "vexed", "vigor", "vista", "vivid", "vocal", "vowel", "waist", "waste", "weird",
    "whale", "wheat", "wield", "wound", "woven", "wrist", "xenon", "xeric", "xerox", "xylem",
    "yacht", "yeast", "yield", "yodel", "young", "youth", "zebra", "zesty", "zonal"
};

int main() {
    //seeding random number generator
    srand(*((unsigned int*)0xff202100));

    // picks a random word from the list
    int random_index = rand() % WORD_COUNT;
    printf("Randomly selected word: %s\n", word_bank[random_index]);

    return 0;
}
