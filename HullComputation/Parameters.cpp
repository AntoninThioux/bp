/**
 * @file Parameters.cpp
 * @author Antonin Thioux (antonin.thioux@gmail.com)
 * @brief This file contains the logic for gathering and parsing parameters.
 * @date last modified at 2022-06-02
 * @version 1.0
 */

#include "Parameters.h"

#define DEFAULT_GRAYSCALE 0
#define DEFAULT_VIEW_SLICE -1 // -1 isn't a valid value it should be overwritten
#define DEFAULT_TIMER 0
#define DEFAULT_BATCHES 1
#define DEFAULT_KERNEL_SIZE_X 3
#define DEFAULT_KERNEL_SIZE_Y 3
#define DEFAULT_KERNEL_SIZE_Z 3
#define DEFAULT_KERNEL_SIZE_T 3
#define DEFAULT_THRESHOLD 100
#define DEFAULT_EXPORT_ANIMATION 0
#define DEFAULT_SPECIAL 0

using namespace HullComputation;
using namespace std;

/**
 * @brief This function parses data dimension files to extract its parameters.
 * @param filename The path to file.
 */
void Parameters::parseFile(string filepath){
    file.open(filepath);
    if (!file) printError("File not found!");
    
    string line;
    stringstream ss;

    if (!getline(file, line)) printError("Expected file found directory!");
    ss << line;
    ss >> width >> height >> depth;
    is4D = (depth != 1);
    ss.clear();

    getline(file, line);
    ss << line;
    ss >> duration;

    datafiles = (string *) malloc(sizeof(string) * duration);
    string path = filepath.substr(0, filepath.rfind('/') + 1);
    for (int i = 0; i < duration; i++) {
        getline(file, line);
        datafiles[i] = path + line;
    }
}

/**
 * @brief This function parse options / flogs of the program.
 * @param n The number of options in the list.
 * @param options The list of options.
 */
void Parameters::parseOptions(int n, char **options) {
    for (int i = 0; i < n; i++) {
        string flag(options[i]);

        if (flag == "-h" || flag == "--help") printHelp();
        else if (flag == "-t" || flag == "--timer") isTimed = 1;
        else if (flag == "-gs" || flag == "--gray-scale") isViewed = 1;
        else if (flag == "--view-slice") sscanf(options[++i], "%d", &viewSlice);
        else if (flag == "-b" || flag == "--batches") sscanf(options[++i], "%d", &batches);
        else if (flag == "-th" || flag == "--threshold") sscanf(options[++i], "%d", &threshold);
        else if (flag == "-kx" || flag == "--kernel-x-size") sscanf(options[++i], "%d", &kx);
        else if (flag == "-ky" || flag == "--kernel-y-size") sscanf(options[++i], "%d", &ky);
        else if (flag == "-kz" || flag == "--kernel-z-size") sscanf(options[++i], "%d", &kz);
        else if (flag == "-kt" || flag == "--kernel-t-size") sscanf(options[++i], "%d", &kt);
        else if (flag == "-ea" || flag == "--export-animation") exportAnimation = 1;
        else if (flag == "-s" || flag == "--special") sscanf(options[++i], "%d", &special);
        else printError("Unknown flag!");
    }
}

/**
 * @brief This function performs misc tests on parameters to check consitences.
 */
void Parameters::checkParameters(){
    if (0 >= threshold) printError("Threshold value too small!");
    if (kx > width) printError("Kernel x size too large!");
    if (kx < 3) printError("Kernel x size too small must be alteast 3!");
    if (ky > height) printError("Kernel y size too large!");
    if (ky < 3) printError("Kernel y size too small must be alteast 3!");
    if (kz > depth && is4D) printError("Kernel z size too large!");
    if (kz < 3 && is4D) printError("Kernel z size too small must be alteast 3!");
    if (kt > duration) printError("Kernel t size too large!");
    if (kt < 3) printError("Kernel t size too small must be alteast 3!");

    if (!isViewed && viewSlice != -1) printError("View slice given but grayscale off!");
    if (isViewed && viewSlice == -1 && is4D) printError("4D date viewed without view slice!");
    if (isViewed && viewSlice != -1 && !is4D) printError("View slice given for 3D data!");
    if (isViewed && is4D && !(0 <= viewSlice && viewSlice <= depth - kz + 1)) printError("Invalid view slice size!");

    if (0 >= batches) printError("Too little batches must be atleast 1!");
    if (batches > duration - kt + 1) printError("Too many batches!");

    if (special != 0 && special != 1 && special != 2) printError("Invalid special value");
}

/**
 * @brief This function prints the help menu for flags and what not.
 */
void Parameters::printHelp(){
    cout << "Usage: compute <DIMENSION-FILE> [OPTIONS]" << endl;

    cout << "DIMENSION-FILE is:" << endl;
    cout << "\tThe path to the input dimension file." << endl;
    cout << "\tThe dimension file's first line should contain 3 ints denoting the size of Spatio images." << endl;
    cout << "\tThe next line in the file should be the number of n frames in the Spatio-Temporal data." << endl;
    cout << "\tThe last n lines should be the directory of the n images." << endl;

    cout << "OPTIONS are:" << endl;
    cout << "\t-h,  --help \t\tDisplays this menu" << endl;
    cout << "\t-t,  --timer \t\tWhen this option is on steps in the computation pipeline will be timed" << endl;
    cout << "\t-gs, --gray-scale \tWhen this option is on a grayscale of the hulls is displayed" << endl;
    cout << "\t     --view-slice \tThe integer following this option gives the z slice to display in grayscale of 3D hulls" << endl;
    cout << "\t-b,  --batches \t\tThe integer value following this option gives the number of batches (DEFAULT=" << DEFAULT_BATCHES << ")" << endl;
    cout << "\t-th, --threshold \tThe integer following this option gives the threshold used in hull computation (DEFAULT=" << DEFAULT_THRESHOLD << ")" << endl;
    cout << "\t-kx, --kernel-x-size \tThe integer following this option gives the kernel x size used in hull computation (DEFAULT=" << DEFAULT_KERNEL_SIZE_X << ")" << endl;
    cout << "\t-ky, --kernel-y-size \tThe integer following this option gives the kernel y size used in hull computation (DEFAULT=" << DEFAULT_KERNEL_SIZE_Y << ")" << endl;
    cout << "\t-kz, --kernel-z-size \tThe integer following this option gives the kernel z size used in hull computation (DEFAULT=" << DEFAULT_KERNEL_SIZE_Z << ")" << endl;
    cout << "\t-kt, --kernel-t-size \tThe integer following this option gives the kernel t size used in hull computation (DEFAULT=" << DEFAULT_KERNEL_SIZE_T << ")" << endl;
    cout << "\t-ea, --export-animation\tWhen this option is on the animation is exported with the hulls in .obj files" << endl;
    cout << "\t-s,  --special \t\tThe following number in range [0-2] gives different ways of computing the hulls (DEFAULT=" << DEFAULT_SPECIAL << ")" << endl;
}

/**
 * @brief This function displays a parameter usage error.
 * @param error The error message to display.
 */
void Parameters::printError(const char *error){
    cerr << "[Usage Error]: \t" << error << endl;
    cout << "compute -h \t for help" << endl;
    exit(EXIT_FAILURE);
}

/**
 * @brief Construct a new Parameters object by parsing program arguments.
 * @param argc Number of arguments.
 * @param argv Array of program arguments.
 */
Parameters::Parameters(int argc, char *argv[])
:isViewed(DEFAULT_GRAYSCALE),viewSlice(DEFAULT_VIEW_SLICE),isTimed(DEFAULT_TIMER),batches(DEFAULT_BATCHES),
kx(DEFAULT_KERNEL_SIZE_X),ky(DEFAULT_KERNEL_SIZE_Y),kz(DEFAULT_KERNEL_SIZE_Z),kt(DEFAULT_KERNEL_SIZE_Z),threshold(DEFAULT_THRESHOLD)
,exportAnimation(DEFAULT_EXPORT_ANIMATION),special(DEFAULT_SPECIAL){
    if (argc == 1)  // No file guard
        printError("No file given!");

    // Help menu guard
    if (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))){
        printHelp();
        exit(EXIT_SUCCESS);
    }

    parseFile(std::string(argv[1]));
    if (argc > 2) parseOptions(argc - 2, &argv[2]);
    checkParameters();
}

/**
 * @brief Destroy the Parameters object
 */
Parameters::~Parameters(){
    file.close();
    free(datafiles);
}
