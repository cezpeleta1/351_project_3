#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

using namespace std;

#define BUF_SIZE 4096

int main(int argc, char** argv)
{
	/* Make sure the command line is correct */
	if (argc < 2)
	{
		cout << "FILE NAME missing\n";

		exit(1);
	}

	/* Open the specified file */
	int in_fd = open(argv[1], O_RDWR);
	int out_fd = open(argv[2], O_RDWR | O_CREAT);


	if (in_fd < 0)
	{
		cout << "\n" << "input file cannot be opened" << "\n";
		exit(1); //it it cannot be opened, exit
	}

	if (out_fd < 0) {
			cout << "\n" << "output file cannot be created" << "\n";
			exit(1);        // if it cannot be created, exit
	}

	struct stat stats;
	if (stat(argv[1], &stats) == 0)
		cout << endl << "file size " << stats.st_size;
	else
		cout << "Unable to get file properties.\n";

	/* Get the page size  */
	int pagesize = getpagesize();
	cout << endl << "page size is " << pagesize << "\n";

//filesize
int filesize = stats.st_size;

	/* map the file into memory */
	char* data = (char*)mmap(NULL, pagesize, PROT_READ | PROT_WRITE, MAP_SHARED, in_fd, 0);

	/* map the file into memory */
	char* newCopy = (char*)mmap(NULL, pagesize, PROT_READ | PROT_WRITE, MAP_SHARED, out_fd, 0);


  char buffer[BUF_SIZE];
	/* Did the mapping succeed ? */
	if (!data)
	{
		cout << "\n" << "mapping did not succeed" << "\n";
		exit(1);
	}
 int rd_count, wt_count;
	while (pagesize < filesize) {
			rd_count = read(in_fd, buffer, BUF_SIZE);
			if (rd_count <= 0) {   // if end of file or error


					break;
			}
			/*cout<<"\n "<<"counting.......";
			cout<<"\n"<<buffer;*/
			wt_count = write(out_fd, buffer, rd_count);
			if (wt_count <= 0) {  // if error
					cout << "\n" << "error on writing...exiting" << "\n";
					exit(1);
			}

	/* Print the whole file character-by-character */
	for (int fIndex = 0; fIndex < pagesize; ++fIndex)
	{

		cout << data[fIndex];

	}
	cout << endl;
	/* Write a string to the mapped region */
//memcpy(data, "Hello world, this is a test\n", sizeof("Hello world, this is a test"));*/

	/* Unmap the shared memory region */
	munmap(data, pagesize);


	/* Close the file */
	close(in_fd);
	close(out_fd);

	return 0;
}
}
