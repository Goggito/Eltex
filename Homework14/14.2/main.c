#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// main
int main() {
  const char* send_path = "/tmp/fifo_one";
  const char* receive_path = "/tmp/fifo_two";

  int fd_read, fd_write;
  char buffer[1024];
  ssize_t bytes_read;
  size_t data_size;

  if (mkfifo(send_path, 0666) == -1) {
    if (errno != EEXIST) {
      perror("Error creating send_path");
      return 1;
    }
  }

  if (mkfifo(receive_path, 0666) == -1) {
    if (errno != EEXIST) {
      perror("Error creating fifo_two");
      return 1;
    }
  }



  fd_read = open(send_path, O_RDONLY);
  if (fd_read == -1) {
    perror("Error opening send_path");
    return 1;
  }

  fd_write = open(receive_path, O_WRONLY);
  if (fd_write == -1) {
    perror("Error opening fifo_two");
    close(fd_read);
    return 1;
  }

  char data_to_write[] = "What's with the money?\n";
  data_size = sizeof(data_to_write);

  ssize_t bytes_written = write(fd_write, data_to_write, data_size);
  if (bytes_written == -1) {
    perror("Error writing to fifo_two");
  } else if (bytes_written < data_size) {
    fprintf(stderr, "Incomplete write to fifo_two\n");
  }

  bytes_read = read(fd_read, buffer, sizeof(buffer));
  
  if (bytes_read > 0) {
    printf("%s\n", buffer);
  }
  else
  {
    perror("Error reading from send_path");
  }

  close(fd_write);
  close(fd_read);

  return 0;
}
