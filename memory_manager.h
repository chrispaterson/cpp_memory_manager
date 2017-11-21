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
 *  function prototypes
 ******************************************************/
/*
 * chunks through the input file and generates the
 * output file
 *
 */
void processFile(std::array< std::unique_ptr< AllocatedMemBlock >, NUM_BLOCKS > &memory);

/*
 * allocates "memory" from our array by using the "best fit"
 * algorithm to find the closest non-negative memory block
 *
 * @param pid        process id to label it
 * @param amount     amount of memory needed
 *
 */
void allocate(int pid, int amount, std::array< std::unique_ptr< AllocatedMemBlock >, NUM_BLOCKS > &memory);

/*
 * releases "memory" 
 *
 * @param pid        process id to release
 *
 */
void release(int pid, std::array< std::unique_ptr< AllocatedMemBlock >, NUM_BLOCKS > &memory);

/*
 * writes to specified file current state of "memory" 
 *
 * @param output_file  file to write to
 *
 */
void print(std::ofstream &output_file, std::array< std::unique_ptr< AllocatedMemBlock >, NUM_BLOCKS > &memory);

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
void reboot(std::array< std::unique_ptr< AllocatedMemBlock >, NUM_BLOCKS > &memory);

/*
 * Creates process with 'free' id that
 * can be used.
 *
 */
void allocate_initial_block(std::array< std::unique_ptr< AllocatedMemBlock >, NUM_BLOCKS > &memory);
