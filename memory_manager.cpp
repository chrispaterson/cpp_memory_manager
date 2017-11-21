#include "memory_manager.h"
#include <iostream>
#include <vector>
#include <cstdlib>

/******************************************************
 *  main
 ******************************************************/
int main() {

  std::array< std::unique_ptr< AllocatedMemBlock >, NUM_BLOCKS > memory;

  // Create our initial empty process
  allocate_initial_block(memory);

  // process input file and write output file
  processFile(memory);

  return EXIT_SUCCESS;
}

/******************************************************
 *  function implementations
 ******************************************************/
/*
 * chunks through the input file and generates the
 * output file
 * @param memory     reference to array of unique pointers
 *                   that is a simulation of memory
 *
 */
void processFile(std::array< std::unique_ptr< AllocatedMemBlock >, NUM_BLOCKS > &memory) {

  // input and output file streams
  std::ifstream input_file(INPUT_FILE_NAME);
  std::ofstream output_file(OUTPUT_FILE_NAME);

  if(output_file) {

    if(input_file) {

      while(!input_file.eof()) {

        // variables for reading from each line of
        // the input file
        char action;
        int pid;
        int memory_amount;

        // read the input values into the variables
        input_file >> action;
        input_file >> pid;
        input_file >> memory_amount;

        // determine our action
        switch(action) {

          // A = allocate memory block
          case 'A':

            allocate(pid, memory_amount, memory);
            break;

            // P = write all the AllocatedMemBlock’s to
            // the output file in order from smallest to
            // largest in size.
          case 'P':

            print(output_file, memory);
            break;

            // R = release memory block
          case 'R':

            release(pid, memory);
            break;

            // S = stops the simulation
          case 'S':

            stop(input_file, output_file);
            break;

            // Z = simulates a reboot of the computer.
            // All allocated memory is released. 1 Meg
            // of contiguous memory is available.
          case 'Z':

            reboot(memory);
            break;

            // if there is an unknown action code
            // that messes up so we exit with a fail
          default:

            // close files, print error and exit
            input_file.close();
            output_file.close();
            std::cerr << "Invalid input in " << INPUT_FILE_NAME << " unable to proceed. I can do nothing with action code: " << action << std::endl;
            exit(EXIT_FAILURE);
            break;
        }
      }

    } else {

      // problem openeing input file
      std::cerr << "Cannot open " << INPUT_FILE_NAME << std::endl;
      exit(EXIT_FAILURE);
    }
  } else {

    // problem openeing output file
    std::cerr << "Cannot open " << OUTPUT_FILE_NAME << std::endl;
    exit(EXIT_FAILURE);
  }
}

/*
 * allocates "memory" from our array by using the "best fit"
 * algorithm to find the closest non-negative memory block
 *
 * @param pid        process id to label it
 * @param amount     amount of memory needed
 * @param memory     reference to array of unique pointers
 *                   that is a simulation of memory
 *
 */
void allocate(int pid, int amount, std::array< std::unique_ptr< AllocatedMemBlock >, NUM_BLOCKS > &memory){

  // gets number of full blocks
  int memBlocks = amount / BLOCK_SIZE;

  // our Needle in the haystack
  int idx = 0;

  // used to store our closest best fit
  // block grouping.  Intitialized to the
  // maximum value so hopefully we can do
  // better than that.
  int bestFit = NUM_BLOCKS;

  // keep a var for if it's an exact fit,
  // if it is we won't split our block but
  // instead just write over it.
  bool exactFit = false;

  // adds a block to house the remainder
  if(amount % BLOCK_SIZE > 0) {
    memBlocks++;
  }

  // create a flag to know if we have at
  // least one place for the memory to go
  bool foundAPlace = false;

  // loop through all the blocks, we update the iterator
  // in the loop with jumps
  for(int i = 0; i < NUM_BLOCKS; ){

    // we have a 'free' location
    if(memory[i]->ProcessId == UNUSED_PID) {

      // we have an unused process id, let's get our
      // number of available blocks at this location
      int availableBlocksHere = memory[i]->MemBlocks;

      // if we have more blocks than we need, if not
      // this memory will not fit here.
      if(availableBlocksHere >= memBlocks) {

        // We have at least one place the process can
        // fit so let's signal that we can use it.
        foundAPlace = true;

        // get the difference of how many blocks we need
        // vs how many are available the smaller the
        // difference the better the fit
        int difference = availableBlocksHere - memBlocks;

        // if it's less difference then it's a better fit
        if(difference <= bestFit) {

          // keep track of our best fit and update our
          // index to this one.
          bestFit = difference;
          idx = i;

          // if it's an exactly fit we won't split the
          // memory we'll just reassign it so we want to
          // know if it's exactly exact.
          if(difference == 0) {
            exactFit = true;
            break;
          }
        }
      }
    }

    // skip ahead to the next pointer that has a value
    i += memory[i]->MemBlocks;
  }

  // if we have found at least one place to allocate the
  // memory we will do it.  If we don't then we just
  // drop it on the floor with no feedback, sorry.
  if(foundAPlace) {

    // as long as it's not an exact fit.  If it is an
    // exact fit, we don't need to split the memory
    // into two pieces because we'll just overwrite
    // the free memory to have the new memory.
    if(!exactFit) {

      // move unused portion block back

      // create a new pointer for unused bit
      memory[idx + memBlocks] = std::unique_ptr<AllocatedMemBlock>(new AllocatedMemBlock);

      // set to the 'free' process id
      memory[idx + memBlocks]->ProcessId = UNUSED_PID;

      // new memblocks size is the the old minus the chunk
      // of new process memory we're using
      memory[idx + memBlocks]->MemBlocks = memory[idx]->MemBlocks - memBlocks;

      // our use maximum amount of memory per block of 'free
      // memory
      memory[idx + memBlocks]->MemorySize = memory[idx + memBlocks]->MemBlocks * BLOCK_SIZE;
    }

    // update to some new values
    memory[idx]->ProcessId = pid;
    memory[idx]->MemBlocks = memBlocks;
    memory[idx]->MemorySize = amount;

  }
}

/*
 * releases "memory"
 *
 * @param pid        process id to release
 * @param memory     reference to array of unique pointers
 *                   that is a simulation of memory
 *
 */
void release(int pid, std::array< std::unique_ptr< AllocatedMemBlock >, NUM_BLOCKS > &memory){

  // loop through each block looking for processId
  for(int i = 0; i < NUM_BLOCKS; ) {

    // capture our jump
    int jump = memory[i]->MemBlocks;

    // if we found our process id
    if(pid == memory[i]->ProcessId) {

      // reset it to pid 0 making it available to write over
      memory[i]->ProcessId = 0;

      // set memory to maximum value (it's really just free memory)
      memory[i]->MemorySize = memory[i]->MemBlocks * BLOCK_SIZE;

      // we found it, therefore no need to continue on this loop
      break;
    }

    // jump to next block
    i += jump;
  }
}

/*
 * writes to specified file current state of "memory"
 *
 * @param output_file  file to write to
 * @param memory     reference to array of unique pointers
 *                   that is a simulation of memory
 *
 */
void print(std::ofstream &output_file, std::array< std::unique_ptr< AllocatedMemBlock >, NUM_BLOCKS > &memory){

  int i;
  int j;
  int tmp;

  std::vector<int> indexes;

  for(i = 0; i < NUM_BLOCKS; ) {

    // add the index of the memory array to
    // the vector
    indexes.push_back(i);

    // add our jump
    i += memory[i]->MemBlocks;
  }

  // insertion sort to sort our vector based on 
  // the number of memory blocks
  for(i = 0; i < indexes.size(); i++ ) {
    tmp = indexes[i];
    j = i;

    while(j > 0 && memory[indexes[j]]->MemBlocks > memory[tmp]->MemBlocks) {
      indexes[j] = indexes[j-1];
      --j;
    }
    indexes[j] = tmp;
  }

  // write sorted output to output_file
  for(i = 0; i < indexes.size(); i++ ) {

    // output record
    output_file << "AllocatedMemBlock " << (i + 1) << std::endl;
    output_file << DISPLAY_INDENT << "Process Id = " << memory[indexes[i]]->ProcessId << std::endl;
    output_file << DISPLAY_INDENT << "MemBlocks = " << memory[indexes[i]]->MemBlocks << std::endl;
    output_file << DISPLAY_INDENT << "MemorySize = " << memory[indexes[i]]->MemorySize << std::endl;
  }

  // add a line break after each print call
  output_file << std::endl;
}

/*
 * Close and exit the program
 *
 * @param output_file  file to close
 * @param input_file   file to close
 *
 */
void stop(std::ifstream &input_file, std::ofstream &output_file){

  //close files
  input_file.close();
  output_file.close();

  // exit program
  exit(EXIT_SUCCESS);
}

/*
 * clears the memory array by
 * resetting all of the pointers
 * @param memory     reference to array of unique pointers
 *                   that is a simulation of memory
 *
 */
void reboot(std::array< std::unique_ptr< AllocatedMemBlock >, NUM_BLOCKS > &memory){

  // loop through all the blocks
  for(int i = 0; i < NUM_BLOCKS; ) {

    // capture our jump amount to the next
    // allocated chunk
    int jump = memory[i]->MemBlocks;

    // reset unique_ptr
    memory[i].reset();

    // add jump amount to the next block
    i += jump;
  }

  // setup for a clean slate
  allocate_initial_block(memory);
}

/*
 * Creates process with all the memory
 * 'free' that can be used.
 * @param memory     reference to array of unique pointers
 *                   that is a simulation of memory
 *
 */
void allocate_initial_block(std::array< std::unique_ptr< AllocatedMemBlock >, NUM_BLOCKS > &memory) {

  // create our unique_ptr with pointer with new struct value
  memory[0] = std::unique_ptr<AllocatedMemBlock>(new AllocatedMemBlock);

  // fills values with defaults and maxes
  memory[0]->ProcessId = UNUSED_PID;
  memory[0]->MemBlocks = NUM_BLOCKS;
  memory[0]->MemorySize = NUM_BLOCKS * BLOCK_SIZE;
}


