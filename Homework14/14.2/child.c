#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// child
int main() {
  const char* fifo_one = "/tmp/fifo_one";
  const char* fifo_two = "/tmp/fifo_two";

  int fd_write = open(fifo_one, O_WRONLY);
  if (fd_write == -1) {
    perror("Error opening fifo_one");
    return 1;
  }

  int fd_read = open(fifo_two, O_RDONLY);
  if (fd_read == -1) {
    perror("Error opening fifo_two");
    close(fd_write);
    return 1;
  }

  char buffer[1024];
  ssize_t bytes_read;

  bytes_read = read(fd_read, buffer, sizeof(buffer));

  if (bytes_read > 0) {
    printf("%s\n", buffer);
  }
  else
  {
    perror("Error reading from send_path");
  }

  char data_to_write[] = "What money?\n";
  size_t data_size = sizeof(data_to_write);
  ssize_t bytes_written = write(fd_write, data_to_write, data_size);
  if (bytes_written == -1) {
    perror("Error writing to fifo_one");
  } else if (bytes_written < data_size) {
    fprintf(stderr, "Incomplete write to fifo_one\n");
  }

  close(fd_read);
  close(fd_write);

  return 0;
}
