#pragma once

#include "AllocatedMemBlock.h"
#include <array>
#include <string>
#include <memory>
#include <fstream>

/******************************************************
 *  Constants
 ******************************************************/
// file constants
const std::string INPUT_FILE_NAME = "input.txt";
const std::string OUTPUT_FILE_NAME = "output.txt";

// how many blocks and number of blocks
const int BLOCK_SIZE = 512;
const int NUM_BLOCKS = 2048;

// unused process id to use for 'free' memory
const unsigned int UNUSED_PID = 0;

// for output display indentation
const std::string DISPLAY_INDENT = "   ";

/******************************************************
 *  Global Variables
 ******************************************************/
// it should be mentioned that I created this here because
// if this were a class this would of course be private
// variable that I could access within the scope of my
// class, however because we aren't allowed to use classes
// I'm forced to use a global variable here.  I could of 
// course make a method to return the value but I think 
// that, in this case would be ugly.
//
// this array is the modeled memory that I will use each
// index as a BLOCK_SIZE chunk of memory.

std::array< std::unique_ptr< AllocatedMemBlock >, NUM_BLOCKS > memory;

/******************************************************
 *  function prototypes
 ******************************************************/
/*
 * chunks through the input file and generates the
 * output file
 *
 */
void processFile();

/*
 * allocates "memory" from our array by using the "best fit"
 * algorithm to find the closest non-negative memory block
 *
 * @param pid        process id to label it
 * @param amount     amount of memory needed
 *
 */
void allocate(int pid, int amount);

/*
 * releases "memory" 
 *
 * @param pid        process id to release
 *
 */
void release(int pid);

/*
 * writes to specified file current state of "memory" 
 *
 * @param output_file  file to write to
 *
 */
void print(std::ofstream &output_file);

/*
 * Close and exit the program 
 *
 * @param output_file  file to close
 * @param input_file   file to close
 *
 */
void stop(std::ifstream &input_file, std::ofstream &output_file);

/*
 * clears the memory
 *
 */
void reboot();

/*
 * Creates process with 'free' id that
 * can be used.
 *
 */
void allocate_initial_block();
